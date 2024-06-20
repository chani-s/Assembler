#ifndef LIST_H
#define LIST_H


/*Add a first word for an instruction line.
@opcode - the opcode of the instruction word.
@source - the addressing method number of the source operand.
@target - the addressing method number of the target operand.
*/
void add_first_word(int op, int source, int target);


/*Add a register word for an instruction line.
@source_register_num - the number of the source register operand.
@target_register_num - the number of the target register operand.
*/
void add_register_word(int source_register_num, int target_register_num);


/*Add a numeric code word
@num - the number to code.
@are - the ARE coding.
*/
void add_number_word(int num, int are);


/*Add an address code word.
@num - the address.
@are - the ARE coding.
*/
void add_address_word(int address, int are);


/*Send the necessary details th the relevant add method.
@operand1_addressing - the addressing method of the first operand.
@operand1_value - the code of the first operand.
@operand2_addressing - the addressing method of the second operand.
@operand2_value - the code of the second operand.
*/
void add_info_words(int operand1_addressing, int operand1_value, int operand2_addressing, int operand2_value);


/*Add a data code word.
@num - the number that needs to be coded.
*/
void add_data_word(int num);


/*Delete the instruction list and the data list of coding words.*/
void delete_lists();


#endif
