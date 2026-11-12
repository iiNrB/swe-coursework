# Structured Programming I — Formatted I/O (printf & scanf)

Course Code: CSE 4107   Date: 2026-11-12
Source: Shahriar Ivan's "03 Formatted I/O" lecture (21 slides)

## Key Points

- `printf`/`scanf` format strings mix **ordinary characters** (printed/matched
  literally) with **conversion specifications** (`%` + letter), which are
  placeholders for values.
- The compiler **never checks** that the number/type of conversion
  specifications matches the number/type of arguments — mismatches compile
  fine and produce garbage output or skip data silently.
- A format specifier can take the full form `%m.pX` (or `%-m.pX` for
  left-justify): `m` = minimum field width, `p` = precision, `X` = the type
  letter (`d`, `f`, `e`, …). Both `m` and `p` are optional.
- **`p`'s meaning changes depending on `X`** — this is the single most
  important thing to remember from this lecture.

## Details

### `printf` — every worked example from the slides

| Code | Output | Why |
|---|---|---|
| `printf("%d", 123);` | `123` | no width/precision, plain decimal |
| `printf("%4d", 123);` | `␣123` | width=4; value needs 3 chars → 1 leading space, right-justified (default) |
| `printf("%4d", 123456);` | `123456` | value needs 6 chars, more than width=4 → width auto-**expands**, nothing is ever truncated |
| `printf("%-4d", 123);` | `123␣` | `-` before `m` → left-justify instead of the default right-justify |
| `printf("%5.3d", 40);` | `␣␣040` | for `%d`, precision = **minimum digit count** (zero-padded): 40 → `040` (3 digits), then right-justified in width 5 |
| `printf("%10.3f", 839.21);` | `␣␣␣839.210` | for `%f`, precision = digits **after the decimal point**; width 10 total |
| `printf("%10.3e", 839.21);` | `␣8.392e+02` | for `%e`, precision = decimal digits in the exponential mantissa (839.21 → 8.392×10²) |
| `printf("%.0e", 839.21);` | `8e+02` | precision 0 on `%e`/`%f` → **no decimal point printed at all** |

**`p`'s meaning by specifier** (the key table):

| Specifier `X` | What `p` means |
|---|---|
| `e` (exponential) | digits after the decimal point in the mantissa (default 6); `p=0` → no decimal point |
| `f` (fixed decimal) | same meaning as `e` — digits after the decimal point |
| `d` (integer) | **minimum** number of digits — extra leading zeros added if needed; default (if `p` omitted) is 1 |

**Common printf mistakes (all compile, none are caught by the compiler):**
```c
printf("%d %d\n", i);        // too few args — 2nd %d prints garbage memory
printf("%d\n", i, j);        // too many args — j is silently ignored
printf("%f %d\n", i, x);     // wrong type order — meaningless output
printf("%d %d\n", &i, &j);   // NEVER use & in printf — only scanf needs it
```

### Escape sequences (used inside format strings)

| Sequence | Meaning |
|---|---|
| `\a` | Alert / bell |
| `\n` | New line |
| `\t` | Tab |
| `\\` | Literal backslash |
| `\"` | Literal double quote |

Example: `printf("Item\tUnit\tPurchase\n\tPrice\tDate\n");`

### `scanf` — how it actually reads input

- `scanf` walks the format string left to right, matching each conversion
  specification against the input.
- For each conversion spec, it **skips leading whitespace**, then reads
  characters belonging to that item's type, stopping at the first character
  that can't belong (that character is "put back" for the next read).
- **Reading an int:** looks for an optional `+`/`-` then digits; stops at
  the first non-digit.
- **Reading a float:** optional sign, digits (optionally with a decimal
  point), optional exponent (`e`/`E` with optional sign and digits).
- Whitespace **in the format string** (e.g. the space in `"%d %d"`) tells
  `scanf` to skip any amount of whitespace in the input — including
  newlines. That's why input can be split across multiple lines:
  ```c
  int i, j; float x, y;
  scanf("%d %d %f %f", &i, &j, &x, &y);
  /* valid input, spread over 3 lines: */
  // 1
  // -20   .3
  //  -4.0e3
  ```
- **Ordinary (non-whitespace) characters in the format string must match
  the input exactly**, or `scanf` puts the offending character back and
  aborts immediately:
  - Format `"%d/%d"`, input `5/96` → **succeeds** (`/` matches `/`)
  - Format `"%d/%d"`, input `5 /96` (space before `/`) → **fails** (the `/`
    in the format string doesn't match the space in the input)

**Common scanf mistakes:**
```c
scanf("%d, %d", &i, &j);   // works, but fragile: input MUST contain a literal
                            // comma+space exactly, or it fails — avoid ordinary
                            // characters in scanf format strings unless needed
scanf("%d\n", &i);          // a trailing \n in the format string is a trap:
                             // scanf then skips whitespace looking for the
                             // NEXT non-whitespace character, which can make
                             // an interactive program hang waiting for more input
```

## Summary

`printf`/`scanf` format strings are placeholder-based, and the compiler does
none of the safety checking — matching the count, type, and literal
characters correctly is entirely the programmer's job. The `%m.pX` syntax
covers width and precision, but precision means something different for
`d` (min digits) than for `e`/`f` (decimal digits) — that's the one rule
most worth memorizing. For `scanf`, remember it skips whitespace freely but
matches ordinary characters exactly, and never put a trailing `\n` in a
scanf format string.

## References
- Lecture: `slides/03-Formatted-IO.pdf` (Shahriar Ivan, CSE 4107)
- Textbook: K.N. King, *C Programming: A Modern Approach* — Formatted I/O chapter covers the same `%m.pX` syntax in more depth
