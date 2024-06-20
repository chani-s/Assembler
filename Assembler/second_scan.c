#include "defs.h"
#include "second_scan.h"
#include "cmp_funs.h"
#include "general_funs.h"
#include "list.h"

extern symbol *symbol_table; /*Can we pass it to the header please???*/
extern int table_size;
extern node *code_image;
extern node *data_image;
extern int IC;
extern int DC;
extern char *file_name;


/*
The main function of the second scan
@file - the file after the macro retirement.
*/
void second_scan(FILE *file)
{
	int entry_flag=0, line_number=0, i=0, ext_counter=0;
	status s_file=success, s=success;
	externals *ext_table = (externals *)malloc(sizeof(externals));
	checking_memory("1", ext_table);	
	IC=0;
	
	while(!feof(file))
	{
		char line[LINE_LENGTH+3];
		s=success;
		line_number++;
		
		fgets(line, LINE_LENGTH+2, file);
		
		/*Ignore comment and empty lines:*/
		if (line[i] == ';' || line[i] == '\n')
			continue;	
			
		s = second_line_analyze (line, &entry_flag, &ext_table, &ext_counter, line_number, s);	
		if(s == error)
			s_file = s;
		
		if (fgetc(file) == EOF)
			break;
		fseek (file, -1, SEEK_CUR);
	}
	if(s_file == success && (code_image != NULL || data_image != NULL))
		create_files (entry_flag, ext_table, ext_counter);
	else
		free(ext_table);
}						


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
status second_line_analyze (char *line, int *entry_flag, externals **ext_table, int *ext_counter, int line_number, status s)
{
	char word[LINE_LENGTH+1];
	int j=0, i=0, register_flag=0;
	
	pass_spaces(line, &i);
	if(line[i] == '\n' || line[i] == '\0')
		return s;
				
	for(j=0; !is_white_char(line[i]); j++, i++)
		word[j] = line[i];
	word[j] = '\0';
		
	if(is_symbol_declaration(word) != UNDEFINED_VALUE)/*bypass declaration of a symbol*/
	{
		pass_spaces(line, &i);	
		for (j=0; !is_white_char(line[i]); j++, i++)
			word[j] = line[i];
		word[j] = '\0';
	}
	if(is_data(word) != UNDEFINED_VALUE || !strcmp(word, ".extern"))
		return s;
	if(strcmp(word, ".entry") == 0)
	{
		(*entry_flag) = 1;
		pass_spaces(line, &i);
		for (j=0; !is_white_char(line[i]); j++, i++)
			word[j] = line[i];
		word[j] = '\0';
		s = update_symbol_address (word, line_number, s, ext_table, ext_counter, 1/****/);
		return s;
	}
	IC++; /*count first word code*/
	
	pass_spaces(line, &i);	
	if(line[i] == '\n' || line[i] == '\0')
		return s;
		
	for(j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)
		word[j] = line[i];
	word[j] = '\0';
			
	if(is_struct(word) != UNDEFINED_VALUE || is_symbol(word) != UNDEFINED_VALUE)
		s = update_symbol_address (word, line_number, s, ext_table, ext_counter, 0/****/);
	
	IC++; /*count first operand code*/
	if (is_register(word) != UNDEFINED_VALUE)
		register_flag=1;
	
	pass_spaces(line, &i);		
	if(line[i] == '\n' || line[i] == '\0')
		return s;		
		
	for(j=0; !is_white_char(line[i]) && line[i] != ','; j++, i++)
		word[j] = line[i];
	word[j] = '\0';
		
	if(is_struct(word) != UNDEFINED_VALUE || is_symbol(word) != UNDEFINED_VALUE)
		s = update_symbol_address (word, line_number, s, ext_table, ext_counter, 0/****/);
	if(!register_flag || is_register(word) == UNDEFINED_VALUE)
		IC++; /*count second operand code*/
	return s;
}


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
status update_symbol_address (char *word, int line_number, status s, externals **ext_table, int *ext_counter, int entry_declare)
{
	int j=0, struct_flag=0;
	node *p =  code_image;
	for(j=0; j<IC; j++)
		p = (node *)p->next;
	if(is_struct(word) != UNDEFINED_VALUE)
	{
		struct_flag = 1;
		word[strlen(word)-2] = '\0';
	}
	for(j=0; j<table_size; j++)
	{
		if(strcmp(symbol_table[j].name, word) == 0)
		{
			/*if symbol not declared: */
			if(symbol_table[j].type == unknown || (symbol_table[j].type == entry && symbol_table[j].D_or_I == '\0'))
				s = print_error(20, line_number);
			/*if symbol is external: */
			else if(symbol_table[j].type == external)
			{
				((p->data).address).ARE = 1;
				update_ext_table(word, ext_table , *ext_counter);
				(*ext_counter)++;
			}
			/*if symbol is regular: */
			else if(entry_declare != 1)
			{
				((p->data).address).ARE = 2;
				((p->data).address).value = symbol_table[j].address;
			}
			break;
		}
	}
	if(struct_flag)
		IC++; /*go after field word*/	
	return s;
}


/*
Enter the updated addresses into the list.
@word - the name of the updated symbol.
@line_number - the number of the current line.
@s - the status of the line.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
return: the status of the file - success if the file is correct, and otherwise -error.
*/
void create_files (int entry_flag, externals *ext_table, int ext_counter)
{
	FILE *ob_file;	
	
	ob_file = open_file(".ob", "w+");
	fputc(' ', ob_file);
	ob_file = convert_to_32 (ob_file, IC & LOWER_BITS_LIT, (IC & HIGHER_BITS_LIT)>>5);
	fputc(' ', ob_file);
	ob_file = convert_to_32 (ob_file, DC & LOWER_BITS_LIT, (DC & HIGHER_BITS_LIT)>>5);
	fputc('\n', ob_file);
	ob_file = create_ob_file(ob_file, code_image, PC);
	ob_file = create_ob_file(ob_file, data_image, PC+IC);
	
	if(entry_flag)
		create_entry_file();
	if(ext_counter > 0)
		create_extern_file(ext_table, ext_counter);
	else
		free(ext_table);
	fclose(ob_file);
}


/*
Creates a file in which the names of the entry files and their addresses are written. 
*/
void create_entry_file ()
{
	int i, address;
	FILE *entry_file = open_file(".ent", "w");
	
	for(i=0; i<table_size; i++)
		if(symbol_table[i].type == entry)
		{
			fprintf(entry_file, "%s", symbol_table[i].name);
			fputc('\t', entry_file);
			address = symbol_table[i].address;
			entry_file = convert_to_32 (entry_file, address & LOWER_BITS_LIT, (address & HIGHER_BITS_LIT)>>5);
			fputc('\n', entry_file);
		}
	fclose(entry_file);
}


/*
Updates the table of externals wherever the name of an external symbol appears in the file
@symbol_name - the name of the updated external symbol.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
*/
void update_ext_table(char *symbol_name, externals **ext_table, int ext_counter)
{
	*ext_table = (externals *)realloc(*ext_table, (ext_counter+2) * sizeof(externals));
	checking_memory("1", *ext_table);
	strcpy((*ext_table)[ext_counter].name, symbol_name);
	(*ext_table)[ext_counter].address = IC + PC;
	
}


/*
Creates a file in which the names of the external files and the locations of their occurrences in the file.
@ext_table - a table that contains details about external symbols.
@ext_counter - the number of appearances of external symbols in the file.
*/
void create_extern_file (externals *ext_table , int ext_counter)
{
	int i, address;
	FILE *extern_file = open_file(".ext", "w");
	
	for(i = 0; i < ext_counter; i++)
	{
		fprintf(extern_file, "%s", ext_table[i].name);
		fputc('\t', extern_file);
		address = ext_table[i].address;
		extern_file = convert_to_32 (extern_file, address & LOWER_BITS_LIT, (address & HIGHER_BITS_LIT)>>5);
		fputc('\n', extern_file);
	}
	fclose(extern_file);
	free(ext_table);
}


/*
Creates a file in which the names of the external files and the locations of their occurrences in the file.
@ob_file - the output file which includes the encoding corresponding to the source file in base 32
@info_word - the code word that should be written in base 32.
@address - the location of the code word in the memory.
*/
FILE * create_ob_file (FILE *ob_file, node *info_word, int address)
{
	while(info_word != NULL)
	{
		unsigned int *ptr = (unsigned int *)(&(info_word->data));
		
		ob_file = convert_to_32 (ob_file, address & LOWER_BITS_LIT, (address & HIGHER_BITS_LIT)>>5);
		fputc('\t', ob_file);
		
		ob_file = convert_to_32 (ob_file, (*ptr) & LOWER_BITS_LIT, ((*ptr) & HIGHER_BITS_LIT)>>5);
		fputc('\n', ob_file);
		
		info_word = (node *)info_word->next;
		address++;
	}
	return ob_file;
}


/*
Convert an encoding to base 32 and write into the relevant file.
@file - the file in which base 32 code should be written.
@right_number - the number represented by the right bits of the code word
@left_number - the number represented by the left bits of the code word
*/
FILE * convert_to_32 (FILE *file, int right_number, int left_number)
{
	char digits[] = {'!','@','#','$','%','^','&','*','<','>',
			'a','b','c','d','e','f','g','h','i','j','k','l',
			'm','n','o','p','q','r','s','t','u','v'};
	char right_char, left_char;
	
	right_char = digits[right_number];
	left_char = digits[left_number];
		
	fputc(left_char, file);
	fputc(right_char, file);
	
	return file;
}


/*
Delete data image, code image and also the symbol table. 
*/
void free_structures ()
{
	int i;
	delete_lists();
	for (i=0; i<=table_size; i++)
		free(symbol_table[i].name);
	free(symbol_table);
}
