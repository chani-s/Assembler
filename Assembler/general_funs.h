#ifndef GENERAL_FUNS_H
#define GENERAL_FUNS_H

/*
Open new file
@ending - extension of the file being opened.
@format - the opening format of the file.
returns: a pointer to the beginning of the created file.
*/
FILE * open_file (char *ending, char *format);


/*
Bypass white characters in the file
@p - a pointer of file.
@line_number - the number of the current line.
return: a pointer to the first non-white character.
*/
FILE* white_spaces(FILE *p, int *line_number);


/*
Checks whether a character is a white character or not.
@c - a character.
return: 0 if c is non-white character or 1 if is white character.
*/
int is_white_char(char c);


/*
Print error messages.
@error num - the number of the error that was found.
@line_number - the number of the current line.
return: error status.
*/
status print_error(int error_num, int line_number);


/*
Checks whether there is a proper number of commas between words, and print error message if not.
@p - a string.
@i - the index of the string after the word.
@correct_commas - the desired number of commas.
@line_number - the number of the current line.
@s - the current status of the file.
return: the status of the file.
*/
status commas_check(char *p, int *i,  int correct_commas, int line_number, status s);


/*
print error message and exit the program if allocation failed due to lack of memory.
@args_num - the number of dynamic allocations made (as a string).
@... - the dynamically allocated structures.
*/
void checking_memory(char* args_num, ...);


/*
Bypass white characters.
@p - a string.
@i - the index of the string after the last word.
*/
void pass_spaces(char *p, int *i);


#endif
