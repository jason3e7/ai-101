---
name: expand-mindmap
description: Expand a small seed - a topic, a question, or a single sentence - into a researched mind map written as a nested Markdown list. Use when the user gives you very little and wants it opened up into structure they can explore, not a prose essay. This is a divergent task: first generate non-overlapping angles, then research each, then structure the findings. You may generate freely, but every node must be labeled as either researched or speculative, and researched nodes must actually be backed by what you found.
---

# Expand Mind Map

Take a tiny seed and grow it into a mind map worth exploring. This is ideation + research in mind-map shape: information flows from few to many. You are allowed - expected - to generate, but you must be honest about what is grounded and what is a guess.

## Output contract

Always output a single Markdown block shaped like this:

```
# Seed topic (one line, the single center)
- Angle A
  - point
  - point (?)
- Angle B
  - point
  - Sub-angle
    - point
- Angle C
```

Hard rules (kept identical to the condense-mindmap skill on purpose, so both maps read the same):

1. **One root.** The first line is the only center, stated in one short line.
2. **Depth: 3 levels by default, 4 at most.** Deeper than that is an outline, not a mind map.
3. **Breadth: <= 7 items per level, 3 is the target.** If a level has more than 7, group the similar ones under a new parent or drop the weakest.
4. **Nodes are phrases, not sentences.** A node must be scannable at a glance.
5. **Output only the Markdown list.** No preamble, no closing summary, and no rendering instructions (do not tell the user where to view it).

## Mark what is real (the rule that matters most here)

This is a divergent task, so invention is fine - but the reader must be able to tell a fact from a guess.

- **Append ` (?)` to any node that is speculative, inferred, or not confirmed by research.** Leave researched nodes unmarked.
- A node left unmarked is a promise that you actually found support for it. Do not leave a guess unmarked.
- Do not fabricate specifics (numbers, names, dates, citations). If you want to point at a specific claim you did not verify, phrase it as a direction and mark it ` (?)`.
- Being lopsided is fine: some angles will be well-researched, others mostly speculative. Show that honestly rather than faking balance.

## Process

1. **Diverge first.** From the seed, generate the top-level angles - and force them to not overlap. Aim for genuinely different directions (e.g. technical / human / economic / historical / contrarian), not three flavors of the same idea. Do this before researching, so research does not narrow you too early.
2. **Research each angle.** Use whatever tools and methods are available (web search, docs, your own knowledge). Ground each branch in what you find. There is no restriction on how you research.
3. **Structure the findings.** Fold the research into each branch as leaves. This step is the same shaping work as condensing: apply the caps and phrasing rules.
4. **Label reality.** Go through every node and add ` (?)` to anything you did not actually confirm.

## Guard against narrowing

LLM ideation tends to collapse toward one core idea (in one study, 94% of generated ideas shared the same concept). Fight it:

- Decide the top-level angles **before** you start researching, and make each one structurally different.
- If two angles start converging on the same content, merge them and open a new, more distant angle.
- Prefer a surprising-but-relevant angle over a safe, obvious one.

## Example

Seed: "why do people procrastinate"

Output:

```
# Why people procrastinate
- Emotion regulation
  - Avoiding a negative feeling, not the task
  - Task feels threatening to self-worth (?)
- Brain / time
  - Present bias: now outweighs later
  - Weak future-self connection (?)
- Task design
  - Goal too vague to start
  - No clear first action
- Environment
  - Frictionless distractions nearby
  - Deadlines too distant to bite (?)
```

Note the four non-overlapping angles, the ` (?)` on unconfirmed claims, short phrases, and the level staying under 7.

## What not to do

- Do not write an essay and call it a mind map.
- Do not leave speculative nodes unmarked - an unmarked node claims you verified it.
- Do not invent fake numbers, citations, or names to look authoritative.
- Do not let all top-level angles collapse into the same idea.
- Do not exceed 4 levels of depth or 7 items on a level.
- Do not append instructions about markmap, Obsidian, or any renderer.
