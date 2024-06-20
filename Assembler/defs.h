#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <unistd.h>

#define LINE_LENGTH 80 /*The valid length for a line*/
#define MAX_FILE_NAME 50 /*The maximum length for a file name*/
#define SYMBOL_SIZE 30 /*The maximum valid length for a symbol*/
#define UNDEFINED_VALUE 512 /*An unvalid numeric operand - used as the negative of the comparison functions (512 is the smallest number that cannot be represented using 10 bits, so we chose it)*/
#define RESERVED_WORD 513 /*An unvalid numeric operand - returned from function when there is an illegal use of a reserved word*/
#define PC 100
#define LOWER_BITS_LIT 31 /*The decimal number for 0000011111*/
#define HIGHER_BITS_LIT 992 /*The decimal number for 1111100000*/

#define COLOR_RED		"\x1b[31m"
#define COLOR_YELLOW	"\x1b[32m"
#define COLOR_OFF		"\x1b[0m"
#define COLOR_BOLD  "\033[1m"
#define BOLD_OFF   "\033[m"

/*A definition of a node at the macros table, which includes the macro's name and cotent*/ 
typedef struct {
	char *name;
	char *content;
}macro;

/*A definition of a node at the externals table, which includes the external symbol's name and the location of its use in memory*/ 
typedef struct {
	char name[SYMBOL_SIZE];
	int address;
}externals;

/*A flag that indicates whether errors were found during the analysis of the file*/
typedef enum {
	success,
	error
}status;

/*A variable that contains the definition type of each symbol*/
typedef enum {
	external,
	relocatable,
	entry,
	unknown
}symbol_type;

/*A definition of a node at the symbols table, which includes the symbol's name, address, type and the type of command stored in it*/ 
typedef struct {
	char *name;
	int address;
	symbol_type type;
	char D_or_I; /*data or instruction symbol*/
}symbol;


/*A definition of an information word, which includes the fields needed for coding*/ 
typedef union{
	struct{
		unsigned int ARE: 2;
		unsigned int target_adrressing: 2;
		unsigned int source_adrressing: 2;
		unsigned int opcode: 4;
	}first_word;
	struct{
		unsigned int ARE: 2;
		signed int value: 8;
	}number;
	struct{
		unsigned int ARE: 2;
		unsigned int value: 8;
	}address;
	struct{
		unsigned int ARE: 2;
		unsigned int target_register: 4;
		unsigned int source_register: 4;
	}registers;
	struct{
		int value: 10;
	}data_word;
}info_word;

/*A definition of a node in the data image or the instruction image, which includes an information word, and pointer to the next word in the image*/ 
typedef struct node *ptr;
typedef struct
{
	info_word data;
	ptr next;
} node;

/*A definition of a pointer to comparation function, which will be used to find the addressing method of operands*/
typedef int (*func_ptr)(char *);

#endif
