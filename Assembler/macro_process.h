#ifndef MACRO_PROCESS_H
#define MACRO_PROCESS_H

/*
The main function of the macro retirement.
@source - the received source file.
@retirement - the file that will contain the code after the macro retirement.
return: the status of the file - error or success.
*/
status macro_process (FILE *source, FILE *retirement);


/*
Enter name of macro into the macro table.
@macro_table - the table that contains the macro details.
@macro_name - the name of the macro.
@line_number - the number of the current line.
@table_size - the index of the new macro.
@s - the status of the line.
@macro_flag - indicates whether within a macro definition.
return: the status of the file - success or error.
*/
status enter_name_of_macro(macro **macro_table, char **macro_name, int line_number, int table_size, status s, int *macro_flag);


/*
Update the datails of macro whose definition is finished in the macro table.
@macro_table - the table that contains the macro details.
@table_size - the index of the current macro.
@j - the end of the macro's content.
*/
void update_macro_details(macro **macro_table, int *table_size, int *j);


/*
Check the validation of a name of macro.
@macro_table - the table that contains the macro details.
@macro_name - the name of the macro.
@line_number - the number of the current line.
@table_size - the index of the current macro.
@s - the status of the line.
return: the status of the file - success or error.
*/
status macro_name_validation (macro *macro_table, char **macro_name, int line_number, int table_size, status s);


/*
Check if there is a call to macro at the beginning of the line and if not - copies the line to the file.
@line - the analyzed line.
@retirement - the file after retire macros.
@maccro_table - the table that contains the macro details.
@first_word - the first word at the current line.
@second_word - the second word at the current line.
@table_size - the number of macros appearing in the macro table.
*/
void find_macro_call(char *line, FILE *retirement, macro *macro_table, char *first_word, char *second_word, int table_size);


/*
Delete the  macro table.
@maccro_table - the table that contains the macro details.
@table_size - the size of the macro table.
*/
void delete_macro_table(macro *macro_table, int table_size);


#endif
