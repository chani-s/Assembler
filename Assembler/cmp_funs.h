#ifndef CMP_FUNS_H
#define CMP_FUNS_H

/*
Checks whether a word is an instruction word.
@word - the word being tested.
return: the opcode if word is indeed an instruction, otherwise UNDEFINED_VALUE will be returned.
*/
int is_instruction(char *word); 


/*
Checks whether a word is a symbol declaration.
@word - the word being tested.\
return: 0 if word is indeed a symbol declaration, RESERVED_WORD if word is reserved word (that is invalid name of symbol), and otherwise UNDEFINED_VALUE will be returned.
*/
int is_symbol_declaration(char *word);


/*
Checks whether a word is a symbol.
@word - the word being tested.
return: 0 if word is indeed a symbol, RESERVED_WORD if word is instruction word, and otherwise UNDEFINED_VALUE will be returned.
*/
int is_symbol (char *word);


/*
Checks whether a word is a struct.
@word - the word being tested.
return: the struct's field if word is indeed a struct, otherwise UNDEFINED_VALUE will be returned.
*/
int is_struct(char *word);


/*
Checks whether a word is a register.
@word - the word being tested.
return: the register's number if word is indeed a register, otherwise UNDEFINED_VALUE will be returned.
*/
int is_register(char *word);


/*
Checks whether a word is a number (with # before).
@word - the word being tested.
return: the number if word is indeed a number, otherwise UNDEFINED_VALUE will be returned.
*/
int is_number_operand (char *word);


/*
Checks whether a word is a number.
@word - the word being tested.
return: the number if word is indeed a number, otherwise UNDEFINED_VALUE will be returned.
*/
int is_number (char *word);


/*
Checks whether a word is a data guidance.
@word - the word being tested.
return: the index of the data guidance word, otherwise UNDEFINED_VALUE will be returned.
*/
int is_data(char *word);


#endif
