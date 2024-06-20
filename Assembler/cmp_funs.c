#include "defs.h"
#include "cmp_funs.h"

/*
Checks whether a word is an instruction word.
@word - the word being tested.
return: the opcode if word is indeed an instruction, otherwise UNDEFINED_VALUE will be returned.
*/
int is_instruction(char *word) 
{
	int i;
	char *ins_table[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
	for(i=0; i<16; i++)
		if(!strcmp(ins_table[i],word))
			return i;
	return UNDEFINED_VALUE;
}


/*
Checks whether a word is a symbol declaration.
@word - the word being tested.\
return: 0 if word is indeed a symbol declaration, RESERVED_WORD if word is reserved word (that is invalid name of symbol), and otherwise UNDEFINED_VALUE will be returned.
*/
int is_symbol_declaration(char *word) 
{
	char temp[LINE_LENGTH+1];
	if(word[strlen(word)-1] != ':')
		return UNDEFINED_VALUE;
	strcpy(temp, word);
	temp[strlen(temp)-1] = '\0';
	if(is_register(temp) != UNDEFINED_VALUE)
		return RESERVED_WORD;
	return is_symbol(temp);
}


/*
Checks whether a word is a symbol.
@word - the word being tested.
return: 0 if word is indeed a symbol, RESERVED_WORD if word is instruction word, and otherwise UNDEFINED_VALUE will be returned.
*/
int is_symbol (char *word) 
{
	int i=1;
	
	if(!isalpha(word[0]) || is_register(word) != UNDEFINED_VALUE)
		return UNDEFINED_VALUE;
	if(is_instruction(word) != UNDEFINED_VALUE)
		return RESERVED_WORD;
	for(i=1; i<strlen(word); i++)
		if(!isalpha(word[i]) && !isdigit(word[i]))
			return UNDEFINED_VALUE;	
	return 0;
}


/*
Checks whether a word is a struct.
@word - the word being tested.
return: the struct's field if word is indeed a struct, otherwise UNDEFINED_VALUE will be returned.
*/
int is_struct(char *word) 
{
	int i=1, field;
	if(!isalpha(word[0]))
		return UNDEFINED_VALUE;
	for(i=1; i<strlen(word); i++)
		if(!isalpha(word[i]) && !isdigit(word[i]))
			break;
	if(i != strlen(word)-2 || word[i] != '.' || (word[i+1] != '1' && word[i+1] != '2'))
		return UNDEFINED_VALUE;
	field = atoi(word+i+1);	 
	return field;
}


/*
Checks whether a word is a register.
@word - the word being tested.
return: the register's number if word is indeed a register, otherwise UNDEFINED_VALUE will be returned.
*/
int is_register(char *word) 
{
	int i;
	char *register_arr[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
	for(i=0; i<8; i++)
		if(!strcmp(register_arr[i],word))
			return i;
	return UNDEFINED_VALUE;
}


/*
Checks whether a word is a number (with # before).
@word - the word being tested.
return: the number if word is indeed a number, otherwise UNDEFINED_VALUE will be returned.
*/
int is_number_operand (char *word) 
{
	if(word[0] != '#')
		return UNDEFINED_VALUE;
	return is_number(word+1);
}


/*
Checks whether a word is a number.
@word - the word being tested.
return: the number if word is indeed a number, otherwise UNDEFINED_VALUE will be returned.
*/
int is_number (char *word) 
{
	int i, num;	
	if(word[0] != '-' && !isdigit(word[0]) && word[0] != '+')
		return UNDEFINED_VALUE;	
	if((word[0] == '-' || word[0] == '+') && strlen(word) == 1)/*if contains just + or - */
		return UNDEFINED_VALUE;	
	for(i=1; i<strlen(word); i++)
		if(!isdigit(word[i]))
			return UNDEFINED_VALUE;
	num = atoi(word);
	if (num >= UNDEFINED_VALUE || num < ((-1)*UNDEFINED_VALUE))
		 return UNDEFINED_VALUE;	 	
	return num;
}


/*
Checks whether a word is a data guidance.
@word - the word being tested.
return: the index of the data guidance word, otherwise UNDEFINED_VALUE will be returned.
*/
int is_data(char *word) 
{
	int i;
	char *data_arr[] = {".data",".string",".struct"};
	for(i=0; i<3; i++)
		if(!strcmp(data_arr[i],word))
			return i;
	return UNDEFINED_VALUE;
}


