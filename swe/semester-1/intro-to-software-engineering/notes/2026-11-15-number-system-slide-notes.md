# Introduction to Software Engineering — Number System (slide-by-slide notes)

Course Code: SWE 4101   Date: 2026-11-15

## Key Points
- Two families of number systems: **non-positional** (Roman numerals — same symbol value regardless of position) and **positional** (digit value depends on position — decimal, binary, octal, hex).
- General positional-value formula: `value = digit × base^position` (position counted from 0, right to left).
- Base = how many distinct digits the system has (decimal=10, binary=2, octal=8, hex=16).
- Any base → decimal: multiply each digit by its positional value and sum.
- Decimal → any base: repeated division by the base, read remainders bottom-to-top (division-remainder method).
- Fractional part → any base: repeated multiplication by the base, read whole-number parts top-to-bottom.
- Binary ↔ Octal and Binary ↔ Hex have direct shortcuts (group bits in 3s or 4s) — no need to go through decimal.

## Details

### Slide 1 — Title
Number System, SWE 4101 (Tanjila Alam Sathi, Dept. of CSE, IUT).

### Slide 2 — Outline
Non-positional, positional, decimal, binary, octal, hexadecimal number systems.

### Slide 3 — Number System (overview)
Two types: non-positional and positional number systems.

### Slide 4 — Non-positional Number System
- Characteristic: each symbol keeps the same value no matter where it sits in the number; symbols are just added up.
- Hard to do arithmetic with.
- Example — Roman numerals: I=1, II=2, V=5, VI=6, X=10.

### Slide 5 — Positional Number System
- Characteristic: fewer symbols ("digits"); a digit's value changes depending on its position.
- Example: decimal number system.

### Slide 6 — Positional Number System (value rule)
- A digit's contribution depends on: the digit itself (face value), its position, and the base.
- Formula: `value of number = Σ (digit × base^(position−1))`, where position is counted along the number.
- Base = total number of distinct digits available; the largest single digit is always `base − 1`.

### Slide 7 — Decimal Number System
- Positional, digits 0–9.
- Example: `4321₁₀ = 1×10⁰ + 2×10¹ + 3×10² + 4×10³ = 1 + 20 + 300 + 4000`.

### Slide 8 — Binary Number System
- Positional, digits 0,1.
- Example: `1010₂ = 0×2⁰ + 1×2¹ + 0×2² + 1×2³ = 0 + 2 + 0 + 8 = 10₁₀`.

### Slide 9 — Octal Number System
- Positional, digits 0–7.
- Example: `7654₈ = 4×8⁰ + 5×8¹ + 6×8² + 7×8³ = 4 + 40 + 384 + 3584 = 4012₁₀`.

### Slide 10 — Hexadecimal Number System
- Positional, digits 0–9, A–F (A=10 … F=15).
- Example: `ABCD₁₆ = 13×16⁰ + 12×16¹ + 11×16² + 10×16³ = 13 + 192 + 2816 + 40960 = 43981₁₀`.

### Slide 11 — Converting Any Base → Decimal
1. Find the positional value of each digit.
2. Multiply each digit's decimal value by its positional value.
3. Sum the products → result.

### Slide 12 — Converting Decimal → Any Base (Division-Remainder method)
1. Divide the decimal value by the target base.
2. Record the remainder as the next digit, starting from the least-significant (rightmost) digit.
3. Repeat with the quotient until the quotient is 0.

### Slide 13 — Worked Example (Decimal → Binary)
`(25)₁₀ = (11001)₂`

### Slide 14 — Fractional Number (positional value)
For digits after the fractional point: positional value = `base^(−position)`, where position is counted starting from the fractional point.

### Slide 15 — Fractional Number (octal → decimal example)
Worked example converting a fractional octal number to decimal using the above rule.

### Slide 16 — Fractional Number: Decimal → Any Base (Multiplication method)
1. Multiply the fractional part by the base.
2. Record the non-fractional (whole-number) part that appears — this becomes the next digit after the point.
3. Repeat with the remaining fractional part until it becomes 0, or enough digits are obtained.

### Slide 17 — Worked Example
Convert `0.865` (base 10) to base 2, base 8, and base 16 using the multiplication method.

### Slide 18 — Conversion Between Two Non-Decimal Bases
1. Convert the number from the starting base to base 10.
2. Convert that base-10 value to the target base.
(i.e., always bounce through decimal as the "middle" base.)

### Slide 19 — Shortcut: Binary → Octal
Group binary digits in sets of 3 (from the binary point outward) and convert each group directly to its octal digit.

### Slide 20 — Shortcut: Octal → Binary
Reverse of slide 19 — expand each octal digit into its 3-bit binary equivalent.

### Slide 21 — Shortcut: Binary → Hexadecimal
Group binary digits in sets of 4 and convert each group directly to its hex digit.

### Slide 22 — Shortcut: Hexadecimal → Binary
Reverse of slide 21 — expand each hex digit into its 4-bit binary equivalent.

### Slide 23 — Equivalent Chart
Reference table matching decimal 0–15 (or similar range) against binary, octal, and hexadecimal equivalents.

## Summary
Number systems split into non-positional (symbol value fixed, e.g. Roman numerals) and positional (value depends on digit × position × base). Decimal, binary, octal, and hex are all positional, differing only in base and digit set. Conversion always reduces to two skills: any-base→decimal (multiply-and-sum) and decimal→any-base (divide-and-remainder for whole numbers, multiply-and-collect for fractions); binary/octal/hex have extra direct shortcuts via 3-bit/4-bit grouping since 8=2³ and 16=2⁴.

## References
- [Number System slides](../slides/Lecture-3-Number-System.pdf) (23 slides, reference material — Tanjila Alam Sathi, not this semester's own Shohel Ahmed sir lecture)
