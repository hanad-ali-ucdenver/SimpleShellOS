# SimpleShell

A lightweight, minimal Unix shell implementation written in C.

## Overview

SimpleShell is a basic command-line interface that provides fundamental shell functionality. It supports both built-in commands and external program execution through system calls.

## Features

- Command parsing and execution
- Built-in commands:
  - `cd`: Change directory
  - `exit`: Exit the shell
- External command execution using fork/exec
- Simple and clean command prompt

## How to Compile

Compile SimpleShell using GCC:

```bash
gcc -o simpleshell simpleshell.c
```

## Usage

After compiling, run the shell:

```bash
./simpleshell
```

You'll be presented with a prompt `>` where you can enter commands.

## Supported Operations

### Built-in Commands

- `cd [directory]`: Change the current working directory
- `exit`: Exit the shell

### External Commands

SimpleShell can execute any program available in the system's PATH, for example:
- `ls`: List directory contents
- `cat [file]`: Display file contents
- `grep [pattern] [file]`: Search for patterns

## Implementation Details

SimpleShell is structured around three main components:

1. **Command Parsing**: The `parse_command()` function breaks input into arguments
2. **Built-in Command Execution**: The `execute_builtin()` function handles internal commands
3. **External Command Execution**: The `execute_command()` function uses fork and exec to run external programs