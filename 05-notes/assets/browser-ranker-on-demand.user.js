// ==UserScript==
// @name         AI 味掃描 (點了才跑) — N_sum & density
// @namespace    https://github.com/jason3e7/ai-101
// @version      1.0.0
// @description  點 Tampermonkey 選單裡的「掃一下 AI 味」才執行, 不會自動掃每頁. 有選取就只量選取範圍, 沒選取就量整頁. 再點一次就關閉面板.
// @author       jason3e7 + Claude
// @match        *://*/*
// @grant        GM_registerMenuCommand
// @run-at       document-idle
// ==/UserScript==

/*
 * 跟 browser-ranker.user.js 的差別:
 *   1. 不自動掃每頁. 要主動點 Tampermonkey 擴充圖示 → 選「掃一下 AI 味」才跑.
 *   2. 支援選取範圍: 有選取 text 就只量選取裡的內容, 沒選取才量整頁容器.
 *   3. 面板是 toggle: 第二次點選單會關掉面板.
 *
 * 9 個訊號、權重、公式都跟 browser-ranker.user.js 一致, 詳見那份 header.
 */

(function () {
  'use strict';

  const PANEL_ID = 'ai-ranker-ondemand-panel';
  const SCALE = 1000;
  const EMOJI_EXCLUDE = new Set(['○', '✗', '★', '☆', '☐']);
  const CORPUS_DENSITY = 69.7; // 2026 鐵人賽全體平均, 供對照

  const RE_EM_DASH = /——/g;
  const RE_EMOJI = /[\u{1F300}-\u{1F5FF}\u{1F600}-\u{1F64F}\u{1F680}-\u{1F6FF}\u{1F700}-\u{1F8FF}\u{1F900}-\u{1F9FF}\u{1FA00}-\u{1FAFF}\u{2600}-\u{26FF}\u{2700}-\u{27BF}\u{1F1E6}-\u{1F1FF}]/gu;
  const RE_STRICT_BOLD_LI = /<li\b[^>]*>\s*(?:<p\b[^>]*>\s*)?<strong\b[^>]*>([^<>]+)<\/strong>[：:\s]*[^\s<]/gi;
  const RE_NOT_BUT = /不(?:只)?是[^。！？\n]{1,25}(?:而是|更是)/g;
  const RE_EASIEST = /最容易[一-鿿]{1,5}的/g;
  const PIPE = '｜'; // U+FF5C

  const N = { em: 4, strict: 3, all: 0, bq: 1, hr: 1.5, notBut: 3, easiest: 3, pipe: 2 };
  const EMOJI_TYPES_CAP = 5;

  function pickContainer() {
    return document.querySelector('.markdown__style') // iThome 文章正文
      || document.querySelector('article')
      || document.querySelector('main')
      || document.body;
  }

  function pickTitle() {
    const el = document.querySelector('.qa-header__title, .qa-list__title, article h1, h1');
    return (el ? el.textContent : document.title) || '';
  }

  // 新: 判斷量「選取範圍」還是「整頁容器」
  function pickTarget() {
    const sel = window.getSelection();
    if (sel && !sel.isCollapsed && sel.toString().trim().length > 0) {
      const range = sel.getRangeAt(0);
      const wrapper = document.createElement('div');
      wrapper.appendChild(range.cloneContents());
      return { container: wrapper, source: '選取範圍 (' + sel.toString().length + ' 字)', isSelection: true };
    }
    const c = pickContainer();
    const source = c === document.body ? 'document.body (泛用掃描)' : (c.className || c.tagName.toLowerCase());
    return { container: c, source, isSelection: false };
  }

  function measure() {
    const target = pickTarget();
    // 複製一份, 拿掉 <pre> / <script> / <style> (排除程式碼區塊)
    const clone = target.container.cloneNode(true);
    clone.querySelectorAll('pre, script, style, noscript').forEach((n) => n.remove());

    const text = clone.textContent || '';
    const htmlNoPre = clone.innerHTML || '';

    const em_B = (text.match(RE_EM_DASH) || []).length;

    const emojis = (text.match(RE_EMOJI) || []).filter((c) => !EMOJI_EXCLUDE.has(c));
    const emoji_B = emojis.length;
    const emoji_types = new Set(emojis).size;

    const all_B = clone.querySelectorAll('strong').length;
    const strict_B = (htmlNoPre.match(RE_STRICT_BOLD_LI) || []).length;
    const bq_B = clone.querySelectorAll('blockquote').length;
    const hr_B = clone.querySelectorAll('hr').length;
    const notBut_B = (text.match(RE_NOT_BUT) || []).length;
    const easiest_B = (text.match(RE_EASIEST) || []).length;
    // 選取範圍模式下不量標題 ｜ (標題通常不在選取裡)
    const pipe_B = target.isSelection ? 0 : (pickTitle().includes(PIPE) ? 1 : 0);

    const chars = text.replace(/\s/g, '').length;

    const b_total = em_B + emoji_B + strict_B + all_B + bq_B + hr_B + notBut_B + easiest_B + pipe_B;
    const emoji_n = emoji_B > 0 ? Math.min(emoji_types, EMOJI_TYPES_CAP) : 0;
    const n_sum =
      (em_B > 0 ? N.em : 0) +
      emoji_n +
      (strict_B > 0 ? N.strict : 0) +
      (all_B > 0 ? N.all : 0) +
      (bq_B > 0 ? N.bq : 0) +
      (hr_B > 0 ? N.hr : 0) +
      (notBut_B > 0 ? N.notBut : 0) +
      (easiest_B > 0 ? N.easiest : 0) +
      (pipe_B > 0 ? N.pipe : 0);
    const base = b_total * n_sum;
    const density = chars ? (base / chars) * SCALE : 0;

    return {
      rows: [
        ['em ——', em_B, em_B > 0 ? N.em : 0],
        ['emoji', emoji_B + ' (' + emoji_types + '種)', emoji_n],
        ['strict 粗體標籤', strict_B, strict_B > 0 ? N.strict : 0],
        ['all 粗體標籤', all_B, 0],
        ['blockquote', bq_B, bq_B > 0 ? N.bq : 0],
        ['hr 分隔線', hr_B, hr_B > 0 ? N.hr : 0],
        ['不是…而是', notBut_B, notBut_B > 0 ? N.notBut : 0],
        ['最容易…的', easiest_B, easiest_B > 0 ? N.easiest : 0],
        ['標題 ｜', pipe_B, pipe_B > 0 ? N.pipe : 0],
      ],
      b_total, n_sum, base, density, chars,
      source: target.source,
      isSelection: target.isSelection,
    };
  }

  function render(r) {
    document.getElementById(PANEL_ID)?.remove();

    const panel = document.createElement('div');
    panel.id = PANEL_ID;
    const dense = r.density;
    const color = dense >= CORPUS_DENSITY ? '#d9480f' : dense >= CORPUS_DENSITY / 2 ? '#e8590c' : '#2b8a3e';
    const modeTag = r.isSelection ? '<span style="font-size:10px;color:#fff;background:#5c7cfa;padding:1px 5px;border-radius:3px;margin-left:6px">選取</span>' : '';

    const rowsHtml = r.rows.map(([name, b, n]) => {
      const dim = (n === 0 && name !== 'all 粗體標籤') || b === 0 ? 'opacity:.45' : '';
      return `<tr style="${dim}"><td style="padding:1px 6px 1px 0">${name}</td>
        <td style="text-align:right;padding:1px 6px">${b}</td>
        <td style="text-align:right;padding:1px 0;color:#868e96">${n || 0}</td></tr>`;
    }).join('');

    panel.innerHTML = `
      <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:6px">
        <strong style="font-size:13px">AI 味掃描${modeTag}</strong>
        <span>
          <button id="air-rescan" title="重新掃描" style="cursor:pointer;border:0;background:#f1f3f5;border-radius:4px;padding:1px 6px;font-size:12px">↻</button>
          <button id="air-close" title="關閉" style="cursor:pointer;border:0;background:#f1f3f5;border-radius:4px;padding:1px 7px;font-size:12px">×</button>
        </span>
      </div>
      <div style="display:flex;gap:14px;margin-bottom:8px">
        <div><div style="font-size:11px;color:#868e96">N_sum</div>
          <div style="font-size:22px;font-weight:700;line-height:1.1">${r.n_sum}</div></div>
        <div><div style="font-size:11px;color:#868e96">density / 千字</div>
          <div style="font-size:22px;font-weight:700;line-height:1.1;color:${color}">${dense.toFixed(2)}</div></div>
      </div>
      <table style="font-size:12px;border-collapse:collapse;width:100%;margin-bottom:6px">
        <thead><tr style="color:#868e96;font-size:11px">
          <td style="text-align:left">訊號</td><td style="text-align:right">B</td><td style="text-align:right">N</td>
        </tr></thead>
        <tbody>${rowsHtml}</tbody>
      </table>
      <div style="font-size:11px;color:#868e96;line-height:1.5">
        B_total ${r.b_total} · base ${r.base} · 字數 ${r.chars}<br>
        來源: ${r.source}<br>
        2026 鐵人賽全體 density ≈ ${CORPUS_DENSITY}<br>
        <em>共現訊號, 不是判決. 高 ≠ 一定是 AI.</em>
      </div>`;

    Object.assign(panel.style, {
      position: 'fixed', top: '16px', right: '16px', zIndex: 2147483647,
      width: '268px', padding: '12px 14px', background: '#fff', color: '#212529',
      border: '1px solid #dee2e6', borderRadius: '10px',
      boxShadow: '0 6px 24px rgba(0,0,0,.18)',
      font: '13px/1.4 -apple-system,"Noto Sans TC",sans-serif',
    });
    document.body.appendChild(panel);
    panel.querySelector('#air-close').onclick = () => panel.remove();
    panel.querySelector('#air-rescan').onclick = () => render(measure());
  }

  // Toggle: 若面板已存在就關掉, 否則掃描
  function toggle() {
    const existing = document.getElementById(PANEL_ID);
    if (existing) {
      existing.remove();
      return;
    }
    try { render(measure()); }
    catch (e) { console.error('[AI 味掃描] 失敗', e); }
  }

  if (typeof GM_registerMenuCommand === 'function') {
    GM_registerMenuCommand('掃一下 AI 味 (toggle)', toggle);
  } else {
    console.warn('[AI 味掃描] GM_registerMenuCommand 不可用, 這個 script 需要 Tampermonkey / Greasemonkey');
  }
})();
