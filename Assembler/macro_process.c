#include "defs.h"
#include "macro_process.h"
#include "cmp_funs.h"
#include "general_funs.h"


/*
The main function of the macro retirement.
@source - the received source file.
@retirement - the file that will contain the code after the macro retirement.
return: the status of the file - error or success.
*/
status macro_process (FILE *source, FILE *retirement)
{
	status s_file = success;
	int macro_flag = 0, i = 0, table_size = 0, j = 0, line_number = 0, k = 0;
	macro *macro_table = (macro *)malloc(sizeof(macro));
	macro_table[table_size].name = (char *)malloc(sizeof(char));
	macro_table[table_size].content = (char *)malloc(sizeof(char));
	checking_memory("3", macro_table, macro_table[table_size].content,macro_table[table_size].name); 

	while(!feof(source))
	{
		char first_word[LINE_LENGTH+1], line[LINE_LENGTH+3], *second_word;
		status s=success;
		line_number++;
		i=0;
		
		fgets(line, LINE_LENGTH, source);
		pass_spaces(line, &i);
		
		for(k=0; !is_white_char(line[i]); i++, k++)/*Get the first word from the line:*/
			first_word[k] = line[i];	
		first_word[k] = '\0';
		
		second_word = line+i;/*keep a pointer to the start of the second word:*/		
		while(*second_word == ' ' || *second_word == '\t')
			second_word++;
			
		if(!macro_flag)
			
			if(strcmp(first_word, "macro") == 0)
				s = enter_name_of_macro(&macro_table, &second_word, line_number, table_size, s, &macro_flag);
			
			else
				find_macro_call(line, retirement, macro_table, first_word, second_word, table_size);
		else
			if(!strcmp(first_word, "endmacro"))
			{
				macro_flag = 0;
				if(*second_word != '\n')
					s = print_error(3,line_number);
				else
					update_macro_details(&macro_table, &table_size, &j);
			}
			else/* copy into the content of macro*/
				for (i=0; line[i] != '\0'; i++, j++)
				{
					macro_table[table_size].content[j] = line[i];
					macro_table[table_size].content = (char *)realloc(macro_table[table_size].content, (j+2)*sizeof(char));
					checking_memory("1",macro_table[table_size].content);
				}	
		if (s == error)
			s_file = s;
		
		if (fgetc(source) == EOF)
			break;
		fseek (source, -1, SEEK_CUR);
	}
	fseek(retirement, 0, SEEK_SET);
	delete_macro_table(macro_table, table_size);	
	return s_file;
}


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
status enter_name_of_macro(macro **macro_table, char **macro_name, int line_number, int table_size, status s, int *macro_flag)
{
	s = macro_name_validation (*macro_table, macro_name, line_number, table_size, s);		
	if (s == success)
	{	 
		(*macro_table)[table_size].name = (char *)realloc((*macro_table)[table_size].name, sizeof(char)*(strlen(*macro_name)+1));
		checking_memory("1", (*macro_table)[table_size].name);	
		strcpy((*macro_table)[table_size].name ,*macro_name);
		*macro_flag=1;
	}
	return s;
}


/*
Update the datails of macro whose definition is finished in the macro table.
@macro_table - the table that contains the macro details.
@table_size - the index of the current macro.
@j - the end of the macro's content.
*/
void update_macro_details(macro **macro_table, int *table_size, int *j)
{
	((*macro_table)[*table_size]).content[*j] = '\0';
	(*table_size)++;
	*j=0;
	(*macro_table) = (macro *)realloc((*macro_table), sizeof(macro)*(*table_size+1));
	(*macro_table)[*table_size].name = (char *)malloc(sizeof(char));
	(*macro_table)[*table_size].content = (char *)malloc(sizeof(char));
	checking_memory("3", *macro_table, (*macro_table)[*table_size].name, (*macro_table)[*table_size].content);
}


/*
Check the validation of a name of macro.
@macro_table - the table that contains the macro details.
@macro_name - the name of the macro.
@line_number - the number of the current line.
@table_size - the index of the current macro.
@s - the status of the line.
return: the status of the file - success or error.
*/
status macro_name_validation (macro *macro_table, char **macro_name, int line_number, int table_size, status s)
{
	char *after_macro_name;
	int i=0;
	
	if((**macro_name) == '\n') /*if there isn't macro name after declaration*/
		s = print_error(2,line_number);
	
	for(i=0; !is_white_char((*macro_name)[i]); i++);
	(*macro_name)[i] = '\0';
	
	if((*macro_name)[i] != '\n')
	{
		after_macro_name = (*macro_name)+i+1;
		for(; *after_macro_name == ' ' || *after_macro_name == '\t'; after_macro_name++); 			
					
		if(*after_macro_name != '\n' && *after_macro_name != '\0') /*if there is more word after name of macro*/
			s = print_error(5,line_number);		
	}
	
	if (is_symbol(*macro_name) == UNDEFINED_VALUE || is_symbol(*macro_name) == RESERVED_WORD) /*if macro name contains invalid characters*/
		s = print_error(15,line_number);				
	
	for(i=0; i < table_size; i++)
		if(strcmp((*macro_name),macro_table[i].name) == 0)
		{
			s = print_error(16,line_number);				
			break;
		}		
	return s;
}


/*
Check if there is a call to macro at the beginning of the line and if not - copies the line to the file.
@line - the analyzed line.
@retirement - the file after retire macros.
@maccro_table - the table that contains the macro details.
@first_word - the first word at the current line.
@second_word - the second word at the current line.
@table_size - the number of macros appearing in the macro table.
*/
void find_macro_call(char *line, FILE *retirement, macro *macro_table, char *first_word, char *second_word, int table_size)
{
	int i=0;
	for(i=0; i<table_size && 0<table_size; i++)/*Check if ther is a call to a macro:*/
		if(strcmp(first_word,macro_table[i].name) == 0)
		{
			if(*second_word != '\n' && *second_word != '\0')
				fputs(line, retirement);
			else
				fputs(macro_table[i].content, retirement);
			break;
		}	
	if(i == table_size || table_size == 0)/*regular line. just copy into the new file:*/
		fputs(line, retirement);
}


/*
Delete the  macro table.
@maccro_table - the table that contains the macro details.
@table_size - the size of the macro table.
*/
void delete_macro_table(macro *macro_table, int table_size)
{
	int i;
	for (i=0; i<=table_size; i++)
	{
		free(macro_table[i].name);
		free(macro_table[i].content);
	}
	free(macro_table);
}
