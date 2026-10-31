# Structured Programming I Lab — Linux Command Line & GCC Cheat Sheet

Course Code: CSE 4108 (Lab)   Date: 2026-10-31

## Key Points
- Linux terminal navigation, file ops, and `gcc` compilation are the daily
  tools needed for this lab.
- `gcc file.c -o out` compiles; `./out` runs; `-Wall -g` adds warnings and
  debug info for `gdb`.
- Symbols like `|`, `>`, `&&`, `*` control how commands connect and operate
  on files — memorize these early, they show up constantly.

## Details

### Commands
| Command | Purpose |
|---|---|
| `pwd` | show current directory |
| `ls` | list files/folders |
| `cd` | change directory |
| `tree` | show folder structure as a tree |
| `cp` | copy files/folders |
| `mv` | move/rename files/folders |
| `rm` | delete files |
| `mkdir` | create a new folder |
| `touch` | create an empty file |
| `rmdir` | delete an empty folder |
| `cat` | print whole file content |
| `less` | scroll through a large file |
| `head` / `tail` | show first/last lines of a file |
| `nano` / `vim` | terminal text editors |
| `chmod` | change file permissions |
| `chown` | change file owner |
| `find` | search for files |
| `grep` | search text inside files |
| `locate` | fast indexed file search |
| `man` | manual/documentation for a command |
| `gcc` | compile C code into an executable |
| `gdb` | debug a compiled program |
| `make` | build multi-file projects via a Makefile |
| `apt` | install/remove/update packages |
| `dpkg` | low-level info on installed packages |
| `ps` | list running processes |
| `top` / `htop` | live CPU/memory and process usage |
| `kill` | stop a process by PID |
| `nohup` | keep a program running after terminal closes |
| `jobs` / `fg` / `bg` | manage background jobs |
| `df -h` | disk space usage |
| `du -sh` | size of a folder |
| `free -h` | RAM usage |
| `ssh` | remote login to another machine |
| `cron` | schedule automatic tasks |
| `ping` | test connectivity to a server |
| `curl` / `wget` | download files / make requests |
| `git` | version control (init/clone/add/commit/push/pull) |

### Symbols
| Symbol | Meaning | Example |
|---|---|---|
| `-x` | short flag | `ls -l` |
| `--word` | long flag | `--help` |
| `>` | redirect output to file (overwrite) | `ls > list.txt` |
| `>>` | redirect output to file (append) | `echo hi >> log.txt` |
| `<` | take input from file | `sort < data.txt` |
| `\|` | pipe — output of one command feeds the next | `ps aux \| grep firefox` |
| `&` | run command in background | `python script.py &` |
| `&&` | run next command only if previous succeeded | `cd folder && ls` |
| `;` | run commands one after another regardless | `cmd1 ; cmd2` |
| `*` | wildcard — matches anything | `rm *.txt` |
| `~` | home folder | `cd ~` |
| `.` | current folder | `cp file.txt .` |
| `..` | parent folder | `cd ..` |
| `/` | root folder / path separator | `/home/user/docs` |
| `$` | variable value | `echo $PATH` |
| `$1`, `$2` | script arguments | `./script.sh arg1` |
| `#` | comment line | `# note` |
| `$?` | exit code of previous command (0 = success) | `echo $?` |

### GCC flags
| Flag | Meaning |
|---|---|
| `-o name` | set output executable name |
| `-Wall` | show all warnings |
| `-g` | include debug info (for `gdb`) |

## Summary
Terminal navigation + file ops + `gcc`/`gdb`/`make` + redirection/pipe
symbols cover what's needed day-to-day in the CSE 4108 lab and for general
Linux comfort. Practice by compiling and running actual lab C files from
the terminal instead of a GUI editor.

## References
- Compiled from in-conversation Q&A while setting up Ubuntu in VirtualBox
  for lab practice (2026-10-31).
