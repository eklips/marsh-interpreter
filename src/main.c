// main.c
// File created: May 7 2020
// written by moxniso

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

int execute();

machine init_vm(void) {
	machine machine;
	machine.pc = 0;
        machine.x = 0;
	machine.y = 0;
        machine.cf= 0;
	machine.s = 0;
	machine.status_code = 'a';
	machine.halt = FALSE;
	machine.sp = 7008;
	for (int i = 0;i<=9000;i++) machine.memory[i] = 0;
	return machine;
}

BOOL load(machine* unnamed, char* argv1)
{
	FILE* program;
	program = fopen(argv1, "rb");
	if (!program) {
		printf("Unable to open ROM called %s.\n", argv1);
		return FALSE;
	}

	fseek(program, 0, SEEK_END);
	long romSize = ftell(program);
	rewind(program);
	if (romSize > 7000) {
		printf("ROM too large. Must be 7000 bytes or less.\n");
		return FALSE;
	}
	
	char* romBuffer = malloc(sizeof(char) * romSize + 1);
	fread(romBuffer, sizeof(char), romSize, program);

	for (int i = 0;i<=romSize;i++)
	         unnamed->memory[i] = romBuffer[i];

	fclose(program);
	free(romBuffer);
	return TRUE;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: marsh [romname]\n");
		return 0;
	}
	else {
	       if ((strcmp(argv[1], "-version") == 0) || (strcmp(argv[1], "--version") == 0)) {
		  printf("Marsh 8-bit interpreter\ncompiled on %s at %s\nversion 1.3\n2020 moxniso\n", __DATE__, __TIME__);
	          return 0;
	        }

	        fprintf(stdout, "Starting...\n");		
		machine unnamed = init_vm();
		printf("Created vm\n");
		if (!load(&unnamed, argv[1])) {
	       		printf("Unable to load ROM.\n");
       	       		return 1;
		}	       
		printf("Successfully loaded ROM into memory!\n");
		char error_counter, status = 0; // If 4+ errors are encountered, exit.
		do {	
			status = execute(&unnamed, error_counter);
			if (status == 3) error_counter++;	
		} while(status != 2);	
		return 0;
	}
}
