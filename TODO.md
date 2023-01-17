# Project TO-DO 

List of to-do tasks to complete the project.

### Todo

- SVG images:
  - [ ] System block diagram.
  - [ ] Immediate block diagrama.
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
  - [x] CPU block using the PC block with instr_addr (to internal address bus) and base (to internal D1 bus) outputs.
- SystemC:
- Qt GUI:
- Documentation:
  - [x] Rename external address bus to ADDR so it can be distinguished from the internal address bus A.
  - [x] PC block with instr_addr and base outputs.
- Other:
