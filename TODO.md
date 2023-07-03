# Project TO-DO 

List of to-do tasks to complete the project.

### Todo

- SVG images:
  - [ ] System block diagram.
  - [ ] Immediate block diagrama.
  - [ ] Update Control Unit circuitry, bits and symbol to reflect changes (replace XFV with isJUMP, rename selIMM as selOp2, eliminate selFLAG, ...)
- SystemC:
- Qt GUI:
- Documentation:
  - [ ] ALU section: operations and flags
- Other:
  - [ ] A latch WE signal must stop enabling the write just before the end of the clock period. If not, the data input of the next clock period could arrive (as it takes zero time) before the WE signal is deasserted, storing the wrong value. Probably the easiest way would be the use of a hidden, global "latch inhibit" signal asserted around the clock edge that finishes the clock period.
  Other posibility would be the use of a micro-clock that divides the clock period into, say, 100 chunks or micro-clock periods ([0,99]);
  and allow the latches to write the incoming value into micro-clocks [1,99], and all control signals to change in micro-clock period 0. 

### In Progress

- SVG images:
- SystemC:
- Qt GUI:
- Documentation:
- Other:

### Done ✓

- SVG images:
  - [x] Logic gates, MUXes, registers, etc. filled in white to be consistent.
  - [x] Rename external address bus to ADDR so it can be distinguished from the internal address bus A.
  - [x] PC block with instr_addr and base outputs.
  - [x] CPU block using the PC block with instr_addr (to internal address bus) and base (to internal D bus) outputs.
  - [x] Update storage elements to reflect ADDR, WDAT, and RDAT are transparent latches.
- SystemC:
- Qt GUI:
- Documentation:
  - [x] Rename external address bus to ADDR so it can be distinguished from the internal address bus A.
  - [x] PC block with instr_addr and base outputs.
- Other:
  - [x] Use funct3 for branches to decide the flags used to determine if the branch is taken, so all branches are treated as a single "instruction" in the control unit (much like loads and stores)
