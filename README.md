
# SIC/XE Assembler

A simplified two-pass assembler for the SIC/XE architecture written in C. This project is structured for maintainability and extensibility, following best practices suitable for a senior software engineering approach. 

## Table of Contents

- [SIC/XE Assembler](#sicxe-assembler)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Project Structure](#project-structure)
  - [Features](#features)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Example Assembly Program](#example-assembly-program)
  - [Extending the Project](#extending-the-project)
  - [Limitations](#limitations)
  - [Dependencies](#dependencies)
  - [Contributing](#contributing)
  - [License](#license)

## Overview

The SIC/XE Assembler is a two-pass assembler for the SIC/XE machine architecture. It reads SIC/XE assembly source code and produces corresponding object code. While simplified, it demonstrates the core principles of:
- **Pass 1**: Scanning source code, creating a symbol table, and computing addresses.
- **Pass 2**: Generating object code using the symbol table and opcode definitions.

This project serves as a foundation for learning assembler construction and can be extended to support more features, directives, and addressing modes.

## Project Structure

```
SICXEAssembler/
├── include/
│   ├── assembler.h     # Declarations for assembler passes
│   ├── opcode.h        # Declarations for opcode table and lookup functions
│   └── symbol.h        # Declarations for symbol table management
├── src/
│   ├── assembler.c     # Implementation of Pass 1 and Pass 2
│   ├── main.c          # Entry point of the assembler program
│   ├── opcode.c        # Implementation of opcode table lookup
│   └── symbol.c        # Implementation of symbol table functions
├── data/
│   └── example.asm     # Example SIC/XE assembly source file for testing
├── Makefile            # Build configuration
└── README.md           # Project documentation
```

- **include/**: Header files with data structures and function prototypes.
- **src/**: Source code implementing assembler logic.
- **data/**: Example assembly program(s).
- **Makefile**: Compiles the project.
- **README.md**: Project documentation.

## Features

- **Two-Pass Assembly**: 
  - **Pass 1**: Reads assembly source, constructs a symbol table, and calculates instruction addresses.
  - **Pass 2**: Translates assembly instructions into object code using the symbol table and opcode definitions.
- **Basic Opcode Support**: Handles a subset of SIC/XE instructions (e.g., `LDA`, `STA`, `LDCH`, etc.).
- **Directives Handling**: Supports essential directives like `START`, `END`, `WORD`, `BYTE`, `RESW`, and `RESB`.
- **Modular Design**: Code is organized into separate modules for symbol table management, opcode handling, and assembler passes.
- **Extensible**: Easily add more opcodes, directives, and advanced addressing modes with a modular design.

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/SICXEAssembler.git
   cd SICXEAssembler
   ```

2. **Build the project** using the provided Makefile:
   ```bash
   make
   ```
   This command compiles the source files and creates an executable named `sicxeasm`.

## Usage

Run the assembler with a source file and specify the output object file:
```bash
./sicxeasm <sourcefile.asm> <objectfile.obj>
```

**Example:**
```bash
./sicxeasm data/example.asm output.obj
```
This assembles `data/example.asm` and produces an object code file named `output.obj`.

## Example Assembly Program

An example assembly file is located at `data/example.asm`. Its contents:
```asm
COPY    START   1000
FIRST   LDA     ALPHA
        STA     BETA
        LDCH    CHARZ
        STCH    CHARZ
        BYTE    C'EOF'
        WORD    3
        RESW    1
        RESB    2
ALPHA   WORD    5
BETA    RESW    1
CHARZ   BYTE    C'Z'
        END     FIRST
```
This file demonstrates common instructions and directives used for testing the assembler.

## Extending the Project

The project is structured to facilitate enhancements:

- **Adding New Opcodes**: Update `opcodeTable` in `src/opcode.c` and adjust opcode size/format.
- **Supporting Addressing Modes**: Enhance the code generation logic in `pass2()` within `src/assembler.c` to handle immediate, indirect, PC-relative, and base-relative addressing.
- **Supporting More Directives**: Modify `pass1()` in `src/assembler.c` to support additional assembler directives such as `EQU`, `ORG`, `LTORG`.
- **Error Handling**: Implement comprehensive parsing and error detection to handle syntax errors, duplicate symbols, undefined symbols, etc.
- **Optimizing Text Records**: Improve text record packing in `pass2()` to manage longer object code sequences per record.
- **Modular Enhancements**: Add new modules or libraries for complex tasks like literal pool management or expression evaluation.

## Limitations

- **Simplified Instruction Handling**: Currently, the assembler supports a limited set of opcodes and assumes a direct addressing mode.
- **Limited Directives**: Some SIC/XE directives and features are not yet implemented.
- **Basic Error Handling**: Error detection and reporting is minimal.
- **Simplified Object Code Generation**: Does not fully implement SIC/XE format details such as proper `nixbpe` bits, format 4 instructions, or correct PC/base relative addressing.

These limitations are intentional for educational purposes and can be systematically addressed during further development.

## Dependencies

- A C compiler supporting C99 (e.g., `gcc`).
- Standard C libraries.

## Contributing

Contributions, suggestions, and bug reports are welcome. Please fork the repository, create feature branches, and open pull requests with well-documented changes. Maintain modularity and coding standards when extending functionality.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details (if you choose to add one).
