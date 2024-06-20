#ifndef SECOND_SCAN_H
#define SECOND_SCAN_H


/*
The main function of the second scan
@file - the file after the macro retirement.
*/
void second_scan(FILE *file);


/*
Analyze a line from the file at a second scan.
@line - the analyzed line.
@entry_flag - indicates whether there are entry symbols.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the file - success if the file is correct, and otherwise -error.
*/
status second_line_analyze (char *line, int *entry_flag, externals **ext_table, int *ext_counter, int line_number, status s);


/*
Enter the updated addresses into the list.
@word - the name of the updated symbol.
@line_number - the number of the current line.
@s - the status of the line.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
@entry_declare - ensure that in case of entry declaration only a validation check of the symbol will be performed without changes at the code image.
return: the status of the file - success if the file is correct, and otherwise -error.
*/
status update_symbol_address (char *word, int line_number, status s, externals **ext_table, int *ext_counter, int entry_declare);


/*
Create run files include files of externals and entrys if there are.
@entry_flag - indicates whether there are entry symbols.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
return: the status of the file - success if the file is correct, and otherwise -error.
*/
void create_files (int entry_flag, externals *ext_table, int ext_counter);


/*
Creates a file in which the names of the entry files and their addresses are written. 
*/
void create_entry_file();


/*
Creates a file in which the names of the external files and the locations of their occurrences in the file.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
*/
void create_extern_file (externals *ext_table , int ext_counter);


/*
Updates the table of externals wherever the name of an external symbol appears in the file
@symbol_name - the name of the updated external symbol.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
*/
void update_ext_table(char *symbol_name, externals **ext_table, int ext_counter);


/*
Creates a file in which the names of the external files and the locations of their occurrences in the file.
@ob_file - the output file which includes the encoding corresponding to the source file in base 32
@info_word - the code word that should be written in base 32.
@address - the location of the code word in the memory.
*/
FILE * create_ob_file (FILE *ob_file, node *info_word, int address);


/*
Convert an encoding to base 32 and write into the relevant file.
@file - the file in which base 32 code should be written.
@right_number - the number represented by the right bits of the code word
@left_number - the number represented by the left bits of the code word
*/
FILE * convert_to_32 (FILE *file, int right_number, int left_number);


/*
Delete data image, code image and also the symbol table. 
*/
void free_structures ();


#endif
