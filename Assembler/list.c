#include "defs.h"
#include "list.h"
#include "cmp_funs.h"
#include "general_funs.h"


extern node *code_image;/*The list of instruction line coding words*/
extern node *data_image;/*The list of data line coding words*/
extern int IC;/*The instructions words counter*/
extern int DC;/*The data words counter*/


/*Add a first word for an instruction line.
@opcode - the opcode of the instruction word.
@source - the addressing method number of the source operand.
@target - the addressing method number of the target operand.
*/
void add_first_word(int opcode, int source, int target)
{
	node *new_node  = (node*) malloc(sizeof(node));
	node *p = code_image;
	memset(new_node,0,sizeof(node));
	checking_memory("1" , new_node);
	
	if(source == UNDEFINED_VALUE)
		source = 0;
	if(target == UNDEFINED_VALUE)/*if there is just one operand, it must be target*/
	{
		target = source;
		source = 0;
	}
		
	((new_node->data).first_word).opcode = opcode;
	((new_node->data).first_word).source_adrressing = source;
	((new_node->data).first_word).target_adrressing = target;
	((new_node->data).first_word).ARE = 0;
	new_node->next = NULL;
	IC++;
	
	if(p == NULL)
		code_image = new_node;
	else
	{
		while(p->next != NULL)
			p = (node *)p->next;
		p->next = (ptr)new_node;
	}
	
}


/*Add a register word for an instruction line.
@source_register_num - the number of the source register operand.
@target_register_num - the number of the target register operand.
*/
void add_register_word(int source_register_num, int target_register_num) 
{
	node *new_node  = (node*) malloc(sizeof(node));
	node *p = code_image;
	memset(new_node,0,sizeof(node));
	checking_memory("1" , new_node);
	
	((new_node->data).registers).target_register = target_register_num;
	
	((new_node->data).registers).source_register = source_register_num;
	((new_node->data).registers).ARE = 0;
	new_node->next = NULL;
	IC++;
	
	while(p->next != NULL)
			p = (node *)p->next;
	p->next = (ptr)new_node;
	
}


/*Add a numeric code word
@num - the number to code.
@are - the ARE coding.
*/
void add_number_word(int num, int are) 
{
	node *new_node  = (node*) malloc(sizeof(node));
	node *p = code_image;
	memset(new_node,0,sizeof(node));
	checking_memory("1" , new_node);
	
	((new_node->data).number).ARE = are;
	((new_node->data).number).value = num;
	new_node->next = NULL;
	IC++;
	
	while(p->next != NULL)
		p = (node *)p->next;
	p->next = (ptr)new_node;
	
}


/*Add an address code word.
@num - the address.
@are - the ARE coding.
*/
void add_address_word(int address, int are) 
{
	node *new_node  = (node*) malloc(sizeof(node));
	node *p = code_image;
	memset(new_node,0,sizeof(node));
	checking_memory("1" , new_node);
	
	((new_node->data).address).ARE = are;
	((new_node->data).address).value = address;
	new_node->next = NULL;
	IC++;
	
	while(p->next != NULL)
		p = (node *)p->next;
	p->next = (ptr)new_node;
	
}


/*Send the necessary details th the relevant add method.
@operand1_addressing - the addressing method of the first operand.
@operand1_value - the code of the first operand.
@operand2_addressing - the addressing method of the second operand.
@operand2_value - the code of the second operand.
*/
void add_info_words(int operand1_addressing, int operand1_value, int operand2_addressing, int operand2_value)
{
	if(operand1_addressing == UNDEFINED_VALUE && operand2_addressing == UNDEFINED_VALUE)
		return;
	if(operand1_addressing == 3 && operand2_addressing == 3)/*two registers*/
	{
		add_register_word(operand1_value, operand2_value);
		return;
	}
	
	if(operand2_addressing != UNDEFINED_VALUE)
		switch(operand1_addressing)
		{
			case 0: add_number_word(operand1_value, 0);break;
			case 1: add_address_word(operand1_value, 0);break;
			case 2: {add_address_word(0, 0);
					 add_number_word(operand1_value, 0);break;}
			case 3: add_register_word(operand1_value, 0);break;
		}
		
	if(operand2_addressing == UNDEFINED_VALUE)
	{
		operand2_addressing = operand1_addressing;
		operand2_value = operand1_value;
	}	
	switch(operand2_addressing)
	{
		case 0: add_number_word(operand2_value, 0);break;
		case 1: add_address_word(operand2_value, 0);break;
		case 2: {add_address_word(0, 0);
				 add_number_word(operand2_value, 0);break;}
		case 3: add_register_word(0, operand2_value);break;
	}
}		


/*Add a data code word.
@num - the number that needs to be coded.
*/
void add_data_word(int num) 
{
	node *new_node  = (node*) malloc(sizeof(node));
	node *p = data_image;
	memset(new_node,0,sizeof(node));
	checking_memory("1" , new_node);
	
	((new_node->data).data_word).value = num;
	new_node->next = NULL;
	DC++;
	
	if(p == NULL)
		data_image = new_node;
	else
	{
		while(p->next != NULL)
			p = (node *)p->next;
		p->next = (ptr)new_node;
	}
	
}


/*Delete the instruction list and the data list of coding words.*/
void delete_lists()
{
	node *current, *next;
	current = code_image;
	while(current != NULL)
	{
		next = (node *)current->next;
		free(current);
		current = next;
	}
	
	current = data_image;
	while(current != NULL)
	{
		next = (node *)current->next;
		free(current);
		current = next;
	}
}








