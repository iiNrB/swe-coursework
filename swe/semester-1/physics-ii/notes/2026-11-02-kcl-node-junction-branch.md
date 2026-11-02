# Physics II — Node, Junction, Branch & KCL Practice Problems

Course Code: Phy 4143   Date: 2026-11-02

## Key Points
- **Node** = point where ≥2 circuit elements join.
- **Junction** = point where ≥3 circuit elements join (current divides here — this is where KCL applies).
- **Branch** = the part of the network between two junction points.
- **Loop** = a closed path; **Mesh** = smallest loop that can't be divided further.
- ⭐ **Exam-important: Practice Problem 2.7 (Fig. 2.26, Lecture 1 slide 9)** — single-node KCL with a dependent current source. This pattern (independent source + resistor + dependent source, all sharing one node) is a classic KCL exam question.

## Details

### Definitions (Lecture 1, slide 7 — circuit ABCD with E1, R1, R2, R3, E2)
- Node points: A, B, C, D (any point where ≥2 elements meet)
- Junctions: B, D (≥3 elements meet — current splits)
- Branches: DAB, BCD, BD
- Loops: ABDA, BCDB

### Practice Problem 2.7 (Fig. 2.26) — full solution
Circuit: 6A current source, 2Ω resistor, dependent current source $i_o/4$, and 8Ω resistor — all in parallel between node $a$ and ground.

1. 2Ω and 8Ω share the same two nodes ⇒ same voltage $v_o$ across both.
2. Ohm's law on the 2Ω branch: $i_o = v_o/2$, i.e. $v_o = 2i_o$.
3. KCL at node $a$ (current in = current out):
   $$6 = i_o + \frac{i_o}{4} + \frac{v_o}{8}$$
4. Substitute $v_o = 2i_o$ so $v_o/8 = i_o/4$:
   $$6 = i_o + \frac{i_o}{4} + \frac{i_o}{4} = 1.5\,i_o$$
5. Solve:
   $$i_o = \frac{6}{1.5} = 4\text{ A}, \qquad v_o = 2 \times 4 = 8\text{ V}$$

**Answer: $i_o = 4$ A, $v_o = 8$ V** (matches the slide's given answer).

### Other solved problems from Lecture 1, slides 8–9 (for reference)
| Problem | Answer |
|---|---|
| Fig 2.21 (20V, 2Ω, 3Ω series) | $v_1=8$V, $v_2=12$V |
| Fig 2.22 (10V & 8V aiding, 4Ω, 2Ω) | $v_1=12$V, $v_2=-6$V |
| Example 2.7 (Fig 2.25, dependent + 3A source) | $i_o=6$A, $v_o=24$V |
| Example 2.8 (Fig 2.27a, 30V–8Ω–(3Ω∥6Ω)) | $i_1=3$A, $i_2=2$A, $i_3=1$A |

## Summary
- Node ⊂ broader term; Junction = node with ≥3 branches (where KCL is meaningful).
- KCL recipe for a single-node circuit: write "sum in = sum out," express every branch current via Ohm's law in terms of the one unknown node voltage, then solve.
- Dependent sources (like $i_o/4$) just get substituted in terms of the same unknown — don't treat them as a separate unknown.

## References
- [Lecture 1 slides](../slides/Lecture-1.pdf) (pages 7–9)
