#include "defs.h"
#include "first_scan.h"
#include "cmp_funs.h"
#include "general_funs.h"
#include "list.h"
#include "second_scan.h"

symbol *symbol_table;
int table_size;
int IC;
int DC;
node *code_image;
node *data_image;


/*
The main function of the first scan.
@file - the file after the macro retirement.
*/
void first_scan(FILE *file)
{
	int line_number=0;
	status s_file=success, s=success;
	code_image = NULL;
	data_image = NULL;
	IC = 0, DC = 0, table_size = 0;
	symbol_table = (symbol *)malloc(sizeof(symbol));
	symbol_table[table_size].name = (char *)malloc(sizeof(char));
	checking_memory("2", symbol_table , symbol_table[table_size].name);
	
	while(!feof(file))
	{
		char line[LINE_LENGTH+3];
		s = success;
		line_number++;
			
		fgets(line, LINE_LENGTH+2, file);
		if(strlen(line) > LINE_LENGTH)/*Validation for length of line:*/
		{
			s = print_error(1, line_number);
			while(fgetc(file) != '\n');
		}

		s = first_line_analyze(line, line_number, s);

		if(s == error)
			s_file = s;
		
		if (fgetc(file) == EOF)
			break;
		fseek (file, -1, SEEK_CUR);
	}
	
	fseek (file, 0, SEEK_SET);
	add_PC_to_address();
	if(s_file == success)
		second_scan(file);
	free_structures();/*Delete all structures*/
}


/*
Analyze a line from the file at a first scan.
@line - the analyzed line.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the file - success if the file is correct, and otherwise -error.
*/
status first_line_analyze (char *line, int line_number, status s)
{
	int index=0, j=0, symbol_flag=0, i=0;
	char word[LINE_LENGTH+1];
	strcpy(word,"");
	
	s = commas_check(line, &i, 0, line_number, s);
	if (line[i] == ';' || line[i] == '\n' || line[i] == '\0')/*Ignore comment and empty lines:*/
		return s;
					
	for (j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)
		word[j] = line[i];
	word[j] = '\0';
		
	if(is_symbol_declaration(word) != UNDEFINED_VALUE)
	{
		if(is_symbol_declaration(word) == RESERVED_WORD)
			s = print_error(30, line_number);
		else if(strlen(word) > SYMBOL_SIZE)/*Validation for length of symbol:*/
			s = print_error(8, line_number);
		else
		{
			symbol_flag = 1;	
			s = add_to_table(word, s, &index, line_number);/*Enter name of symbol into the table:*/
		}
		s = commas_check(line, &i, 0, line_number, s);/*Pass to the second word in the line*/
		if(line[i] == '\n')
			s = print_error(27, line_number);
		for (j=0; !is_white_char(line[i]); j++, i++)
			word[j] = line[i];
		word[j] = '\0';
	}
	
	else if(word[strlen(word)-1] == ':')
	{
		s = print_error(28, line_number);
		s = commas_check(line, &i, 0, line_number, s);/*Pass to the second word in the line*/
		if(line[i] == '\n')
			s = print_error(27, line_number);
		for (j=0; !is_white_char(line[i]); j++, i++)
			word[j] = line[i];
		word[j] = '\0';
	}
		
	s = commas_check(line, &i, 0, line_number, s);
		
	if(is_instruction(word) != UNDEFINED_VALUE)
	{
		if(symbol_flag)
			s = update_symbol_details (index, IC, 'I',relocatable, line_number, s);
		s = analyze_instruction_line(line+i,is_instruction(word), line_number, s);
	}
	else if(is_data(word) != UNDEFINED_VALUE)
	{
		if(symbol_flag)
			s = update_symbol_details (index, DC, 'D',relocatable, line_number, s);
		s = analyze_data_line(line+i, word, line_number, s);
	}
	else if(!strcmp(word,".extern") || !strcmp(word,".entry"))
		s = analyze_extern_or_entry_line(line+i, word, line_number, s);
	else if(strchr(word, ':') != NULL)
		s = print_error(34, line_number); 
	else if(!symbol_flag)
		s = print_error(4, line_number);
	return s;
}


/*
Analyze an instruction line from the file at a first scan.
@line - the analyzed line.
@opcode - the opcode of the instruction that have to be analyze
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status analyze_instruction_line(char* line, int opcode, int line_number, status s)
{
	int i=0, j, index=0;
	int operand1_coding = UNDEFINED_VALUE, operand2_coding = UNDEFINED_VALUE, operand1_addressing = UNDEFINED_VALUE, operand2_addressing = UNDEFINED_VALUE;
	char first_operand[LINE_LENGTH+1], second_operand[LINE_LENGTH+1];
	strcpy(first_operand, "");
	strcpy(second_operand, "");	
	
	for (j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)/*read first operand:*/	
		first_operand[j] = line[i];
	first_operand[j] = '\0';
	
	if(opcode == 14 || opcode == 15)
	{
		if(strcmp(first_operand, "") != 0) /*check for invalid character at the end of the line*/
			s = print_error(6,line_number);	
	}
	else if(opcode == 12)
	{
		if(strcmp(first_operand, "") == 0) /*check for missing operand*/
			s = print_error(7, line_number);	
		s = check_superfluous_text (line, &i, line_number, s);
		s = addressing_validation (&operand1_addressing, &operand1_coding, first_operand, 0, 3, 2, line_number, s);	
	}
	else if (opcode == 4 || opcode == 5 || opcode == 7 || opcode == 8 || opcode == 9 || opcode == 10 || opcode == 11 || opcode == 13)
	{
		if(strcmp(first_operand, "") == 0) /*check for missing operand*/
			s = print_error(7, line_number);	
		s = check_superfluous_text (line, &i, line_number, s);
		s = addressing_validation (&operand1_addressing, &operand1_coding, first_operand, 1, 3, 2, line_number, s);	
	}	
	else
	{
		if(strcmp(first_operand, "") == 0) /*check for missing operand*/
			s = print_error(7, line_number);
		s = commas_check(line, &i, 1, line_number, s);/*check commas and read second operand:*/
		for(j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)
			second_operand[j] = line[i];
		second_operand[j] = '\0';
		
		s = check_superfluous_text (line, &i, line_number, s);
		
		if(strcmp(second_operand, "") == 0) /*check for missing operand*/
			s = print_error(7, line_number);
			
		if(opcode == 0 || opcode == 2 || opcode == 3)
		{
			s = addressing_validation (&operand1_addressing, &operand1_coding, first_operand, 0, 3, 1, line_number, s);
			s = addressing_validation (&operand2_addressing, &operand2_coding, second_operand, 1, 3, 2, line_number, s);
		}
		else if(opcode == 1)
		{
			s = addressing_validation (&operand1_addressing, &operand1_coding, first_operand, 0, 3, 1, line_number, s);
			s = addressing_validation (&operand2_addressing, &operand2_coding, second_operand, 0, 3, 2, line_number, s);
		}
		else if(opcode == 6)
		{
			s = addressing_validation (&operand1_addressing, &operand1_coding, first_operand, 1, 2, 1, line_number, s);
			s = addressing_validation (&operand2_addressing, &operand2_coding, second_operand, 1, 3, 2, line_number, s);
		}
	}
	if(operand1_addressing == 1 || operand1_addressing == 2)
		s = add_to_table(first_operand, s, &index, line_number);
	if(operand2_addressing == 1 || operand2_addressing == 2)
		s = add_to_table(second_operand, s, &index, line_number);
		
	if(s == success)
	{
		add_first_word(opcode, operand1_addressing, operand2_addressing);
		add_info_words(operand1_addressing, operand1_coding, operand2_addressing, operand2_coding);
	}
	return s;		
}			


/*
Add symbol name to the symbol table.
@symbol_name - the symbol name.
@s - the status of the line.
@i - the relevant index in the symbol table.
@line_number - the number of the current line.
return: the status of the line - error or success.
*/
status add_to_table(char *symbol_name, status s, int *i, int line_number)
{
	if (is_struct(symbol_name) != UNDEFINED_VALUE)
		symbol_name[strlen(symbol_name)-2] = '\0';/*Delete ".1" or ".2" */
	if (is_symbol_declaration(symbol_name) != UNDEFINED_VALUE)
		symbol_name[strlen(symbol_name)-1] = '\0';/*Delete ":"*/
		
	if(strlen(symbol_name) > SYMBOL_SIZE)/*Validation for length of symbol:*/
		return print_error(8, line_number);
							
	for(*i = 0; *i < table_size ; (*i)++)
		if(strcmp(symbol_name, symbol_table[*i].name) == 0)
			break;
	if(*i == table_size)
	{		
		symbol_table[table_size].name = (char *)realloc(symbol_table[table_size].name, sizeof(char)*(strlen(symbol_name)+1));
		strcpy(symbol_table[table_size].name ,symbol_name);
		symbol_table[table_size].D_or_I = '\0';
		symbol_table[table_size].address = 0;
		symbol_table[table_size].type = unknown;
		
		symbol_table = (symbol *)realloc(symbol_table, sizeof(symbol)*((table_size)+2));
		symbol_table[(table_size)+1].name = (char *)malloc(sizeof(char));
		checking_memory("3", symbol_table[table_size].name, symbol_table, symbol_table[(table_size)+1].name);
		(table_size)++;
	}
	return s;
}			


/*
Analyze a data line from the file at the first scan
@line - the analyzed line.
@data_type - the type of the data that have to be analyze.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status analyze_data_line (char *line, char *data_type, int line_number, status s)
{
	int i=0;
	
	if(!strcmp(data_type,".data"))
		s = encode_data_arguments (line, data_type, line_number, s);
	else if(!strcmp(data_type,".string"))
		s = encode_string_arguments (line, line_number, s);
	else if(!strcmp(data_type,".struct"))
	{
		s = encode_data_arguments (line, data_type, line_number, s);
		while(!is_white_char(line[i]) && line[i] != ',')
			i++;
		s = commas_check(line, &i, 1, line_number, s);
		s = encode_string_arguments (line+i, line_number, s);
	}
	return s;
}


/*
Encode the numbers as info-words.
@line - the data line which include numbers declarrations.
@data_type - the type of the data that have to be analyze.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status encode_data_arguments (char *line, char *data_type, int line_number, status s)
{
	int i=0, j=0, number;
	char argument[LINE_LENGTH];
	strcpy(argument, "");
	
	for(j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)
		argument[j] = line[i];
	argument[j] = '\0';
	
	if(strcmp(argument,"") == 0)
		return print_error(13, line_number);
		
	while(line[i] != '\n' && line[i] != '\0' && !strcmp(data_type, ".data"))
	{
		int commas_flag = 0;
		while(line[i] == ' ' || line[i] == '\t' || line[i] == ',') /*pass over spaces and check commas*/
		{
			if(line[i] == ',')
			{
				if(commas_flag)
					s = print_error(11, line_number);
				else
					commas_flag = 1;
			}
			i++;
		}	
		if((line[i] == '\n' || line[i] == '\0') && commas_flag) /*Invalid comma after the last number*/
			s = print_error(11, line_number);
		else if ((line[i] != '\n' || line[i] != '\0') && !commas_flag) /*Missing comma between numbers*/
			s = print_error(10, line_number);
			
		number = is_number(argument);
		if(number == UNDEFINED_VALUE) /*Undefine number*/
			s = print_error(33, line_number);	
		else if(s == success)/*insert the last number into tha list*/
			add_data_word(number);
		
		for(j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)/*Get the next number*/
			argument[j] = line[i];
		argument[j] = '\0';
	}
	number = is_number(argument);	
	if(number == UNDEFINED_VALUE)/*check for invalid character at the end of the line*/
		s = print_error(33, line_number);
	else if(s == success)/*insert the last number into tha list*/
		add_data_word(number);
	return s;
}


/*
Encode the characters of the string as info-words.
@line - the data line which include string declarrations.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status encode_string_arguments (char *line, int line_number, status s)
{
	int i=1, j=1;
	char argument[LINE_LENGTH];
	strcpy(argument, "");
	
	argument[0] = line[0];
	for(j=1; line[i] != '\n' && line[i] != '"' && line[i] != '\0'; j++, i++)
		argument[j] = line[i];
	argument[j] = line[i];
	argument[j+1] = '\0';
	
	if(argument[0] == '\0' || argument[0] == '\n')
		s = print_error(18, line_number);
	else if(argument[0] != '"' || argument[strlen(argument)-1] != '"' || strlen(argument) < 2)/*string validation*/
		s = print_error(14, line_number);
		
	if(line[i] == '"')
		i++;
	s = check_superfluous_text (line, &i, line_number, s);
				
	if(s == success)/*insert the string into tha list*/
	{
		for(i = 1; i < strlen(argument)-1; i++)
			add_data_word(argument[i]);		
		add_data_word(0);/*add word for the \0 after all the string*/
	}
	return s;
}


/*
Encode the characters of the string as info-words.
@line - the line which include a statement about an external or entry symbol.
@command_type - .extern or .entry . 
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status analyze_extern_or_entry_line(char *line, char *command_type, int line_number, status s)
{
	int i=0, j=0, index=0;
	char symbol_name[SYMBOL_SIZE];
	strcpy(symbol_name,"");
	
	for(j=0; !is_white_char(line[i]); j++, i++)
		symbol_name[j] = line[i];
	symbol_name[j] = '\0';
		
	if(!strcmp(symbol_name, ""))
		s = print_error(18, line_number);
		
	else if(is_symbol(symbol_name) == UNDEFINED_VALUE)
		s = print_error(28, line_number);
		
	s = check_superfluous_text (line, &i, line_number, s);
			
	if(s == success)
	{
		s = add_to_table(symbol_name, s, &index, line_number);
		if(!strcmp(command_type, ".extern"))
			s = update_symbol_details (index, 0, '\0', external, line_number, s);
		else
			s = update_symbol_details (index, 0, '\0', entry, line_number, s);
	}
	return s;
}


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
status addressing_validation (int *operand_addressing, int *operand_code, char *operand, int start, int end, int op_type, int line_number, status s)
{
	func_ptr operand_checking[] = {&is_number_operand, &is_symbol, &is_struct, &is_register};
	
	for(*operand_addressing = 0; *operand_addressing <= 3; (*operand_addressing)++)
	{
		*operand_code = (*(operand_checking[*operand_addressing]))(operand);
		if(*operand_code != UNDEFINED_VALUE)
		{
			if(is_struct(operand) != UNDEFINED_VALUE)
				operand[strlen(operand)-2] = '\0';
			break;
		}
	}
	if(*operand_addressing < start || *operand_addressing > end)
	{
		if(op_type == 1)
			s = (*operand_code == UNDEFINED_VALUE ? print_error(25, line_number) : print_error(12, line_number));
		else
			s = (*operand_code == UNDEFINED_VALUE? print_error(26, line_number) : print_error(9, line_number));
	}
	else if(*operand_code == RESERVED_WORD)
		s = (op_type == 1 ? print_error(31, line_number) : print_error(32, line_number));
	return s;
}


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
status update_symbol_details (int i, int address, char D_or_I, symbol_type type, int line_number, status s)
{
	if((type == relocatable && symbol_table[i].type == external))
		return print_error(21, line_number);
	if(type == external && symbol_table[i].type == relocatable)
		return print_error(29, line_number);
	if(type == relocatable && symbol_table[i].type == entry && symbol_table[i].D_or_I != '\0')
		return print_error(19, line_number);
	if((type == entry && symbol_table[i].type == external) || (type == external && symbol_table[i].type == entry))
		return print_error(22, line_number);
	if(type == relocatable && symbol_table[i].type == relocatable)
		return print_error(19, line_number);
	if(type == external && symbol_table[i].type == external)
		return print_error(23, line_number);
	if(type == entry && symbol_table[i].type == entry)
		return print_error(24, line_number);
	
	symbol_table[i].address = address;
	symbol_table[i].D_or_I = D_or_I;
	if(type == relocatable && (symbol_table[i].type == entry || symbol_table[i].type == external))
		return s;
	symbol_table[i].type = type;
	return s;
}


/*
Checking whether there are extra characters at the end of the line.
@line - the analyzed line.
@i - the index after the last desired word in the line.
@line_number - the number of the current line.
@s - the status of the line.
return: the status of the line - error or success.
*/
status check_superfluous_text (char *line, int *i, int line_number, status s)
{
	s = commas_check(line, i, 0, line_number, s);
	if(line[*i] != '\n' && line[*i] != '\0')
		s = print_error(6, line_number);
	return s;
}


/*
update the addresses of the declared symbols by adding PC to the addresses of the symbols and also IC to the addresses of data words.
*/
void add_PC_to_address()
{
	int i=0;
	for(i=0; i<table_size; i++)
	{
		symbol_table[i].address += PC;
		if(symbol_table[i].D_or_I == 'D')
			symbol_table[i].address += IC;
	}
}

