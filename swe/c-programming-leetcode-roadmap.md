# C Programming + 200 LeetCode Roadmap (Semester 1 → Semester 2)

Goal: learn C properly (beyond what CSE 4107 alone covers) and solve
**200 LeetCode problems by the end of Semester 2** (~mid-August 2027).
From now (~1 Nov 2026) that's about **42 weeks**.

## Part A — C Fundamentals, in order

This follows roughly the same order as CSE 4107 itself, with a few
topics the course won't get to in time for you to start LeetCode early.
Course materials for each already exist in `semester-1/structured-programming-i/`.

1. **Syntax, I/O, variables, operators** — matches Lecture 0–3
   (Getting Started → Expressions) in `slides/`.
2. **Control flow** — if/else, switch, loops — Lecture 4–5.
3. **Functions** — parameters, return values, recursion, scope.
4. **Arrays & strings** — 1D/2D arrays, `string.h`, manual string
   manipulation (K.N. King covers this in depth — see `books/`).
5. **Pointers** — pointer arithmetic, pointers to pointers, pointers to
   functions. This is where most people get stuck; don't rush it.
6. **Dynamic memory** — `malloc`/`calloc`/`realloc`/`free`, and actually
   noticing when you forget to free something.
7. **Structs & unions** — `typedef`, nested structs, arrays of structs.
8. **File I/O** — `fopen`/`fread`/`fwrite`/`fclose`.
9. **Build your own data structures in C** — linked list, stack, queue,
   binary search tree, a simple hash table. C has no built-in library for
   these (unlike C++/Python), so you *have* to build them — which is
   exactly what makes this worth doing before you touch LeetCode's
   medium-difficulty problems.

Steps 1–6 overlap directly with CSE 4107's own pace — use the course to
force the schedule. Steps 7–9 you'll likely need to push yourself, since
the course may not reach them before the semester ends.

## Part B — LeetCode Plan (200 problems, 42 weeks)

| Phase | Weeks | Focus | Pace | Cumulative |
|---|---|---|---|---|
| 1 | 1–8 | Easy only — arrays, strings, basic math/loops. Matches C fundamentals §1–4. | 3/week | 24 |
| 2 | 9–16 | Easy→medium — recursion, two pointers, basic sorting. Matches §5–6. | 4/week | 56 |
| 3 | 17–28 | Medium — linked lists, stacks/queues, hashing, basic trees. Build the structure yourself first (§9), then solve problems using it. | 5/week | 116 |
| 4 | 29–42 | Medium/hard mixed — graphs (BFS/DFS), intro DP, greedy, BSTs. | 6/week | 200 |

**Ease off during exam weeks** — don't try to hold the weekly pace
through Mid/Final exams. Redistribute instead:
- Winter Mid-exam (14–25 Dec 2026) and Winter Final (15–26 Feb 2027) fall
  in Phase 1/2 — expect near-zero problems those weeks, make it up in the
  surrounding ones.
- Summer Mid-exam (24 May–4 Jun 2027) and Summer Final (2–13 Aug 2027)
  fall in Phase 4 — same adjustment; Phase 4's target is deliberately the
  most aggressive so there's room to absorb this.

## Part C — Practical notes

- **C vs C++ for speed**: solving LeetCode in pure C is excellent for
  learning (you build everything from scratch) but slower per problem
  than C++, which gives you `vector`, `stack`, `queue`, `map` for free.
  Once you've built each core structure yourself in C at least once
  (Phase 3), it's completely reasonable to switch to C++ for Phase 4 if
  the 200-problem pace is tight — you already know the core syntax, you're
  only adding the STL on top. Don't switch before you've built the
  structures by hand at least once, or you skip the actual point of doing
  this in C first.
- **Pick problems with a list, not randomly** — LeetCode's own "Top
  Interview 150" or "Blind 75" lists are well-curated and match the topic
  order in Part A reasonably well.
- **Weekly cadence**: 3–4 solving sessions/week (~1–2 hrs each) plus one
  review session — revisit anything that took over 30 minutes or that you
  couldn't solve, and re-implement it from scratch without looking.
- This is the C-specific, problem-count-specific version of the DSA
  section (§1) in [beyond-curriculum.md](beyond-curriculum.md) — that
  file has the wider roadmap (system design, projects, grad school, etc.)
  this slots into.
