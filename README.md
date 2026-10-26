# 🏃‍♂️ MotionLang Lexer

> **MotionLang** is a beginner-friendly, procedural programming language based on real-world motion and spatial actions. Replacing abstract jargon with tangible metaphors, it treats data as physical objects being gathered, transformed, stored, and routed through loops mimicking circular momentum.

This repository contains the **Lexical Analyzer (Scanner)** for MotionLang, built in C from scratch for our Principles of Programming Languages (PPL) project (Group 3).

---

## 🚀 How to Clone and Run

**1. Clone the repository:**
Open your VS Code terminal and run:
```bash
git clone https://github.com/Romar-jpg/MotionLang.git
```

**2. Compile the project:**
Navigate into the folder and use GCC to compile:
```bash
gcc main.c motionlang_tokens.c -o motionlang_lexer
```

**3. Run the executable:**
* **Windows:** `.\motionlang_lexer.exe`
* **Linux/Mac:** `./motionlang_lexer`

---

## 🛠️ Module Integration Guide

This guide is for team members to hook their specific modules into the shared token architecture.

### 📌 General Setup
Any C file that interacts with tokens must include Module 3's header file at the top:
```c
#include "motionlang_tokens.h"
```

### 🔹 Module 1: Engine & Stream Handlers
* **Goal:** Read the source file character by character using `fgetc()`.
* **Requirement for Mod 3:** You must track the `current_line` and `current_column` integers. Every time you read a newline (`\n`), increment the line counter and reset the column counter. 
* **Output:** Pass these spatial coordinates to Module 2 so they can be bundled into the token for accurate error reporting.

### 🔹 Module 2: State Transition & Token Matching
* **Goal:** Build the manual `switch-case` state machine.
* **Process:** Accumulate valid characters into a string `buffer`. When your state machine hits an accepting state (e.g., reaching a space after spelling `"spin"`), call the Module 3 packaging function:
  ```c
  // Example call:
  Token newToken = createToken(TOKEN_IDENTIFIER, buffer, current_line, current_column);
  ```
* **💡 Pro-Tip:** You do not need to manually check if identifiers are keywords (like `spin`, `grab`, `num`). Just pass them as `TOKEN_IDENTIFIER`, and Module 3's internal DFA registry will automatically upgrade them to reserved words!

### 🔹 Module 3: Token Structs & Keyword Registry (Completed)
* **Goal:** Handles the `Token` objects, enums, and keyword lookup logic to keep the state machine clean. 
* **Architecture:** Built strictly as a Deterministic Finite Automaton (DFA) without `<string.h>` or `strcmp` shortcuts to adhere to project constraints.

### 🔹 Module 4: Test Suite & Build (Pending)
* **Goal:** Will handle `.easy` file testing and setting up the `Makefile` to replace the manual `gcc` commands.

---

## ⚠️ Troubleshooting

| Error Type | Description & Solution |
| :--- | :--- |
| **Exit Code -1** <br>*(Undefined Reference)* | If you use the VS Code "Run" button and get this error, your editor is only compiling your active tab. <br><br>**Solution:** Use the terminal `gcc` command above, or update your `.vscode/tasks.json` to compile `"${workspaceFolder}\\*.c"`. |
| **Error 4551** <br>*(App Control Block)* | Windows Defender or your antivirus will likely block the freshly compiled `.exe`. <br><br>**Solution:** Add your project folder to your Antivirus/Windows Security **Exclusions** list. |
### 🔹 Module 4: Test Suite & Build
Will handle `.mtnl` file testing and setting up the `Makefile` to replace the manual `gcc` commands.


<img width="708" height="122" alt="image" src="https://github.com/user-attachments/assets/24343c15-a694-41ad-a684-447da17155ff" />

If hindi mahanap yung exact path ng C Compiler, do the following:

1. Press Ctrl + Shift + P in VS Code.
2. Type and select: C/C++: Select IntelliSense Configuration...
3. Choose the GCC compiler that VS Code automatically detects on your system. VS Code will update this line to the correct path automatically. Or, click yung may mga gcc.exe para ma-update yung path.
4. Or first, verify if gcc exists sa device by checking `gcc --version`. Then, type `where.exe gcc` then i-copy yung file path sa `.vscode\c_cpp_properties.json`.
