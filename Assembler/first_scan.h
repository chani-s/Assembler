#ifndef FIRST_SCAN_H
#define FIRST_SCAN_H

/*
The main function of the first scan.
@file - the file after the macro retirement.
*/
void first_scan(FILE *file);


/*
Analyze a line from the file at a first scan.
@line - the analyzed line.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the file - success if the file is correct, and otherwise -error.
*/
status first_line_analyze (char *line, int line_number, status s);


/*
Analyze an instruction line from the file at a first scan.
@line - the analyzed line.
@opcode - the opcode of the instruction that have to be analyze
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status analyze_instruction_line(char* line, int opcode, int line_number, status s);

/*
Add symbol name to the symbol table.
@symbol_name - the symbol name.
@s - the status of the line.
@i - the relevant index in the symbol table.
@line_number - the number of the current line.
return: the status of the line - error or success.
*/
status add_to_table(char *symbol_name, status s, int *i, int line_number);


/*
Analyze a data line from the file at the first scan
@line - the analyzed line.
@data_type - the type of the data that have to be analyze.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status analyze_data_line (char *line, char *data_type, int line_number, status s);


/*
Encode the numbers as info-words.
@line - the data line which include numbers declarrations.
@data_type - the type of the data that have to be analyze.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status encode_data_arguments (char *line, char *data_type, int line_number, status s);


/*
Encode the characters of the string as info-words.
@line - the data line which include string declarrations.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status encode_string_arguments (char *line, int line_number, status s);


/*
Encode the characters of the string as info-words.
@line - the line which include a statement about an external or entry symbol.
@command_type - .extern or .entry . 
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status analyze_extern_or_entry_line(char *line, char *command_type, int line_number, status s);


/*
Checking the compatibility of the operand addressing method with the instruction word.
@operand_addressing - the addressing method number.
@operand_code - the encoding value returned from calling the comparison functions. 
@operand - the string of the operand itself.
@start - the lowest legal addressing method number.
@end - the highest legal addressing method number.
@op_type - 1 if source operand, 2 if target operand.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status addressing_validation(int *operand_addressing, int *operand_code, char *operand, int start, int end, int op_type, int line_number, status s);


/*
Update details of symbol just defined.
@i - the index of the symbol whose details need to be updated.
@address - the address of the current symbol.
@D_or_I - the type of the symbol - data or instruction line.
@type - external, relocateble, or entry symbol.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status update_symbol_details (int i, int address, char D_or_I, symbol_type type, int line_number, status s);


/*
Checking whether there are extra characters at the end of the line.
@line - the analyzed line.
@i - the index after the last desired word in the line.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status check_superfluous_text (char *line, int *i, int line_number, status s);


/*
update the addresses of the declared symbols by adding PC to the addresses of the symbols and also IC to the addresses of data words.
*/
void add_PC_to_address();

#endif
