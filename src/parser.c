// The Marsh 8-bit interpreter
//
// parser.c
// File created: May 7 2020
// written by moxniso

#include <stdio.h>
#include <stdlib.h>
#include "vm.h" 

int execute(machine* vm, char error_counter) {
	if (vm->pc >= 7000)  {
		printf("OUT OF MEMORY\n");
		return 2;
	}
	
	unsigned short opcode_to_decode = vm->memory[vm->pc] << 8 | vm->memory[vm->pc + 1];
	short target;
	char* selected_reg = NULL;
	debug_printf("Executing %x at %d...\nErrors: %d\n", vm->memory[vm->pc], vm->pc, error_counter);
	
	switch(opcode_to_decode & 0xFF00) {
		case(0x0100): // NOP
			vm->pc++;
			break;
		case(0xA000): // ADD
			vm->x += vm->y;
			vm->y = 0;
			vm->pc++;
			break;
		case(0xA100): // JUMP
			target = (vm->memory[vm->pc+1]);  
			debug_printf("Jumping to %x (aka %d)\n", target, target);
			
			if (target < 0 || target > 7000) {
			       printf("Invalid address to jump to: %x", target);
		       	       break;
			}	       
			vm->pc = target;
			break;		
		case(0xB000): // SUB
			vm->x -= vm->y;
			vm->y = 0;
			vm->pc++;
			break;
		case(0x4000): // DUMP
			fprintf(stdout, "\nX:%x, Y:%x S:%x\npc:%d, current ROM data:%x\ncurrent system status:%c\n\n",
			vm->x, vm->y, vm->s, vm->pc, opcode_to_decode, vm->status_code);
			vm->pc++;
			break;
		case(0x5000): // PUSH
			if ((opcode_to_decode & 0x000F) == 0x000C) {
				vm->memory[vm->sp+1] = vm->x;
				vm->sp++;
			}
			else if ((opcode_to_decode & 0x000F) == 0x000D) {
				vm->memory[vm->sp+1] = vm->y;
				vm->sp++;
			}
			else if ((opcode_to_decode & 0x000F) == 0x000B) {
			        vm->memory[vm->sp+1] = vm->s;
			        vm->sp++;
			}
		        else goto unknown_op;
			vm->pc += 2;
			break;
		case(0x6000): // POP
			if ((opcode_to_decode & 0x000F) == 0x000C) {
				vm->x = vm->memory[vm->sp];
				vm->memory[vm->sp] = 0; vm->sp--;
			}
			else if ((opcode_to_decode & 0x000F) == 0x000D) {
				vm->y = vm->memory[vm->sp];
				vm->memory[vm->sp] = 0; vm->sp--;
			}
			else if ((opcode_to_decode & 0x000F) == 0x000B) {
			        vm->s = vm->memory[vm->sp];
			        vm->memory[vm->sp] = 0; vm->sp--;
			}
			else goto unknown_op;
			vm->pc += 2;
			break;
		case(0xF000): // HALT
			vm->halt = TRUE;
			vm->status_code = 'b';
			fprintf(stdout, "SYSTEM HALTED\n");
	      		while(1);  
		case(0xFA00): // END
			vm->halt = TRUE;
			vm->status_code = 'b';
			fprintf(stdout, "EXITING...\n");
			return 2;
		case(0xC000): // MOVE INTO X
			vm->x = (opcode_to_decode & 0x00FF);
			vm->pc += 2;
			break;
		case(0xD000): // MOVE INTO Y
			vm->y = (opcode_to_decode & 0x00FF);
			vm->pc += 2;
			break;	
	        case(0xB100): // MOVE INTO S
		        vm->s = (opcode_to_decode & 0x00FF);
		        vm->pc += 2;
		        break;
	        case(0xFD00): // MOVE REGISTER INTO X,
	        case(0xFE00): // Y,
	        case(0xFF00): // and S
		      switch (opcode_to_decode & 0xFF00) {
		          case(0xFD00):
			    selected_reg = &vm->x;
			    break;
		          case(0xFE00):
			    selected_reg = &vm->y;
			    break;
		          case(0xFF00):
			    selected_reg = &vm->s;
			    break;
		          default:
			    printf("Illegal MOVI instruction: %x\n", opcode_to_decode);
			    vm->pc += 2;
			    selected_reg = NULL;
			    break;
		      }
		      if (selected_reg == NULL) break;
		        // just to see if the register pointer and register are correct
		        debug_printf("MOVI selected_reg: %x, %p, %p\n", *selected_reg, selected_reg, &vm->x);
		        
			switch (opcode_to_decode & 0x00FF) {
		            case(0x00C0):
			        if (*selected_reg == vm->x) vm->x = 0;
			        else vm->x = *selected_reg;
			        break;
		            case(0x00D0):
			        if (*selected_reg == vm->y) vm->y = 0;
			        else vm->y = *selected_reg;
		                break;
		            case(0x00B1):
			        if (*selected_reg == vm->s) vm->s = 0;
			        else vm->s = *selected_reg;
		                break;
                        }
			vm->pc += 2;
			selected_reg = NULL;
			break;
	        case(0xEC00): // CMP X, Y
		        if (vm->x == vm->y) vm->cf = TRUE;
		        else vm->cf = FALSE;
		        break;
		case(0xE000): // SYSCALL
			switch(vm->s) {
			        case(0x04): // PUTCHAR
					fprintf(stdout, "%c", vm->x);
					break;
			        case(0x05): // PUTINT
					fprintf(stdout, "%d", vm->x);
					break;
			        case(0x06): // READCHAR_STACK
				        vm->memory[vm->sp+1] = getchar();
					vm->sp++;
                                        break;
			        case(0x0C): // READCHAR_X
				        vm->x = getchar();
				        break;
			        case(0x0D): // READCHAR_Y
				        vm->y = getchar();
				        break;
			        case(0x0B): // READCHAR_S
				        vm->s = getchar();
					break;
				       
			        default:
					fprintf(stdout, "Invalid syscall (%d)\n", vm->s);
					break;
			}
			vm->pc++;
			break;
		default: // UNKNOWN OPCODE
unknown_op:
			fprintf(stdout, "wtf, unknown opcode! %x\n", (opcode_to_decode >> 8));
			if (error_counter > 4) {
				printf("4+ ERRORS\nFATAL CRASH\n");
				return 2;
			}
			vm->pc++;
			return 3;
	}
	return 0;
}	
