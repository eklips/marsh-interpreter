# marsh-interpreter
A bytecode interpreter for the Marsh 8-bit instruction set.

# What is Marsh?
Marsh is a simple 8-bit instruction set inspired by the MOS 6502 and CHIP-8. The programs are written in Marsh assembly and then
assembled to create a ROM, which is then run via this interpreter/VM. 

https://github.com/moxniso/marsh-assembler

# What platform is this for?
Anything that compiles C. The only thing Marsh really uses is stdio.h and stdlib.h

# What does the testing ROM do?
It is a sanity test, here is the program flow... 

1. 0x40 - dump the machine state
2. 0xC003 - move 3 into X
3. 0xD004 - move 4 into Y
4. 0xA0 - add X and Y and store the sum in X.
5. 0xD005 - move 5 into Y
6. 0x500D - push Y onto the stack
7. 0x600C - pop what the stack pointer is pointing at into X (5)
8. 0x40 - dump the machine state
9. 0xA159 - jump to byte 59 of the ROM

10. (at byte 59) 0x00 - empty opcode, to intentionally cause an error
11. 0xFA - exit all
12. 0xF0 - halt the system (never executed)
