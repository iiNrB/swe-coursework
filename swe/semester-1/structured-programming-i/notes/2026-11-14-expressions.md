# Structured Programming I — Expressions (full lecture, all 44 slides)

Course Code: CSE 4107   Date: 2026-11-14
Source: Shahriar Ivan's "04 Expressions" lecture (44 slides)

## Key Points

- An **expression** is a formula that computes a value, built from variables,
  constants, and operators.
- Five operator families covered: **arithmetic**, **relational**, **logical**,
  **assignment**, **increment/decrement**.
- **Precedence** decides which operator runs first when none are in
  parentheses; **associativity** decides the tie-break when two operators of
  the *same* precedence sit side by side.
- A huge share of real C bugs trace back to one of three things: **integer
  division truncation**, **prefix vs postfix `++`/`--`**, or **using an
  assignment/increment operator more than once on the same variable inside
  one expression** (undefined behavior).

## Details

### 1. Operators — overview (slide 2)

C's operator families: arithmetic (`+ - * / %`), relational (`< > <= >= ==
!=`), logical (`&& || !`), assignment (`=` and compound forms), and
increment/decrement (`++ --`).

### 2. Arithmetic Operators (slides 3–11)

- **Unary** (one operand): `+` (unary plus, purely cosmetic — emphasizes a
  positive constant), `-` (unary minus, negates).
- **Binary** (two operands): `+ - * /` and `%` (remainder).
  - `10 % 3` → `1`; `12 % 4` → `0`.
  - `%` requires **both** operands to be `int` — won't compile otherwise.
- Mixing `int` and `float` in `+ - * /` always produces a `float` result:
  `9 + 2.5f` → `11.5`; `6.7f / 2` → `3.35`.
- **`/` truncates when both operands are `int`**: `1 / 2` is `0`, not `0.5`.
- Dividing by zero (`/` or `%`) is **undefined behavior** — never do it.
- Negative operands with `/`/`%` were *implementation-defined* in C89
  (different compilers could legally disagree); C99 fixed this: division
  truncates toward zero, and `i % j` always has the same sign as `i`.
- **Implementation-defined behavior** exists because C favors matching raw
  hardware behavior for speed — but code that depends on it is non-portable
  and should be avoided.
- **Operator precedence**: `i + j * k` means `i + (j * k)` — multiplication
  binds tighter than addition. When unsure, use parentheses or a table.
- **Precedence table so far** — highest to lowest: unary `+`/`-`, then
  `* / %`, then binary `+`/`-`.
- **Associativity**: when two operators of equal precedence sit together,
  this decides grouping direction.
  - The binary arithmetic operators (`* / % + -`) are **left associative**:
    `i - j - k` ≡ `(i - j) - k`.
  - The unary operators (`+`/`-`) are **right associative**:
    `- + i` ≡ `-(+i)` (the operator closest to the operand runs first).
  - When in doubt: use a table, or just add parentheses.

### 3. Mini-project: Computing a UPC Check Digit (slides 12–15)

A practical use of everything above. A UPC barcode has a single digit, two
groups of five digits, and a check digit. Given the first 11 digits, the
check digit is computed as:

1. Add the 1st, 3rd, 5th, 7th, 9th, 11th digits → `first sum`
2. Add the 2nd, 4th, 6th, 8th, 10th digits → `second sum`
3. `first sum × 3 + second sum`
4. Subtract 1
5. Take the remainder when divided by 10
6. `9 − remainder` → the check digit

Worked example for `0 13800 15173 5`: first sum = 8, second sum = 21,
`8×3+21 = 45`, `45−1 = 44`, `44 % 10 = 4`, `9−4 = 5` ✓ (matches the real
check digit).

The program reads the 11 digits one at a time using `scanf` with `%1d`
(a width of 1 forces it to read exactly one digit at a time, even from a
5-digit group typed together).

### 4. Assignment Operator (slides 16–27)

**Simple assignment (`v = e`)** — slides 17–19, 23:

- Evaluates `e` and copies its value into `v`.
- If `v` and `e` have different types, `e` is **converted to `v`'s type**
  during the assignment: `i = 72.99f;` → `i` becomes `72` (truncated, not
  rounded); `f = 136;` → `f` becomes `136.0`.
- Unlike many languages, **assignment is an operator in C**, not just a
  statement — it has a value: the value of `v = e` is `v`'s value *after*
  the assignment. So `i = 72.99f` itself evaluates to `72`.
- **lvalue**: the left side of `=` must be an *lvalue* — something that
  represents an actual memory location (a variable), not a constant or a
  computed result. `12 = i;`, `i + j = 0;`, `-i = j;` are all illegal
  ("invalid lvalue in assignment").

**Side effects and chained assignment** (slides 20–22):

- An operator that modifies one of its operands has a **side effect**.
  `=` has one: it modifies its left operand.
- Because `=` is an operator with a value, assignments chain:
  `i = j = k = 0;` — and since `=` is **right associative**, this is
  `i = (j = (k = 0));`.
- Chained assignments across different types can surprise you:
  `f = i = 33.3f;` → `i` becomes `33` first, *then* `f` is assigned `33`
  (from `i`, as an `int`), so `f` ends up `33.0`, not `33.3`.
- "Embedded assignments" (an assignment tucked inside a larger expression)
  are legal — `k = 1 + (j = i);` prints `i j k` as `1 1 2` — but they hurt
  readability and are a common source of subtle bugs.

**Compound assignment (`v op= e`)** (slides 24–27):

| Operator | Meaning |
|---|---|
| `v += e` | `v = v + e` |
| `v -= e` | `v = v - e` |
| `v *= e` | `v = v * e` |
| `v /= e` | `v = v / e` |
| `v %= e` | `v = v % e` |

- `v += e` is **not exactly equivalent** to `v = v + e`:
  1. **Precedence**: `i *= j + k` means `i = i * (j + k)`, not `i = i * j + k`.
  2. **Rare side-effect cases**: if `v` itself has a side effect, `+=`
     evaluates it once while `v = v + e` evaluates it twice.
- **Don't transpose the two characters**: `i =+ j;` compiles fine but means
  `i = (+j)` (just copies `j` into `i`) — not `i += j`.

### 5. Increment and Decrement Operators (slides 28–34)

- `++` adds 1, `--` subtracts 1 — shorthand for the already-seen patterns:
  `i = i + 1;` ≡ `i += 1;` ≡ `++i;` / `i++;`
- Both come in **prefix** (`++i`, `--i`) and **postfix** (`i++`, `i--`)
  forms, and both have side effects (they modify their operand).
- **The one rule that matters most:**
  - `++i` (prefix) — increments *immediately*, then the expression's value
    is the *new* value.
  - `i++` (postfix) — the expression's value is the *old* value, and `i`
    is incremented afterward (guaranteed to happen before the next
    statement, though C doesn't say exactly when).

  ```c
  i = 1;
  printf("%d\n", ++i);  // "2" — i becomes 2, then 2 is used
  printf("%d\n", i);    // "2"

  i = 1;
  printf("%d\n", i++);  // "1" — old value 1 is used, then i becomes 2
  printf("%d\n", i);    // "2"
  ```
  `--i` / `i--` behave the same way, just subtracting.

- **Using `++`/`--` more than once on the same variable in one expression
  is risky and hard to read:**
  ```c
  i = 1; j = 2;
  k = ++i + j++;   // i becomes 2 first, j's old value (2) is used, then j becomes 3
                    // → i=2, j=3, k = i+j = 2+2 = 4
  i = 1; j = 2;
  k = i++ + j++;   // both postfix: both old values are used
                    // → i=2, j=3, but k = 1+2 = 3
  ```

### 6. Expression Evaluation (slides 35–42)

**Updated precedence table** (highest → lowest), now including `++`/`--`
and assignment:

| Precedence | Operator | Associativity |
|---|---|---|
| 1 (highest) | postfix `++` `--` | left |
| 2 | prefix `++` `--`, unary `+` `-` | right |
| 3 | `* / %` | left |
| 4 | binary `+` `-` | left |
| 5 (lowest) | `=` and all compound assignment | right |

**Using the table to add parentheses to a bare expression** — go from
highest precedence to lowest, wrapping each operator with its operands:

```
a = b += c++ - d + --e / -f

level 1 (postfix ++):      a = b += (c++) - d + --e / -f
level 2 (prefix --, unary -): a = b += (c++) - d + (--e) / (-f)
level 3 (/):                a = b += (c++) - d + ((--e) / (-f))
level 4 (+, -):              a = b += (((c++) - d) + ((--e) / (-f)))
level 5 (=, +=):              (a = (b += (((c++) - d) + ((--e) / (-f)))))
```

**Order of subexpression evaluation is mostly unspecified** (slides 37–42):

- C does **not** define the order subexpressions are evaluated in, except
  around `&&`, `||`, `?:`, and the comma operator.
  In `(a + b) * (c - d)`, you can't assume `(a + b)` runs before `(c - d)`.
- This is harmless for most expressions — but **becomes undefined behavior**
  when a subexpression modifies a variable that another part of the *same*
  expression also reads or modifies:
  ```c
  a = 5;
  c = (b = a + 2) - (a = 1);   // undefined behavior — a is both read and
                                 // reassigned within the same expression
  ```
  The same danger applies to `++`/`--`:
  ```c
  i = 2;
  j = i * i++;   // could legally be 4 OR 6, depending on evaluation order
  ```
- **The fix: never read and modify the same variable within one expression.**
  Split it into separate statements instead:
  ```c
  a = 5;
  b = a + 2;
  a = 1;
  c = b - a;   // always 6 — no ambiguity
  ```
- Undefined behavior in general means: the program might compile and run
  differently per compiler, might fail to compile, might crash, or might
  silently produce garbage. Always avoid it.

### 7. Expression Statements (slides 43–44)

- Any expression followed by `;` becomes a statement — but it's only useful
  if it has a side effect:
  ```c
  i = 1;    // useful (assignment side effect)
  i--;      // useful (decrement side effect)
  i * j - 1; // useless — computes a value, throws it away, does nothing
  ```
- A simple typo can create one of these "do-nothing" statements by accident
  — typing `i + j;` when you meant `i = j;` compiles fine but does nothing.
  Some compilers warn: *"statement with no effect."*

## Summary

Arithmetic in C hides two classic traps: `/` truncates silently with two
`int` operands, and `%`/`/` with zero or negative operands needs care.
Assignment is a real operator with a value and a side effect, which lets it
chain and embed — powerful, but readability and subtle type-conversion bugs
are the cost. `++`/`--` prefix vs postfix is the single most testable
distinction in this whole lecture. And the deepest trap of all: C does not
guarantee the order subexpressions evaluate in, so any expression that both
reads and modifies the same variable (via `=`, `+=`, `++`, `--`) more than
once is undefined behavior — always split such code into separate, ordered
statements instead.

## References
- Lecture: `slides/04-Expressions.pdf` (Shahriar Ivan, CSE 4107)
- Handout: `slides/handouts/04-handout-part1.pdf` / `04-handout-part2.pdf`
