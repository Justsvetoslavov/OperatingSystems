# Operating Systems Materials Repository

Comprehensive collection of seminar notes, example scripts, solved tasks, past exam problems, and auxiliary theory resources for an Operating Systems (Операционни системи) course. The content mixes English and Bulgarian. This root README gives you a roadmap.

## High-Level Structure

| Folder | Purpose |
|--------|---------|
| `01-Seminars/` | Weekly/sequential seminar materials: explanations, example shell/C snippets, and solution scripts. |
| `02-Exams/` | Archived past exam problem sets (Shell, Bash scripting, and C programming) organized by year and topic. |
| `03-Theory/` | Core theory summaries, PDF notes, diagrams (process states, RAID), and exam preparation documents. |
| `04-Utils/` | Supplementary utilities: cheat sheets, reference PDFs, homework archives, and curated useful links. |

---
## 01-Seminars/
Seminar-focused learning materials. Each `Sem.xx` directory (e.g. `Sem.01`, `Sem.02`, … `Sem.05`) contains:
- `README.md` – Main topical explanation for that seminar (e.g. Bash basics, variables, I/O, processes, pipes).
- `Solutions/` – Individual task solution scripts (mostly Bash `taskNN.sh`).
- Additional nested thematic folders for later seminars (e.g. in `Sem.05`: `01-Input&Output/`, `02-Processes/`, `03.Pipes/`) each with its own `README.md` and sometimes a `Solutions/` subfolder.
- Images or supporting assets when needed.

Typical progression:
1. Intro & Linux architecture basics.
2. Shell basics, redirections, simple scripting, collections of practice tasks.
3. Expanded Bash scripting constructs (parameters, loops, subshell, process substitution, exit status, tests).
4. Deeper scripting patterns (as seen in the attached detailed Bash README) plus structured solutions (`task01.sh` …).
5. System-level themes: file descriptors & I/O, process creation, inter-process communication (pipes), etc.

> Tip: Open a seminar README side‑by‑side with its `Solutions/` scripts to connect theory to implementation.

---
## 02-Exams/
Historic exam preparation materials split by domain and year.

Subdirectories:
- `01-Shell/` – Shell scripting fundamentals tasks (by year: 2016–2023, with possible variants like `SE`, `IN`).
- `02-Bash/` – Bash‑specific problem sets per year (concepts: parameter handling, text processing, automation). Variants such as `SE` (Software Engineering) / `CE` (Computer Engineering) / `IN` (Informatics) indicate track/specialization versions.
- `03-C/` – C programming OS‑centric problems:
  - `Input&Output/` – Low-level file descriptor I/O, buffering, redirection style tasks.
  - `Processes&Pipes/` – `fork`, `exec`, `wait`, `pipe`, process coordination, simple shells, filters.
  - Year subfolders (2016–2023) often with different variant codes (`SE`, `IN`).

Files inside often include statements plus (sometimes) skeletons or solution hints. Use them for timed practice—simulate exam conditions by hiding solutions until after attempting.

---
## 03-Theory/
Concise theoretical references and exam prep documents:
- `README.md` – Index or overview.
- `temi_2024.pdf`, `ОС_Подготовка_Изпит-1.pdf`, `Устен-изпит-ОС.pdf` – Bulgarian exam preparation outlines / oral exam topics.
- `OS Theory (main).pdf`, `Skelet-Materials.pdf`, `Little Book Of Semaphores.pdf` – Core OS topics (synchronization, concurrency, classic problems).
- `c_functions.pdf` – C standard / system function reference.
- `images/` – Diagrams (process lifecycle, RAID, etc.).

Great for revising conceptual areas before delving into practice tasks.

---
## 04-Utils/
Support and helper resources:
- `Cheatsheets/` – Quick reference PDFs (Bash redirections, `vi` commands).
- `Theory/` – Additional theory PDFs overlapping or deepening `03-Theory` topics.
- `Useful-Links/` – Curated external references (`README.md`).

Use these for fast lookups while scripting or coding in C.

---
## Quick Reference
| Topic | Where to Start |
|-------|----------------|
| Variables & Expansion | `Sem.04/README.md` (Variables section) |
| Positional Params | `Sem.04/README.md` (Parameters of scripts) |
| I/O Redirection | Earlier seminars + Cheatsheet PDF |
| Loops & Conditionals | `Sem.04/README.md` (for/while/if/case) |
| Process Control | `Sem.05/02-Processes/README.md` (once populated) |
| Pipes & IPC | `Sem.05/03.Pipes/` |
| C System Calls | `02-Exams/03-C/` + `c_functions.pdf` |
| Synchronization (theory) | `Little Book Of Semaphores.pdf` |
