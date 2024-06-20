#include "defs.h"
#include "general_funs.h"

extern char file_name[];

/*
Open new file
@ending - extension of the file being opened.
@format - the opening format of the file.
returns: a pointer to the beginning of the created file.
*/
FILE * open_file (char *ending, char *format)
{
	char full_name[MAX_FILE_NAME+3];
	FILE *f;
	
	strcpy(full_name, file_name); 
	strcat(full_name, ending);
	
	f = fopen(full_name, format);
	if (!f)
	{
		printf("File not exist.\n");
		exit(0);
	}
	return f;
}


/*
Bypass white characters in the file
@p - a pointer of file.
@line_number - the number of the current line.
return: a pointer to the first non-white character.
*/
FILE* white_spaces(FILE *p, int *line_number)
{
	char c = fgetc(p);
	while(c == ' ' || c == '\t' || c == '\n')
	{
		if (c == '\n')
			(*line_number)++;
		c = fgetc(p);
	}
	fseek(p, -1, SEEK_CUR);
	return p;
}


/*
Checks whether a character is a white character or not.
@c - a character.
return: 0 if c is non-white character or 1 if is white character.
*/
int is_white_char(char c)
{
	if(c != ' ' && c != '\n' && c != '\t' && c != '\0' && c != EOF)
		return 0;
	return 1;
}	


/*
Print error messages.
@error num - the number of the error that was found.
@line_number - the number of the current line.
return: error status.
*/
status print_error(int error_num, int line_number)
{
	printf("%s%s%s ", COLOR_BOLD, COLOR_RED, "error in file");
	printf("%s%s", COLOR_YELLOW, file_name);
	if (error_num == 2 || error_num == 3 || error_num == 5 || error_num == 15 || error_num == 16)
		printf(".as");
	else
		printf(".am");
	printf("%s%s", COLOR_RED, " line ");
	printf("%s%d", COLOR_YELLOW, line_number);
	printf("%s%s%s%s", COLOR_RED, ": ", COLOR_OFF, BOLD_OFF);
	switch(error_num)
	{
		case 1:  printf("line is too long.\n");break;
		case 2:  printf("missing name of macro.\n");break;
		case 3:  printf("invalid characters after endmacro.\n");break;
		case 4:  printf("undefined first word in line.\n");break;
		case 5:  printf("invalid characters after declaration of macro.\n");break;
		case 6:  printf("invalid characters at the end of the line.\n");break;
		case 7:  printf("missing operand.\n");break;
		case 8:  printf("too long label name.\n");break;
		case 9:  printf("invalid addressing method (target operand).\n");break;
		case 10: printf("missing comma.\n");break;
		case 11: printf("superfluous commas.\n");break;
		case 12: printf("invalid addressing method (source operand).\n");break;
		case 13: printf("missing arguments.\n");break;
		case 14: printf("Invalid string argument.\n");break;
		case 15: printf("invalid macro name.\n");break;
		case 16: printf("macro name is already in use.\n");break;
		case 17: printf("invalid comma.\n");break;
		case 18: printf("missing argument.\n");break;
		case 19: printf("symbol is already declared.\n");break;/**/
		case 20: printf("symbol undeclared.\n");break;
		case 21: printf("external symbol cannot be declared as relocatable.\n");break;
		case 22: printf("symbol cannot be declared as both external and entry.\n");break;
		case 23: printf("symbol cannot be declared twice as external.\n");break;
		case 24: printf("symbol cannot be declared twice as entry.\n");break;		
		case 25: printf("unknown source operand.\n");break;/**/
		case 26: printf("unknown target operand.\n");break;/**/
		case 27: printf("empty symbol.\n");break;/**/
		case 28: printf("Invalid symbol name.\n");break;/**/
		case 29: printf("relocatable symbol cannot be declared as external.\n");break;
		case 30: printf("symbol name can not be a reserved word.\n");break;
		case 31: printf("symbol name can not be a reserved word (source operand).\n");break;
		case 32: printf("symbol name can not be a reserved word (target operand).\n");break;	
		case 33: printf("Invalid numeric argument.\n");break;
		case 34: printf("missing space after symbol declaration.\n");break;
	}
	return error;
}


/*
Checks whether there is a proper number of commas between words, and print error message if not.
@p - a string.
@i - the index of the string after the word.
@correct_commas - the desired number of commas.
@line_number - the number of the current line.
@s - the current status of the file.
return: the status of the file.
*/
status commas_check(char *p, int *i,  int correct_commas, int line_number, status s)
{
	int commas_counter = 0;
	
	for(; p[(*i)] == ' ' || p[(*i)] == '\t' || p[(*i)] == ','; (*i)++)
		if(p[(*i)] == ',')
			commas_counter++;
	
	if (commas_counter < correct_commas)
		s = print_error (10, line_number);
	if (commas_counter > correct_commas)
	{
		if(correct_commas == 0)
			s = print_error (17, line_number);
		else
			s = print_error (11, line_number);
	}
	return s;
}


/*
print error message and exit the program if allocation failed due to lack of memory.
@args_num - the number of dynamic allocations made (as a string).
@... - the dynamically allocated structures.
*/
void checking_memory(char* args_num, ...)
{
	int num;
	va_list p;
	va_start(p, args_num);
	num = atoi(args_num);
	if( num == 0)
		return;
	while(--num)
		if(!va_arg(p, char*))
		{
			printf("\n--Error: no memory. allocation failed--\n\n");
			exit(0);
		}
	va_end(p);
}


/*
Bypass white characters.
@p - a string.
@i - the index of the string after the last word.
*/
void pass_spaces(char *p, int *i)
{
	while (p[(*i)] == ' ' || p[(*i)] == '\t' || p[(*i)] == ',') 
		(*i)++;
}
