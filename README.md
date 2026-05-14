# TinyCompiler

TinyCompiler is a lightweight compiler designed to translate code written in a custom language into an intermediate representation or machine-executable code. This project serves as a learning resource for compiler design and construction principles, ideal for students and enthusiasts looking to dive into language processing and code generation.

---

## Features

- Custom parser for the provided language syntax
- Abstract syntax tree (AST) generation
- Intermediate code generation
- Lexer and tokenizer for breaking down input code
- Error handling and diagnostics for invalid syntax

---

## Required Libraries

This project requires the following standard libraries:

- **<string>**: For string manipulations in classes such as `Emitter`, `Token`, and others.
- **<iostream>**: For input/output stream operations, such as printing diagnostic messages.
- **<fstream>**: For writing compiled code to output files.
- **<unordered_map>**: For storing and efficiently accessing keyword mappings (used in `Token` class).
- **<unordered_set>**: For tracking symbols and labels in the `Parser` class.

No external libraries are needed (beyond the standard C++ library).

---

## Getting Started

### Prerequisites
Ensure you have the following installed:
- **C++ Compiler**: GCC (g++) or Clang with C++17 (or higher) support.  

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/Ayush-Sharma23/TinyCompiler.git
   ```
2. Navigate to the project directory:
   ```bash
   cd TinyCompiler
   ```
3. Compile the source code:
   ```bash
   g++ -std=c++17 -o tiny_compiler *.cpp
   ```

### Usage
1. Create your program file in the TINY language (e.g., `input_code.txt`).
2. Run the compiled `tiny_compiler` executable and provide the TINY file as a command-line argument:
   ```bash
   ./tiny_compiler input_code.txt
   ```
3. After successful execution, the compiler will output a translated C file named `out.c` in the current directory.
4. You can then compile the generated `out.c` file using a C compiler like GCC:
   ```bash
   gcc -o output_program out.c
   ```
5. Run the compiled program:
   ```bash
   ./output_program
   ```

---

## Files

All source code files are located at the root of the repository:

- **tinyCompiler.cpp**: Contains the entry point (`main`) for the compiler.
- **emit.h**: Contains the `Emitter` class for generating output code.
- **lex.h**: Defines the lexical analyzer (`lex`) and token classes for tokenizing input code.
- **parser.h**: Implements the parser that checks syntax and generates intermediate representations.

Example and test files can be added as needed for demonstration.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- This project is my implementation of the TINY Compiler made in Austin Z. Henley's **[blog](https://austinhenley.com/blog/teenytinycompiler1.html)**
- Open-source tools and libraries used within the project
