
##Instruction Number

From the 7-bit instruction opcode, as the lower 2 bits are always 11 binary, they are discarded. 
Let's call opcode_6_2 = opcode(6:2).
The 3-bits funct3 field, when used, is used in full.
From the funct7 field, when used, only its bit 5 is used. Let's call funct7_5 to this bit.
From the funct12 field, when used, only its bit 0 is used. Let's call funct12_0 to this bit.

Let's assume the fetch phase requires no more than KC cycles; this translates into ROM positions 0 to KC-1.
The first INUM value generated by this component will be, in this case, 1xKC.

Let's also assume that no instruction will require (after the fetch phase) more than KC cycles to finish execution;
INUM values will consecutive values multiple of KC. 

Current value of KC = 4, so the ROM table is divided into 64 "sections", the first one devoted to the instruction fetch phase 
and the rest to the execution of each instruction. To have all sections of equal length the maximum value for KC = 7 if no more than 
2 custom instructions are supported, and max KC = 5 with the 11 custom instructions shown in the table below.

This is not a requirement, just a simplification; if required, an instruction with an INUM = n&times;KC may use up to 2&times;KC cycles 
after the fetch phase simply by not assigning (k+1)&times;KC as the INUM of any other instruction. Not all sections would be of the same length then, but this is not a problem.

| opcode_6_2 | funct3 | funct7_5 | funct12_0 |INUM&divide;K | Instruction/opcode | Comments |
|:----------:|:------:|:--------:|:---------:|-------------:|:-------------------|:---------|
| 00011 | 000 | x | x |    1 | FENCE          | Execute as a NOP 
| 00000 | xxx | x | x |    2 | LOAD	          | All loads are a single instruction from CU's point of view. Differences in size, alignment, or zero/sign-extension for bytes and halfs are handled by the memory interface block. Size is funct3(1:0), unsigned = funct3(2).
| 01000 | xxx | x | x | 0x0C | STORE          | All loads are a single instruction from CU's point of view. Differences in size, alignment, or zero/sign-extension for bytes and halfs are handled by the memory interface block. Size is funct3(1:0), unsigned = funct3(2) (same bits as from LOAD).
| 00101 | xxx | x | x |    3 | AUIPC          | AUIPC 
| 01101 | xxx | x | x |    4 | LUI            | LUI
| 11001 | xxx | x | x |    5 | JALR           | JALR
| 11011 | xxx | x | x |    6 | JAL            | JAL 
| 11000 | 000 | x | x |    7 | BRANCH(BEQ)	  | ( a == b? branch : continue )
| 11000 | 001 | x | x |    8 | BRANCH(BNE)	  | ( a != b? branch : continue )
| 11000 | 100 | x | x |    9 | BRANCH(BLT)	  | ( a < b? branch : continue )
| 11000 | 101 | x | x |   10 | BRANCH(BGE)	  | ( a >= b? branch : continue )
| 11000 | 110 | x | x |   11 | BRANCH(BLTU)	  | ( (unsigned)a < (unsigned)b? branch : continue )
| 11000 | 111 | x | x |   12 | BRANCH(BGEU)	  | ( (unsigned)a >= (unsigned)b? branch : continue )
| 11100 | 000 | x | 0 |   13 | SYSTEM(ECALL)  | ECALL
| 11100 | 000 | x | 1 |   14 | SYSTEM(EBREAK) | EBREAK
| 00100 | 000 | x | x |   15 | OP-IMM (ADDI)  | a + b with I-immediate
| 00100 | 001 | x | x |   16 | OP-IMM (SLLI)  | a << b with I-immediate
| 00100 | 010 | x | x |   17 | OP-IMM (SLTI)  | ( a < b? 1: 0 ) with I-immediate
| 00100 | 011 | x | x |   18 | OP-IMM (SLTIU  | ( (unsigned)a < (unsigned)b? 1: 0 ) with I-immediate
| 00100 | 100 | x | x |   19 | OP-IMM (XORI)  | a ^ b with I-immediate
| 00100 | 101 | 0 | x |   20 | OP-IMM (SRLI)  | a >> b with I-immediate
| 00100 | 101 | 1 | x |   21 | OP-IMM (SRAI)  | a >>* b with I-immediate
| 00100 | 110 | x | x |   22 | OP-IMM (ORI)	  | a | b with I-immediate
| 00100 | 111 | x | x |   23 | OP-IMM (ANDI)  | a & b with I-immediate
| 01100 | 000 | 0 | x |   24 | OP (ADD)		  | a + b
| 01100 | 000 | 1 | x |   25 | OP (SUB)		  | a - b
| 01100 | 001 | x | x |   26 | OP (SLL)		  | a << b
| 01100 | 010 | x | x |   27 | OP (SLT)		  | ( a < b? 1: 0 )
| 01100 | 011 | x | x |   28 | OP (SLTU)	  | ( (unsigned)a < (unsigned)b? 1: 0 )
| 01100 | 100 | x | x |   29 | OP (XOR)		  | a ^ b
| 01100 | 101 | 0 | x |   30 | OP (SRL)		  | a >> b
| 01100 | 101 | 1 | x |   31 | OP (SRA)		  | a >>* b
| 01100 | 110 | x | x |   32 | OP (OR)		  | a | b
| 01100 | 111 | x | x |   33 | OP (AND)		  | a & b
| 00010 | 000 | x | x |   34 | CUSTOM-0 (000) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 001 | x | x |   35 | CUSTOM-0 (001) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 010 | x | x |   36 | CUSTOM-0 (010) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 011 | x | x |   37 | CUSTOM-0 (011) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 100 | x | x |   38 | CUSTOM-0 (100) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 101 | x | x |   39 | CUSTOM-0 (101) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 110 | x | x |   40 | CUSTOM-0 (110) | Custom instruction (room for expansion) - R/I/S/B-type
| 00010 | 111 | x | x |   41 | CUSTOM-0 (111) | Custom instruction (room for expansion) - R/I/S/B-type
| 01010 | xxx | x | x |   42 | CUSTOM-1		  | Custom instruction (room for expansion) - U/J-type
| 10110 | xxx | x | x |   43 | CUSTOM-2		  | Custom instruction (room for expansion) - U/J-type
| 11110 | xxx | x | x |   44 | CUSTOM-3		  | Custom instruction (room for expansion) - U/J-type


## Address Counter

The ROM table is synchronous, ie. it accepts one address in one clock cycle and the corresponding contents are provided in the next clock cycle.

The address counter is simply a register that stores uADDR + 1 in each clock cycle.
A couple of MUXes are used to set uADDR to 0x00 (when one of the RST or EOI signals are active) or to the output value of the Instruction Counter (when the wIR signal is active), or simply to the address of the address counter register. 

- Example 1: In a given clock cycle t the CPU is finishing the execution of some instruction which uses the ROM position at address k in its last clock cycle. In the previous clock cycle uADDR = k, and the value k+1 shouldn't be used.
   1. In the clock cycle t: EOI = 1 from the ROM table at position k (last clock cyle of the instruction); address counter = k+1 already but as EOI is asserted, uADDR = 0. At the same time, the D input of the address counter is uADDR + 1 = 1.
   1. In the next clock cycle t+1: the ROM table outputs content from address 0x00 (first cycle of fetch phase of the next instruction); at the same time, address counter = 1, ROM address = address_counter = 0x01. Next value for address counter = 2.
   1. ...
- Example 2: In a given clock cycle t the CPU is finishing the fetch phase of the execution from ROM contents address w and the instruction number returns INUM=k as the first ROM position after fetch for the current instruction.
   1. In the clock cycle t: wRI = 1 from the ROM table at position w (last clock cycle of fetch phase); address counter = k+1 already but as wRI is asserted, uADDR = INUM = k. At the same time, the D input of the address counter is uADDR + 1 = k+1.
   1. In the next clock cycle t+1: the ROM table outputs content from address k (first cycle after fetch phase of the current instruction); at the same time, address counter = k+1, ROM address = address_counter = k+1. Next value for address counter = k+2.
   1. ...

8.5. ROM table

The ROM table contains the values for the control signals of the CPU datapath, the memory interface and the internal
signals of the control unit itself.
One position of the ROM table corresponds to one clock cycle in the instruction execution.

| bit positions     |    31   	|    30   	|    29   	|    28   	|    27   	|    26   	|    25   	|    24   	|
|:-----------------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| control signal    |   fetch   |  wrMEM  	|  rdMEM  	|  wADDR  	|  wWDAT 	| tWDATDQ 	|   wRDAT 	|  tRDATD1	|

| bit positions     |    23   	|    22   	|  21 - 20  |    19   	|    18   	|    17   	|    16   	|
|:-----------------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| control signal    |  wTAKEJ 	|   XFV     |  selFLAG  |   iPC   	|   wPC   	|   tPCA  	|   tPCD1 	|

| bit positions     |    15   	|    14   	|    13   	|   12 - 9  |    8   	|
|:-----------------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| control signal    |  wRBANK 	|  tRS1D1 	|  tRS2D2 	| selopALU	|    wALU 	|

| bit positions     |    7   	|     6   	|     5   	|   4 - 2   |     1   	|     0   	|
|:-----------------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| control signal    |   tALUA 	|  tALUD1 	|   wIR   	|  selIMM 	|  tIMMD2 	|   EOI   	|


If we assume the external memory requires 2 clock cycles and that ADDR, WDAT, and RDAT are transparent latches, the content of the ROM table follows: 

ROM table contents (in binary)

|                   |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|:-----------------:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:--:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:----:|:-:|:-:|:-:|:-:|:---:|:-:|:-:|---------|
|  control signals  |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |   s  |   |   |   |   |     |   |   |         |
|  control signals  |   |   |   |   |   | t |   | t |   |   |  s |   |   |   |   |   |   |   |   e  |   |   |   |   |     |   |   |         |
|  control signals  |   |   |   |   |   | W |   | R | w |   |  e |   |   |   |   | w | t | t |   l  |   |   | t |   |  s  | t |   |         |
|  control signals  | f | w | r | w | w | D | w | D | T |   |  l |   |   |   | t | R | R | R |   o  |   | t | A |   |  e  | I |   |         |
|  control signals  | e | r | d | A | W | A | R | A | A |   |  F |   |   | t | P | B | S | S |   p  | w | A | L |   |  l  | M |   |         |
|  control signals  | t | M | M | D | D | T | D | T | K | X |  L | i | w | P | C | A | 1 | 2 |   A  | A | L | U | w |  I  | M | E |         |
|  control signals  | c | E | E | D | A | D | A | D | E | F |  A | P | P | C | D | N | D | D |   L  | L | U | D | I |  M  | D | O |         |
|  control signals  | h | M | M | R | T | Q | T | 1 | J | V |  G | C | C | A | 1 | K | 1 | 2 |   U  | U | A | 1 | R |  M  | 2 | I | Comment |
|      address      |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|                   |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|      (fetch)      |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|        0x00       | 1 | 0 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | fecth, PC -> ADDR, RD (1st cycle) 
|        0x01       | 1 | 0 | 1 | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 00 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 1 | 000 | 0 | 0 | fecth, RD (2nd cycle), RDAT -> IR, end of fetch phase
|        0x02       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|        0x03       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|                   |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|      (FENCE)      |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|        0x04       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 1 | Do nothing (no-operation) and finish (EOI)
|        0x05       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|        0x06       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|        0x07       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|                   |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|       (LOAD)      |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|        0x08       | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0000 | 0 | 1 | 0 | 0 | 000 | 1 | 0 | RBANK[rs1] + I-imm -> ADDR
|        0x09       | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | RD (1st cycle) 
|        0x0A       | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 1 | RD (2nd cycle), WDAT -> RBANK[rd], and finish (EOI)
|        0x0B       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|                   |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|      (STORE)      |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
|        0x0C       | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0000 | 0 | 1 | 0 | 0 | 000 | 1 | 0 | RBANK[rs1] + I-imm -> ADDR, RBANK[rs2] -> WDAT
|        0x0D       | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | WR (1st cycle) 
|        0x0E       | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 1 | WR (2nd cycle) and finish (EOI) 
|        0x0F       | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 00 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0000 | 0 | 0 | 0 | 0 | 000 | 0 | 0 | Unused entry
|                   |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
| more entries ...  |   |   |   |   |   |   |   |   |   |   |    |   |   |   |   |   |   |   |      |   |   |   |   |     |   |   |         |
