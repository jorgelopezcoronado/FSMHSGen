#include "FSM.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

int yyparse (fsm_ll **machine, yyscan_t scan);

fsm_arr *getFSM (char *source)
{
	fsm_ll *machine = create_fsm_ll(); //root of AST
	fsm_arr *fsm; // returning data struct

	yyscan_t scanner;
	YY_BUFFER_STATE state;

	if (yylex_init(&scanner)) 
	{
		//couldn't initialize
		return NULL;
	}


	state = yy_scan_string(source, scanner); //tokenize source_code
	if (yyparse(&machine, scanner)) //retreive the AST from parser using tokenize string
	{
		//error parsing not your issue
		return NULL;
	}
        
	yy_delete_buffer(state, scanner);
	yylex_destroy(scanner);

	fsm = fsm_ll_to_fsm_arr(machine);
//	delete_fsm_ll(machine);

	return fsm; 
}

char *textFromFile(char *filename)
{
	FILE *file;
	size_t fileSize;
	char *text;
		

	file = fopen(filename, "rb");
	if (!file)
	{
		printf("Error reading file %s!\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END); 
	fileSize = ftell(file); // what's the position of the pointer in the end of this stream?
	rewind(file);
	
	text = (char*)malloc(fileSize + 1); //one mor to null terminate the string

	if(!text)
	{
		//failed to allocate text
		return NULL;
	}
	
	if(!fread(text, 1, fileSize, file))
	{
		printf("Error! Unable to read the file %s!", filename);
		free(text);
		return NULL;
	}

	fclose(file);
	
	return text;

}

loglevel log_level;
logtype log_type;
const char *log_name;

int main(int argc, char **argv)
{
	char *input;
	fsm_arr *fsm;
	
	log_level = error;
	log_type = none;
	log_name = "FSMHSGen.log"; 

	if(argc < 2)
	{
		fsm_log(error, "Error! an FSM filename analyize is expected!\n");
		exit(1);
	}
	
	log_type = syslog_log;	
		
	input = textFromFile(argv[1]);

	if(!input)
	{	
		//no input from file
		exit(1);
	}

	fsm = getFSM(input);
	
	free(input);
	
	if(!fsm)
	{
		fsm_log(error, "Error! Parse error in the file!");
		exit(1);
	}

	print_fsm_arr(fsm);
	delete_fsm_arr(fsm);
	
	return 0;
}
