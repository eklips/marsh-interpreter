#ifndef VM_H
#define VM_H

typedef enum {FALSE, TRUE} BOOL;
#ifdef DEBUG
#define debug_printf(...) printf(__VA_ARGS__);
#else
#define debug_printf(...) NULL;
#endif

typedef struct unnamed_struct {
	char x; 
	char y;
        char s;
  unsigned char memory[9000]; // memory map detailed in DOCUMENTATION
	int pc;
	int sp;
        BOOL cf;
	char status_code;
	BOOL halt;	
} machine;

#endif
