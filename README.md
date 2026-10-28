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
mingw32-make
```

**3. Run the executable:**
The lexer strictly accepts only `.mtnl` files. You can pass your source file as an argument.
* **Windows:** `.\motionlang_lexer.exe valid_test.mtnl`
* **Linux/Mac:** `./motionlang_lexer invalid_test.mtnl`

*(If no file is provided, it defaults to looking for `valid_test.mtnl` in the directory).*

**4. Check the Output:**
Upon successful execution, the lexer will generate a plain text file named **`Symbol Table.txt`** containing the mapped tokens, lexemes, and exact line/column coordinates.

**5. Clean the Workspace:**
To remove generated files, simply run:
```bash
mingw32-make clean
```

**NOTE**: Everytime na may na-change sa code, you must COMPILE the project first before running the executable. Need siya para mag-take effect yung changes.
If walang changes, then kahit yung executable lang ang i-run.

---

## 🛠️ Module Integration Guide

This guide is for team members to hook their specific modules into the shared token architecture.

### 📌 General Setup
Any C file that interacts with tokens must include Module 3's header file at the top:
```c
#include "motionlang_tokens.h"
```

### 🔹 Module 1: Engine & Stream Handlers (Completed)
* **Goal:** Read the source file character by character using `fgetc()`.
* **Requirement for Mod 3:** You must track the `current_line` and `current_column` integers. Every time you read a newline (`\n`), increment the line counter and reset the column counter. 
* **Output:** Passes these spatial coordinates to Module 2 so they can be bundled into the token for accurate error reporting. Fully integrated via the stream bridge.

### 🔹 Module 2: State Transition & Token Matching (Completed)
* **Goal:** Build the manual `switch-case` state machine.
* **Process:** Accumulates valid characters into a string `buffer`. When the state machine hits an accepting state, it calls the Module 3 packaging function:
  ```c
  // Example call:
  Token newToken = createToken(TOKEN_IDENTIFIER, buffer, current_line, current_column);
  ```
* **💡 Pro-Tip:** You do not need to manually check if identifiers are keywords (like `spin`, `grab`, `num`). Just pass them as `TOKEN_IDENTIFIER`, and Module 3's internal DFA registry will automatically upgrade them to reserved words!

PART A (Identifiers, Keywords and Reserved Words, Numbers and Text Literals, Comments): COMPLETED  
PART B (Main Switch loop, Operators, Delimiters and Brackets, Fallbacks): COMPLETED

### 🔹 Module 3: Token Structs & Keyword Registry (Completed)
* **Goal:** Handles the `Token` objects, enums, and keyword lookup logic to keep the state machine clean. 
* **Architecture:** Built strictly as a Deterministic Finite Automaton (DFA) without `<string.h>` or `strcmp` shortcuts to adhere to project constraints.

### 🔹 Module 4: Test Suite & Build (Completed)
* **Goal:** Handles `.mtnl` file syntax testing and strict file extension validation. Implements the `Makefile` build system to replace manual `gcc` commands and generate the strict `Symbol Table.txt` output file.

---

## ⚠️ Troubleshooting

| Error Type | Description & Solution |
| :--- | :--- |
| **Exit Code -1** <br>*(Undefined Reference)* | If you use the VS Code "Run" button and get this error, your editor is only compiling your active tab. <br><br>**Solution:** Use the terminal `mingw32-make` command above, or update your `.vscode/tasks.json` to compile `"${workspaceFolder}\\*.c"`. |
| **Error 4551** <br>*(App Control Block)* | Windows Defender or your antivirus will likely block the freshly compiled `.exe`. <br><br>**Solution:** Add your project folder to your Antivirus/Windows Security **Exclusions** list. |


<img width="708" height="122" alt="image" src="https://github.com/user-attachments/assets/24343c15-a694-41ad-a684-447da17155ff" />

If hindi mahanap yung exact path ng C Compiler, do the following:

1. Press Ctrl + Shift + P in VS Code.
2. Type and select: C/C++: Select IntelliSense Configuration...
3. Choose the GCC compiler that VS Code automatically detects on your system. VS Code will update this line to the correct path automatically. Or, click yung may mga gcc.exe para ma-update yung path.
4. Or first, verify if gcc exists sa device by checking `gcc --version`. Then, type `where.exe gcc` then i-copy yung file path sa `.vscode\c_cpp_properties.json`.