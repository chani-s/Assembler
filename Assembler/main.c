#include "defs.h"
#include "macro_process.h"
#include "general_funs.h"
#include "first_scan.h"

char file_name[MAX_FILE_NAME];
	
int main(int argc , char* argv[])
{
	FILE *source, *retirement;
	status s = success;
	int i;

	if(argc == 1)
	{
		printf("\nYou forgot to enter a name of file. please enter './run' and add the name of the file you want to compile.\n"); 
		exit(0);
	}

	for(i = 1; i< argc; i++)
	{
		strcpy(file_name, argv[i]);
		source = open_file (".as", "r");
		retirement = open_file (".am", "w+");
		
		s = macro_process(source, retirement);
		fclose (source);
		
		/*Starting to scan the file:*/
		if (s == success)
			first_scan(retirement);
		fclose(retirement);
	}
	return 0;
}


