# nanoRISCV-mc-2db short description
Simplified, unoptimized, multi-cycle RISC-V RV32I in SystemC + Qt using two internal data buses for educational purposes

# Intro
The idea is to develop a graphical interactive simulation of a minimal computer system based on a RV32I CPU (single hart) with no additional extensions.
The goal is to use MUXes, tristage gates, registers, and latches in the design so it can be used for educational purposes.
An additional educational goal is to add to the simulation the possibility to force the user (aka the student) to determine the value of the control unit signals to execute specific, already-included (in the TV32I ISA) instructions
or enhance the ISA with new instructions (unused opcodes in the RV32I ISA); to this extent the Control Unit is ROM-based and the ROM is editable by the user at runtime.

# Overall hardware structure
The architecture of the system is Von-Neumann. 
The system contains a single memory block containing instructions and data starting at address zero.
Peripherals are memory-mapped starting at address 0x80000000.
The CPU main features:
- It's multi-cycle, ie. the execution of each instruction requires multiple clock cycles,
- It's a non-pipeline design, ie. a CPU doesn't execute an instruction until the previous one is finished,
- Supports RV32I instructions only, without priviledge instruction, exceptions, traps, or interrupts,
- Doesn't support unaligned memory accesses although all properly aligned byte, half, and word accesses are supported.

## Memory map
The system has a simple memory map:
- 0x00000000 to 0x00007FFF: Instruction + data memory
- 0x00008000 to 0x7FFFFFFF: Unpopulated
- 0x80000000 to 0x80007FFF: Peripherals. TBD.
- 0x80008000 to 0xFFFFFFFF: Unpopulated

## CPU structure

### Memory interface

### Control Unit structure

# Software development
Software development for this system is based on the use of a standard RV32I assembler that generates an intel-hex file from the final executable.
The intel-hex file is loaded by the Qt program to fill the system memory.

# How to simulate
