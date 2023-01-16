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
- All edge-triggered writes are controlled by a WE (write enable) input connected to a control signal. The write operation is actually performed at the end of the clock cycle.

The timing of the different elements is as follows:
1. Except for the operations explicitly listed here, the time required to perform an operation is almost zero, and any number of these operations performed one after the other also requires a negligible amount of time.
2. For the operations explicitly listed here, the term "N clock cycle(s)" actually means _almost_ "N clock cycle(s)". This means that a single operation requiring "N clock cycles(s)" can be successfully combined in-series with any number of negligible-time operations in exactly N clock cycle(s). 
- Example: An ALU operation takes "1 clock cycle". This actually means that, in the same clock cycle, the CPU can
  - a) Read operands and dump the values onto the internal data buses (zero time operations), 
  - b) then perform the ALU operation (1 clock cycle), 
  - c) followed by dumping the ALU result onto the internal address bus (zero time operation), and 
  - d) writing this value into the ADDR register (zero time operation).
3. Any operation performed by the ALU requires 1 clock cycle.
  1. During this clock cycle the ALU inputs (values dumped onto both internal data buses and the operation selection) must be stable.
  2. During the last part of the clock cycle the ALU puts the calculated value onto the result output.
4. The external memory operations (either read or write) take exactly two clock cycles. 
  1. For write operations, during this two clock cycles the inputs to the external memory (address, rd, wr, be, and dq) must be stable.
  2. For read operations, during this two clock cycles the inputs to the external memory (address, rd, wr, and be) must be stable.
  3. For read operations, during the last part ff the second clock cycle the external memory dumps the read value onto the dq bus.

## List of control signals

Control signals, in general, follow the nomenclature *op*ELEM or *op*FROMTO:
- *op* is a prefix stating the operation performed by the control signal, always lowercase:
  - *t* means controlling a tristate gate to dump a value onto a bus.
  - *w* or *wr* means writing into an storage element (either register or transparent latch). Currently, *wr* prefix is reserved for writes onto the external memory, all write control signals for CPU-internal storage elements use the *w* prefix.
  - *rd* means reading from a storage element. Currently, applied only the external memory.
  - *i* means increment.  Currently, applied only the Program Counter (PC).
- ELEM is the name of the element, eg. RBANK is the Register Bank, PC is the Program Counter
- FROMTO is a composed name. FROM is always the name of the source element, TO is the name of the destination element or bus. For example, tRB1D1 is the tristate control signal that connects the output of the register bank read output 1 (RB1) and the data bus 1 (D1).

Apart from the signals for the external memory interface, the CPU receives only two signals:
1. RST. High-level asynchronous reset input. This input is asserted high at the beginning of the simulation to initialize the internal elements of the CPU.
2. CLK. The CPU clock. A clock cycle is defined as the time elapsed between a CLK rising edge and the subsequent falling edge. All writes into edge-triggered storage elements occur at the end of the clock cycle (on the falling edge).

All control signals are high-level active and active for an integral number of clock cycles. 
Those signals enabling the write into edge-triggered storage elements (registers, register bank) are actually write enables (WE) signals synchronous to the clock, and the write is actually performed at the end of the clock cycle, at the negative edge of the clock.

The complete list of control signals are:
1. Control signals internal to the Control Unit:
  1. EOF (End-Of-Fetch). It signals the last clock cycle of the fetch phase, when the IR is written with the instruction bits.
  2. EOI (End-Of-Instruction). This control signal is internal to the Control Unit. It signals the last clock cycle of the execution of the current instruction.
2. FETCHing. Signals that the fetch phase is ongoing. It is asserted in the first clock cycle of the execution of any instruction and it remains asserted until the last clock cycle of the fetch phase, when the IR (Instruction Register) is written with the instruction bits.
3. Memory-related control signals:
  1. wrMEM. Write into external memory.
  2. rdMEM. Read from external memory.
  3. wADDR. Write into the ADDR latch. This contains the external memory address.
  4. wWDAT. Write into the WDAT latch. This contains the value to be written into external memory.
  5. tWDATDQ. Dump (through a tristate gate) the WDAT latch onto the external DQ data bus.
  6. wRDAT. Write into the RDAT latch. This contains the value read from external memory.
  7. tRDATD1. Dump (through a tristate gate) the RDAT latch onto the internal D1 data bus.
4. PC-related control signals:
  1. JB (Jump-or-Branch). Signals the instruction may (potentially) change the value of the PC.
  2. XZ (eXpected-Zero-flag). Expected value of the Z (zero) flag from the ALU for the branch to take the jump.
  3. XN (eXpected-Negative-flag). Expected value of the N (negative) flag from the ALU for the branch to take the jump.
  4. XF (eXpected-oVerflow-flag???).
  5. iPC (increment PC). Increment the PC value.
  6. tPCA. Dump (through a tristate gate) the PC register onto the internal address data bus.
  6. tPCD1. Dump (through a tristate gate) the PC register onto the internal D1 data bus.
  
## Design decisions
Several questionable design decisions have been made, some of them to add clarity to the design, some others simply to add a variety of elements in the design (which is important so the user/student can analyze different kinds of elements):
- Two data buses internal to the CPU have been used, one for each operand inputs of the ALU, called "internal data bus 1" (D1) and "internal data bus 2" (D2).
- A MUX + tristate have been used to dump the immediate value (I, S, U, B, or J) to D2 and a separate tristate (with a separate control signal) to dump the value of the second read port of the register bank (RB2) to the same D2 bus. The first MUX has 5 entries (3 selection bits), so integrating RB2 as a 6th one would reduce the number of control signals (the tRB2D2 control signal).
- Four separate tristate gates (with their corresponding control signals) have been used to dump the value of PC, RB1, RDAT, and ALU onto the D1 data bus. Using a 4-to-1 MUX would require only two selection bits, eliminating 2 control signals.
- Two separate tristate gates (with their corresponding control signals) have been used to dump the value of PC and ALU onto the internal address bus. Using a 2-to-1 MUX would require only one selection bit, eliminating 1 control signal.
- The storage elements that connect the CPU with the external memory are built using transparent latches, all the other storage elements of the CPU are edge-triggered registers. Although this reduces the number of clock cycles required to perform some operations, the rationale behind this decision is just to add variety to the CPU internal storage elements.
- A special op1 + op2 - 4 operation is implemented in the ALU to calculate PC-relative jump targets. This allows increasing the current PC value at the end of the fetch phase (so it can be performed bindly irrespective of the instruction to be executed) and, at the same time, eliminates the need of a secondary buffer to store the PC of the current instruction to perform the PC-relative target address calculation. 

## Memory map
The system has a simple memory map:
- 0x00000000 to 0x00007FFF: Instruction + data memory
- 0x00008000 to 0x7FFFFFFF: Unpopulated
- 0x80000000 to 0x80007FFF: Peripherals. TBD.
- 0x80008000 to 0xFFFFFFFF: Unpopulated

## CPU structure

The CPU structure is depicted below

![alt text][imgCpu]

with the components:
1. 

[imgCpu]: (./doc/images/rv32i_mc_2db_cpu.svg) "CPU hardware structure"

### Memory interface

[imgMemiface]: (./doc/images/rv32i_mc_2db_cpu_memiface.svg) "Memory interface hardware structure"

### Control Unit structure

[imgControlunit]: (./doc/images/rv32i_mc_2db_cpu_cu.svg) "Control unit hardware structure"

# Software development
Software development for this system is based on the use of a standard RV32I assembler that generates an intel-hex file from the final executable.
The intel-hex file is loaded by the Qt program to fill the system memory.

# How to simulate
