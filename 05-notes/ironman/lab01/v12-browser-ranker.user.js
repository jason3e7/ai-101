// ==UserScript==
// @name         lab01 V12 AI 味即時掃描 — N_sum & density
// @namespace    https://github.com/jason3e7/ai-101
// @version      1.0.0
// @description  在瀏覽網頁時, 用 lab01 V12 的 9 個訊號即時算出 N_sum 與 density (每千字), 浮出一個小面板. iThome 文章頁自動掃, 其他網頁從 Tampermonkey 選單「V12 掃描這頁」手動觸發. 這是共現訊號, 不是判決.
// @author       jason3e7 + Claude
// @match        *://*/*
// @grant        GM_registerMenuCommand
// @run-at       document-idle
// ==/UserScript==

/*
 * 跟 analyzeV12.py 對齊的 9 個訊號與權重 (2026-10-02 套用後):
 *
 *   訊號            B (數量)                                   N (權重, Bᵢ>0 才計)
 *   em     ——       innerText 裡 "——" 出現次數                 4
 *   emoji           emoji 字元數 (扣掉 ○✗★☆☐)                  min(distinct_types, 5)
 *   strict          <li> 開頭的 <strong>標籤</strong>：          3
 *   all             <strong> 總數                               0  (只進 B_total, 當放大器)
 *   bq              <blockquote> 數                             1
 *   hr              <hr> 數                                     1.5
 *   v08c   不是…而是  不(只)?是…(而是|更是)                       3
 *   v09    最容易…的  最容易[中文]{1,5}的                         3
 *   v11    標題 ｜    標題含全形 ｜                              2  (binary)
 *
 *   B_total = ΣBᵢ
 *   N_sum   = Σ Nᵢ (只加 Bᵢ>0 的)
 *   base    = B_total × N_sum
 *   density = base / 去空白字數 × 1000   ← 「每千字」
 *
 * 全屆 2026 鐵人賽 15,057 篇的 V12 全體 density ≈ 69.7, 可當對照基準.
 * <pre> 程式碼區塊整段排除 (跟 analyzer 一樣), 標題不算進字數 (只有 v11 看標題).
 */

(function () {
  'use strict';

  const SCALE = 1000;
  const EMOJI_EXCLUDE = new Set(['○', '✗', '★', '☆', '☐']);
  const CORPUS_DENSITY = 69.7; // V12 全屆平均, 供對照

  // --- 跟 analyzeV12.py 相同的 pattern ---
  const RE_EM_DASH = /——/g;
  const RE_EMOJI = /[\u{1F300}-\u{1F5FF}\u{1F600}-\u{1F64F}\u{1F680}-\u{1F6FF}\u{1F700}-\u{1F8FF}\u{1F900}-\u{1F9FF}\u{1FA00}-\u{1FAFF}\u{2600}-\u{26FF}\u{2700}-\u{27BF}\u{1F1E6}-\u{1F1FF}]/gu;
  const RE_STRICT_BOLD_LI = /<li\b[^>]*>\s*(?:<p\b[^>]*>\s*)?<strong\b[^>]*>([^<>]+)<\/strong>[：:\s]*[^\s<]/gi;
  const RE_V08C = /不(?:只)?是[^。！？\n]{1,25}(?:而是|更是)/g;
  const RE_V09 = /最容易[一-鿿]{1,5}的/g;
  const PIPE = '｜'; // U+FF5C

  // N 權重 (Bᵢ>0 才計). emoji 另算 min(types,5).
  const N = { em: 4, strict: 3, all: 0, bq: 1, hr: 1.5, v08c: 3, v09: 3, v11: 2 };
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

  function measure() {
    const container = pickContainer();
    // 複製一份, 拿掉 <pre> / <script> / <style> (跟 analyzer 一樣排除程式碼區塊)
    const clone = container.cloneNode(true);
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
    const v08c_B = (text.match(RE_V08C) || []).length;
    const v09_B = (text.match(RE_V09) || []).length;
    const v11_B = pickTitle().includes(PIPE) ? 1 : 0;

    const chars = text.replace(/\s/g, '').length;

    const b_total = em_B + emoji_B + strict_B + all_B + bq_B + hr_B + v08c_B + v09_B + v11_B;
    const emoji_n = emoji_B > 0 ? Math.min(emoji_types, EMOJI_TYPES_CAP) : 0;
    const n_sum =
      (em_B > 0 ? N.em : 0) +
      emoji_n +
      (strict_B > 0 ? N.strict : 0) +
      (all_B > 0 ? N.all : 0) +
      (bq_B > 0 ? N.bq : 0) +
      (hr_B > 0 ? N.hr : 0) +
      (v08c_B > 0 ? N.v08c : 0) +
      (v09_B > 0 ? N.v09 : 0) +
      (v11_B > 0 ? N.v11 : 0);
    const base = b_total * n_sum;
    const density = chars ? (base / chars) * SCALE : 0;

    return {
      rows: [
        ['em ——', em_B, em_B > 0 ? N.em : 0],
        ['emoji', emoji_B + ' (' + emoji_types + '種)', emoji_n],
        ['strict 粗體標籤', strict_B, strict_B > 0 ? N.strict : 0],
        ['all <strong>', all_B, 0],
        ['blockquote', bq_B, bq_B > 0 ? N.bq : 0],
        ['hr 分隔線', hr_B, hr_B > 0 ? N.hr : 0],
        ['v08c 不是…而是', v08c_B, v08c_B > 0 ? N.v08c : 0],
        ['v09 最容易…的', v09_B, v09_B > 0 ? N.v09 : 0],
        ['v11 標題 ｜', v11_B, v11_B > 0 ? N.v11 : 0],
      ],
      b_total, n_sum, base, density, chars,
      container: container === document.body ? 'document.body (泛用掃描)' : (container.className || container.tagName.toLowerCase()),
    };
  }

  function render(r) {
    document.getElementById('v12-ranker-panel')?.remove();

    const panel = document.createElement('div');
    panel.id = 'v12-ranker-panel';
    const dense = r.density;
    const color = dense >= CORPUS_DENSITY ? '#d9480f' : dense >= CORPUS_DENSITY / 2 ? '#e8590c' : '#2b8a3e';

    const rowsHtml = r.rows.map(([name, b, n]) => {
      const dim = (n === 0 && name !== 'all <strong>') || b === 0 ? 'opacity:.45' : '';
      return `<tr style="${dim}"><td style="padding:1px 6px 1px 0">${name}</td>
        <td style="text-align:right;padding:1px 6px">${b}</td>
        <td style="text-align:right;padding:1px 0;color:#868e96">${n || 0}</td></tr>`;
    }).join('');

    panel.innerHTML = `
      <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:6px">
        <strong style="font-size:13px">lab01 V12 掃描</strong>
        <span>
          <button id="v12-rescan" title="重新掃描" style="cursor:pointer;border:0;background:#f1f3f5;border-radius:4px;padding:1px 6px;font-size:12px">↻</button>
          <button id="v12-close" title="關閉" style="cursor:pointer;border:0;background:#f1f3f5;border-radius:4px;padding:1px 7px;font-size:12px">×</button>
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
        來源: ${r.container}<br>
        全屆 V12 平均 density ≈ ${CORPUS_DENSITY}<br>
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
    panel.querySelector('#v12-close').onclick = () => panel.remove();
    panel.querySelector('#v12-rescan').onclick = () => render(measure());
  }

  function scan() {
    try { render(measure()); }
    catch (e) { console.error('[V12] 掃描失敗', e); alert('V12 掃描失敗: ' + e.message); }
  }

  if (typeof GM_registerMenuCommand === 'function') {
    GM_registerMenuCommand('V12 掃描這頁', scan);
  }

  // iThome 文章頁自動掃一次
  if (/(^|\.)ithelp\.ithome\.com\.tw$/.test(location.host) && location.pathname.includes('/articles/')) {
    scan();
  }
})();
