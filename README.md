# 🔗 Pipex

> A simplified recreation of shell piping (`|`) and command redirection, implemented in C as part of the 42 School curriculum.

---

## 📌 Project Overview

**Pipex** is a project that reproduces the Unix shell's pipe and redirection behavior. It takes multiple commands and connects them via pipes, executing them in sequence with the appropriate input/output redirection.

This project enhances your understanding of:
- File descriptors  
- Pipes and processes  
- `fork`, `execve`, `dup2`  
- Environment variables and path resolution  

---

## 🎯 Objectives

- Reproduce shell-like behavior for:
  - Simple pipe: `cmd1 < infile | cmd2 > outfile`
  - Bonus: multiple pipes (e.g., `cmd1 | cmd2 | cmd3 | ...`)
- Handle command execution with arguments
- Properly manage file descriptors, memory, and child processes
- Respect environment variables for `PATH` resolution
- Implement robust error handling

---

## 🔧 Requirements

- Unix-like OS (Linux or macOS)
- `cc` or `gcc` compiler
- Make utility

---

## 📥 Installation & Compilation

```bash
git clone https://github.com/falhaimouni/pipex.git
cd pipex
make
```
## 🚀 Usage

---

### ✅ Mandatory Usage (2 commands)

```bash
./pipex infile "cmd1" "cmd2" outfile
```
## 💡 Equivalent to
```bash
< infile cmd1 | cmd2 > outfile
```
📦 Example
```bash
./pipex input.txt "grep hello" "wc -l" output.txt
```

