---
name: condense-mindmap
description: Condense a large amount of source material into a compact mind map written as a nested Markdown list. Use when the user provides many notes, a long transcript, meeting minutes, or several documents and wants the key structure pulled out as a scannable map rather than a prose summary. This is a convergent, faithfulness-first task: every node must trace back to the source; do not invent branches to make the map look tidy.
---

# Condense Mind Map

Turn a pile of source material into one nested-bullet mind map that a reader can scan in seconds. This is summarization + synthesis in mind-map shape: information flows from many to few, and faithfulness matters more than completeness.

## Output contract

Always output a single fenced or plain Markdown block shaped like this:

```
# Root topic (one line, the single center)
- Branch A
  - leaf
  - leaf
- Branch B
  - leaf
  - Sub-branch
    - leaf
- Branch C
```

Hard rules:

1. **One root.** The first line is the only center, stated in one short line.
2. **Depth: 3 levels by default, 4 at most.** Deeper than that is an outline, not a mind map.
3. **Breadth: <= 7 items per level, 3 is the target.** If a level has more than 7, group the similar ones under a new parent or drop the weakest. Never leave a dozen siblings on one level.
4. **Nodes are phrases, not sentences.** A node must be scannable at a glance. If a full sentence is creeping in, cut it to its core noun phrase.
5. **Output only the Markdown list.** No preamble, no closing summary, no "here is your mind map", and no rendering instructions (do not tell the user where to view it).

## Faithfulness rule (the one that matters most)

This is a convergent task. The danger is quietly adding nodes that were not in the source to make the structure look complete.

- **Every node must trace back to the source material.** If you cannot point to where a node came from, do not include it.
- Do not add "obvious" branches from your own knowledge. If the source never mentioned it, it does not belong on this map.
- If the source is thin in some area, let that branch be thin. An honest lopsided map beats a fabricated balanced one.
- When you are unsure whether something was in the source, leave it out.

## Process

1. **Extract.** Pull the concrete facts, claims, numbers, and decisions out of the material. Ignore filler.
2. **Group.** Cluster the extracted items by similarity; find the parent concept each cluster shares. This is where synthesis happens - a good branch name says something none of its leaves say alone.
3. **Name branches.** Give each branch a short phrase that genuinely covers everything beneath it.
4. **Prune to the caps.** Apply the depth (<=4) and breadth (<=7, ideally 3) limits. Regroup or cut until every level fits.

## Large input: map-reduce

When the source material is too large to fit in the context window at once, do not truncate silently. Use map-reduce:

1. **Map:** split the material into chunks and condense each chunk into its own small mind map.
2. **Reduce:** merge those small maps, then condense the merged result once more into the final map, still obeying every rule above.

Tell the user briefly that you used map-reduce and how many chunks, so they know some cross-chunk connections may be weaker.

## Example

Input (excerpt of meeting notes):

> Q3 revenue was NT$4.2M, up 12% QoQ. Marketing overspent by 8%. The new onboarding flow cut churn from 5.1% to 3.8%. Support ticket volume doubled after the launch; two engineers reassigned to fix it. Board wants a hiring freeze except for support.

Output:

```
# Q3 review: growth up, support strained
- Numbers
  - Revenue NT$4.2M (+12% QoQ)
  - Churn 5.1% -> 3.8%
  - Marketing 8% over budget
- New onboarding
  - Cut churn
  - Doubled support tickets
- Response
  - 2 engineers -> support
  - Hiring freeze except support
```

Note how every node traces to the notes, phrases stay short, and the level stays under 7.

## What not to do

- Do not write a paragraph summary and call it a mind map.
- Do not add a "Recommendations" or "Next steps" branch unless the source actually contained one.
- Do not exceed 4 levels of depth or 7 items on a level.
- Do not append instructions about markmap, Obsidian, or any renderer.
