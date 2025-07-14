![GNU/Linux](https://img.shields.io/badge/GNU%2FLinux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Bash](https://img.shields.io/badge/Bash-4EAA25?style=for-the-badge&logo=gnu-bash&logoColor=white)
![Shell](https://img.shields.io/badge/Shell_Script-121011?style=for-the-badge&logo=gnu-bash&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Makefile](https://img.shields.io/badge/Makefile-003366?style=for-the-badge&logo=cmake&logoColor=white)

<div align="center">
  <img src="https://img.shields.io/badge/42-Project-blue?style=flat-square&logo=42" alt="42 Project">
  <img src="https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c" alt="Language C">
  <img src="https://img.shields.io/badge/Status-Complete-green?style=flat-square" alt="Status">
</div>

# 🐚 Minishell



## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Signal Handling](#signal-handling)
- [Technical Implementation](#technical-implementation)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## 🔍 Overview

**Minishell** is a simplified implementation of a Unix shell, developed as part of the 42 School curriculum. This project recreates the core functionality of bash, providing users with a command-line interface to interact with the operating system. The shell supports command execution, built-in commands, environment variable management, and advanced features like pipes and redirections.

The project demonstrates deep understanding of:
- Process management and system calls
- File descriptor manipulation
- Signal handling
- Memory management
- Parsing and lexical analysis

## ✅ Features

### Core Functionality
- **Command Execution**: Execute system commands with arguments
- **Built-in Commands**: Implementation of essential shell built-ins
- **Environment Variables**: Full support for environment variable expansion
- **Command History**: Navigate through previously executed commands
- **Interactive Prompt**: Clean, informative command prompt display

### Advanced Features
- **Pipes (`|`)**: Connect multiple commands for data flow
- **Redirections**: 
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append redirection (`>>`)
  - Here document (`<<`)
- **Quote Handling**: Proper parsing of single and double quotes
- **Variable Expansion**: Support for `$VAR` and `${VAR}` syntax
- **Exit Status**: Proper handling of command exit codes

### Signal Management
- **CTRL+C**: Interrupt current command
- **CTRL+D**: Exit shell gracefully
- **CTRL+\\**: Quit signal handling

## 🛠 Prerequisites

### System Requirements
- **Operating System**: Linux or macOS
- **Compiler**: GCC or Clang with C99 support
- **Make**: GNU Make utility
- **Readline Library**: For command history and line editing

### Required Libraries
- `readline` - Command line editing and history
- `termcap` - Terminal capability database (dependency of readline)

## 🚀 Installation

### Step 1: Install Readline Library

#### On Linux (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install libreadline-dev
```

#### On Linux (CentOS/RHEL/Fedora):
```bash
# CentOS/RHEL
sudo yum install readline-devel

# Fedora
sudo dnf install readline-devel
```

#### On macOS:
```bash
# Using Homebrew (recommended)
brew install readline

# Using MacPorts
sudo port install readline
```

### Step 2: Clone and Build

```bash
# Clone the repository
git clone https://github.com/aboubakr-jelloulat/Minishell.git
cd Minishell

# Compile the project
make

# Clean build files (optional)
make clean
```

### Step 3: Run Minishell

```bash
./minishell
```

## 🎯 Usage

### Basic Command Execution
```bash
minishell$ ls -la
minishell$ pwd
minishell$ echo "Hello, World!"
```

### Environment Variables
```bash
minishell$ export MY_VAR="Hello"
minishell$ echo $MY_VAR
minishell$ unset MY_VAR
```

### Pipes and Redirections
```bash
# Using pipes
minishell$ ls -la | grep ".c" | wc -l

# Output redirection
minishell$ echo "Hello" > output.txt
minishell$ cat output.txt

# Input redirection
minishell$ wc -l < input.txt

# Append redirection
minishell$ echo "World" >> output.txt
```

### Here Document
```bash
minishell$ cat << EOF
> This is a here document
> Multiple lines supported
> EOF
```

### Quote Handling
```bash
minishell$ echo "Hello $USER"     # Variable expansion
minishell$ echo 'Hello $USER'     # Literal string
minishell$ echo "It's a \"test\""  # Escaped quotes
```

## 🔧 Built-in Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `echo` | Display text with optional -n flag | `echo [-n] [string...]` |
| `cd` | Change directory | `cd [path]` |
| `pwd` | Print working directory | `pwd` |
| `export` | Set environment variables | `export [VAR=value]` |
| `unset` | Remove environment variables | `unset [VAR]` |
| `env` | Display environment variables | `env` |
| `exit` | Exit the shell | `exit [status]` |

## 📡 Signal Handling

Minishell properly handles Unix signals to provide a smooth user experience:

- **SIGINT (Ctrl+C)**: Interrupts the current command and displays a new prompt
- **SIGQUIT (Ctrl+\\)**: Ignored in interactive mode, handled in child processes
- **EOF (Ctrl+D)**: Cleanly exits the shell

## 🏗 Technical Implementation

### Architecture Overview
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│     Lexer       │───▶│     Parser      │───▶│    Executor     │
│   (Tokenizer)   │    │  (AST Builder)  │    │ (Command Runner)│
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Input Handling  │    │ Syntax Analysis │    │ Process Control │
│   & History     │    │   & Validation  │    │   & Built-ins   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Key Components

#### 1. Lexical Analysis
- Tokenizes input into meaningful units
- Handles quotes, operators, and special characters
- Manages whitespace and word boundaries

#### 2. Parsing Engine
- Builds Abstract Syntax Tree (AST)
- Validates command syntax
- Handles operator precedence and associativity

#### 3. Execution Engine
- Manages process creation and termination
- Handles pipe and redirection setup
- Implements built-in command logic

#### 4. Environment Management
- Maintains environment variable state
- Handles variable expansion and substitution
- Manages shell options and configuration

## 📁 Project Structure

```
MINISHELL/
├── includes/
│   ├── enums.h         # Enumeration definitions
│   ├── minishell.h     # Main header file
│   ├── prototypes.h    # Function prototypes
│   └── structs.h       # Data structure definitions
├── src/
│   ├── .build/         # Build artifacts and object files
│   ├── builtins/       # Built-in command implementations
│   ├── env/            # Environment variable management
│   ├── Error/          # Error handling and reporting
│   ├── exec/           # Command execution and process management
│   ├── heredoc/        # Here document implementation
│   ├── lib/            # Custom library functions
│   ├── parsing/        # Syntax analysis and command parsing
│   ├── redirection/    # Input/output redirection handling
│   ├── tokenizer/      # Tokenization and lexical analysis
│   └── main.c          # Entry point and main loop
├── Makefile            # Build configuration
└── README.md           # This file
```

## 🧪 Testing

💻 Launch ./minishell -> because sometimes writing your own shell is the best way to debug life.

### Automated Testing
Consider using shell script test suites or frameworks like:
- Custom bash test scripts for regression testing
- Unit testing with check framework for individual components
- Integration tests with expected output comparison
- Memory leak detection with valgrind during testing sessions

## 🤝 Contributing

This project is part of the 42 School curriculum. While direct contributions may not be appropriate due to academic integrity policies, feedback and suggestions are welcome.

### Development Guidelines
1. Follow the 42 Norm coding standards
2. Ensure no memory leaks using valgrind
3. Test thoroughly with various edge cases
4. Document any significant changes or additions

## 📄 License

This project is created for educational purposes as part of the 42 School curriculum. Please respect academic integrity policies when using this code.

---

<div align="center">
  <strong>Happy Coding! 🚀</strong>
</div>

---
