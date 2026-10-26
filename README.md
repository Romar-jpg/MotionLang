# MotionLang Lexer

MotionLang is a beginner-friendly, procedural programming language based on real-world motion and spatial actions. Replacing abstract jargon with tangible metaphors, it treats data as physical objects being gathered, transformed, stored, and routed through loops mimicking circular momentum.

This repository contains the Lexical Analyzer (Scanner) for MotionLang, built in C from scratch for our Principles of Programming Languages (PPL) project (Group 3).

## 🚀 How to Clone and Run
1. Open your VS Code terminal and clone the repository:
   ```bash
   git clone https://github.com/YourUsername/MotionLang.git
   ```
2. Navigate into the folder and compile the project using GCC:
   ```bash
   gcc main.c motionlang_tokens.c -o motionlang_lexer
   ```
3. Run the executable:
   * **Windows:** `.\motionlang_lexer.exe`
   * **Linux/Mac:** `./motionlang_lexer`

---

## 🛠️ Module Integration Guide (For Team Members)

### 📌 General Setup
Any C file that interacts with tokens must include Module 3's header file at the top:
```c
#include "motionlang_tokens.h"
```

### 🔹 Module 1: Engine & Stream Handlers
**Goal:** Read the source file character by character using `fgetc()`.
* **Requirement for Mod 3:** You must track the `current_line` and `current_column` integers. Every time you read a newline (`\n`), increment the line counter and reset the column counter. 
* You will pass these coordinates to Module 2 so they can be bundled into the token for accurate error reporting.

### 🔹 Module 2: State Transition & Token Matching
**Goal:** Build the manual `switch-case` state machine.
* Accumulate valid characters into a string `buffer`.
* When your state machine hits an accepting state (e.g., reaching a space after spelling "spin"), call the Module 3 packaging function:
```c
// Example call:
Token newToken = createToken(TOKEN_IDENTIFIER, buffer, current_line, current_column);
```
* **Note:** You do not need to manually check if identifiers are keywords (like `spin`, `grab`, `num`). Just pass them as `TOKEN_IDENTIFIER`, and Module 3's internal registry will automatically identify them as reserved words!

### 🔹 Module 3: Token Structs & Keyword Registry (Completed)
Handles the `Token` objects, enums, and keyword lookup logic to keep the state machine clean.

### 🔹 Module 4: Test Suite & Build
Will handle `.easy` file testing and setting up the `Makefile` to replace the manual `gcc` commands.

### TROUBLESHOOTING 
<img width="708" height="122" alt="image" src="https://github.com/user-attachments/assets/24343c15-a694-41ad-a684-447da17155ff" />

If hindi mahanap yung exact path ng C Compiler, do the following:

1. Press Ctrl + Shift + P in VS Code.
2. Type and select: C/C++: Select IntelliSense Configuration...
3. Choose the GCC compiler that VS Code automatically detects on your system. VS Code will update this line to the correct path automatically. Or, click yung may mga gcc.exe para ma-update yung path.
4. Or first, verify if gcc exists sa device by checking `gcc --version`. Then, type `where.exe gcc` then i-copy yung file path sa `.vscode\c_cpp_properties.json`.
