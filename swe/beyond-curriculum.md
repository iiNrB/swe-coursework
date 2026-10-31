# Beyond the Curriculum — Roadmap for Industry, Grad School & Research

IUT's SWE curriculum covers the fundamentals, but it won't by itself get
you Silicon-Valley-ready, into a strong grad program, or research-capable.
This is what to add on top, roughly in the order it starts paying off.
Not a checklist to rush — spread over the full 4 years.

## 1. Data Structures & Algorithms (start now, never stop)

The single highest-leverage thing for both industry interviews and CS
maturity. Your "Structured Programming I" course barely scratches this.

- Learn: arrays/strings, linked lists, stacks/queues, trees (BST, heaps),
  graphs (BFS/DFS, shortest path, MST, topological sort), hashing,
  recursion & backtracking, dynamic programming, greedy algorithms.
- Practice: [LeetCode](https://leetcode.com) (aim for 300-500 problems by
  graduation, mixed difficulty), or competitive programming via
  [Codeforces](https://codeforces.com) if you enjoy contests.
- Competitive programming (Codeforces/ICPC) is a strong path in
  Bangladesh specifically — IUT fields ICPC teams; joining one sharpens
  DSA far faster than solo practice and looks great on a resume/SOP.
- Book: *Introduction to Algorithms* (CLRS) for rigor, *Elements of
  Programming Interviews* or *Cracking the Coding Interview* for
  interview-style practice.

## 2. CS Fundamentals your curriculum under-covers

- **Operating Systems**: processes/threads, scheduling, memory
  management, concurrency, file systems. (Book: *Operating Systems:
  Three Easy Pieces* — free online.)
- **Computer Networks**: TCP/IP, HTTP, DNS, how the web actually works.
- **Databases**: relational modeling, SQL (seriously, beyond "SELECT *"),
  indexing, transactions; then one NoSQL store (MongoDB or Redis) to see
  the trade-offs.
- **Discrete Math & Linear Algebra**: needed for algorithms, ML, and any
  grad-level CS theory course — go beyond what Math 4141 alone gives you.

## 3. Software Engineering practice (the part courses never teach)

- **Git** beyond `add`/`commit`/`push`: branching strategies, rebasing,
  resolving conflicts, PR review etiquette.
- **Testing**: unit tests, integration tests, TDD basics — pick one
  language and actually practice writing tests, not just code.
- **Clean code & design patterns**: *Clean Code* (Robert Martin), Gang of
  Four patterns — enough to recognize them, not memorize all 23.
- **System design**: how to design a URL shortener, a chat app, a news
  feed — this is a core interview category at every big tech company.
  Start with *Designing Data-Intensive Applications* (Kleppmann) or the
  freely available "System Design Primer" on GitHub.
- **CI/CD basics**: GitHub Actions, automated testing on push.

## 4. Build real things (your portfolio matters more than your GPA)

- Pick 2-4 substantial personal projects (not tutorials-followed) and
  finish them: a full-stack web app, a CLI tool, something that touches
  a real problem you care about. Put them on GitHub with a real README.
- Contribute to at least one open-source project — even small PRs
  (docs, bug fixes) teach you how real codebases and review processes
  work, and are a concrete talking point in interviews.
- A web stack worth knowing regardless of specialization: HTML/CSS/JS,
  one frontend framework (React is the safest industry default), one
  backend framework (Node/Express, Django, or Flask), REST API design.

## 5. Tools of the trade

- **Linux/command line**: comfortable with the shell, not just GUI tools
  — most servers and a lot of dev work lives here.
- **Docker**: containerize something you built. Kubernetes basics later
  if you're drawn to infra/DevOps.
- **Cloud fundamentals**: pick one of AWS/GCP/Azure, learn the basics
  (compute, storage, a managed database) — free tiers exist for exactly
  this.

## 6. If you're aiming at Silicon Valley / big tech specifically

- DSA interview prep (above) is non-negotiable — this gates the resume
  screen at every FAANG-tier company regardless of your other skills.
- System design prep (mid-to-senior-level roles, but worth starting
  early so it's second nature later).
- Internships: apply early and often — a US/remote internship (or even
  a well-known local tech company) is worth more on a resume than
  another course. Start applying from 2nd/3rd year.
- Behavioral interview prep: have 5-6 concrete stories ready (STAR
  format) about projects, conflicts, failures — every big tech loop
  includes this.
- A clean, specific resume and LinkedIn — quantify impact, not just
  "worked on X."

## 7. If you're aiming at higher studies (MS/PhD) or research

- **Undergrad research experience matters more than almost anything
  else** for grad admissions — approach a professor (at IUT or
  elsewhere) about assisting with research in 2nd/3rd year, even
  unpaid. A publication or even a solid research project writeup is a
  huge differentiator.
- Build real mathematical maturity: real analysis, linear algebra
  (proof-based, not just computational), probability & statistics —
  especially important if you're drawn toward ML/AI research.
- Keep your GPA genuinely strong — unlike industry, grad admissions
  does look closely at it.
- Standardized tests: check current requirements per target
  program/country — many US programs dropped the GRE requirement
  post-2020 but it varies; IELTS/TOEFL is still near-universal.
- Start building relationships with 2-3 professors early — you'll need
  strong, specific letters of recommendation, which requires them
  actually knowing your work, not just your course grade.
- A clear, specific Statement of Purpose built around a research
  direction you can articulate — this is built over years, not written
  the week of the deadline.

## 8. If AI/ML specifically interests you (industry or research)

- Python ML stack: NumPy, pandas, scikit-learn first; then PyTorch.
- Math prerequisites from #2/#7 above are not optional here — linear
  algebra and probability are the actual foundation of ML, not DSA.
- Andrew Ng's ML/DL courses (Coursera) are a well-trodden, solid start.
- Pick a concrete sub-area to go deep in eventually (CV, NLP, RL, etc.)
  rather than staying generalist forever — depth is what research and
  specialized roles reward.

## 9. Soft skills that quietly gate everything above

- Technical writing: a README, a design doc, a research writeup are all
  the same muscle — practice it deliberately.
- Communication/presentation: you will be evaluated on how you explain
  your work as much as the work itself, in interviews and in research.
- English fluency (written and spoken) — especially relevant for
  Silicon Valley roles and international grad programs.

## Semester-by-Semester Plan

Don't try to do all nine sections in parallel. IUT runs 2 semesters/year
(Winter + Summer), 8 semesters total over 4 years. Section numbers below
refer back to the sections above. This is a guide, not a rigid schedule —
shift things around based on how each semester's course load actually
feels.

### Semester 1 — Winter, Year 1 *(current)*
- **DSA (§1)**: arrays, strings, linked lists, stacks/queues, recursion —
  pairs naturally with Structured Programming I. Solve ~5-10 easy LeetCode
  problems/week as a habit, not a sprint.
- **Tools (§5)**: get comfortable in the Linux terminal; learn Git beyond
  add/commit/push (branches, merge conflicts).
- Look into IUT's ICPC/competitive-programming group if one exists —
  easiest to join before you're busy with upper-year courses.

### Semester 2 — Summer, Year 1
- **DSA (§1)**: sorting/searching, hashing, intro trees (BST) and binary
  heaps. Move from easy to easy/medium LeetCode.
- **Web basics (§4)**: HTML/CSS/JS — enough to build one small static
  page or a simple form-driven site.
- **SWE practice (§3)**: write your first real unit tests for something
  you built, even a small script.
- Start your **first personal project** (§4) — small and finishable beats
  big and abandoned.

### Semester 3 — Winter, Year 2
- **DSA (§1)**: trees deeper, graphs (BFS/DFS), basic recursion-heavy
  problems. If you joined a CP group, start rated contests.
- **CS fundamentals (§2)**: start Operating Systems self-study (processes,
  threads, scheduling) — it isn't deep in the core curriculum until later,
  if at all.
- **Databases (§2)**: learn real SQL — schema design, joins, indexes —
  not just SELECT/INSERT.

### Semester 4 — Summer, Year 2
- **DSA (§1)**: dynamic programming (the big one — budget real time here),
  greedy algorithms, shortest-path/MST on graphs.
- **CS fundamentals (§2)**: Computer Networks self-study (TCP/IP, HTTP,
  DNS).
- **Web/projects (§4)**: pick one frontend framework (React) or backend
  framework (Node/Express, Django, Flask) and build **project #2** with it
  — something with a real backend, not just static pages.
- If grad school interests you: start a casual conversation with a
  professor whose work interests you (§7) — too early to ask for a
  position, not too early to be on their radar.

### Semester 5 — Winter, Year 3
- **System design (§3)**: start with the basics — how a URL shortener,
  a chat app, a simple feed are designed. No need for depth yet.
- **Tools (§5)**: Docker — containerize something you've already built;
  pick one cloud provider and do its free-tier fundamentals.
- **Research (§7)**: if grad school is the goal, this is the semester to
  actually start assisting a professor, even unpaid/informal.
- Start applying for your **first internship** (§6) — summer between
  Year 3 and Year 4 is a common internship window.

### Semester 6 — Summer, Year 3
- **System design (§3)**: go deeper — read a chapter of *Designing
  Data-Intensive Applications* or work through the System Design Primer.
- **Open source (§4)**: make your first real contribution (docs or a bug
  fix is a fine start).
- **Project #3** (§4): something more ambitious, ideally touching a real
  problem you care about — this is your strongest portfolio piece.
- If in a research track: aim to have a concrete writeup or draft result
  by the end of this semester.

### Semester 7 — Winter, Year 4
- **Interview prep (§6)**: DSA mock interviews, 5-6 behavioral stories
  in STAR format, resume/LinkedIn overhaul.
- **System design (§6)**: mock interviews specifically, not just reading.
- **If going grad school (§7)**: finalize your Statement of Purpose, ask
  for LORs now (professors need lead time), sit IELTS/TOEFL if required,
  check each target program's specific deadlines — many are as early as
  December for Fall admission the following year.
- Submit full-time job and/or grad school applications this semester or
  early next — deadlines for both tracks tend to land around here.

### Semester 8 — Summer, Year 4
- **Final rounds (§6/§7)**: onsite/virtual interview loops, or grad
  school admission decisions and funding negotiations.
- Finish and polish whatever projects/research/open-source work is still
  open — this is your last semester to add anything to the portfolio
  before it's "final."
- Capstone/thesis if your program has one — make it genuinely good, it's
  often the most substantial thing on your resume/CV at graduation.
- Start transition prep for whatever's next: relocation logistics for a
  job, or program start-date requirements for grad school.
