
#include "FSM_Gedanken_experiments.h"
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
	delete_fsm_ll(machine);

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

//Global variables to hold the tool configuration.
loglevel log_level;
logtype log_type;
const char *log_name;
size_t max_time;
size_t max_mem;
size_t max_length;
char *filename;	

void print_help(char *progname)
{
	printf("Usage %s: [-h|--help|--h] [-ml max_length] [-mt max_time] [-mm max_mem] [-ll log_level] [-lt log_type] [-cf config_file] <fsm_file>\n", progname);
	printf("\t\t-h\n\t\t\tPrints this message, the program help.\n");
	printf("\t\t-ml max_length [default=0]\n\t\t\tThe maximal length of the homing sequence.\n\t\t\tIntepreted as a number. 0 = no limit.\n");
	printf("\t\t-mt max_time [default=0]\n\t\t\tThe maximal time allowed to spend to obtain the homing sequences.\n\t\t\tIntepreted as a number. 0 = no limit.\n");
	printf("\t\t-mm max_mem [default=0]\n\t\t\tThe maximal memory allowed to allocate to obtain the homing sequences.\n\t\t\tIntepreted as a number. 0 = no limit.\n");
	printf("\t\t-ll log_level [default=3]\n\t\t\tSpecifies the verbosity on the logged output.\n\t\t\tIntepreted as a number.\n\t\t\t0 = Emergency\n\t\t\t1 = Alert\n\t\t\t2 = Critical\n\t\t\t3 = Error\n\t\t\t4 = Warning\n\t\t\t5 = Notice\n\t\t\t6 = Info\n\t\t\t7 = Debug\n");
	printf("\t\t-lt log_type [default=local]\n\t\t\tSpecifies logging style.\n\t\t\tlocal = log to local filename \"FSMHSGen.log\"\n\t\t\tsyslog = log using standard syslog facility, local7\n");
	printf("\t\t-cf config_file\n\t\t\tSpecifies configuration file.\n\t\t\tConfiguration will be overwritten by the values in the file, and this values will be overwritten if future flags are read.\n");
	printf("\t\tfsm_file\n\t\t\tSpecifies FSM to analyze in the .fsm file format.\n");
}

unsigned char is_int (char *string)
{
	size_t i = 0;
	while (string[i])
		if(string[i] <= 47 || string[i++] >=58)
			return 0;
	return 1;
}

/*quick and dirty implementation*/
void parse_file(char *filename)
{
	void parse_args(int argc, char**argv);
	char **arguments = NULL;
	char *text = NULL, text_ptr;
	int size = 0;
	size_t text_index = 0;

	text = textFromFile(filename);
	
	if(!text)
		return;

	text_ptr = text;

	while(*)

	parse_args(size, arguments);
		
	free(text);
	
}

void parse_args(int argc, char **argv)
{
	size_t i = 1;
	while(i < argc)
	{
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || !strcmp(argv[i], "--h"))
		{
			print_help(argv[0]);
			exit(0);
		}
		else if(!strcmp(argv[i], "-ml"))
		{
			if(i != argc - 1)	
			{
				if(!is_int(argv[++i]))
				{
					char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -ml option, %s is not a positive number\n", argv[i]) + 1));
					sprintf(msg, "Error! expected number after -ml option, %s is not a positive number\n", argv[i]);
					fsm_log(error, msg);               
					free(msg);
					print_help(argv[0]);
					exit(1);
				}
				sscanf(argv[i],  "%llu", &max_length);
			}
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -ml option\n") + 1));
				sprintf(msg, "Error! expected number after -ml option\n");
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		else if(!strcmp(argv[i], "-mt"))
		{
			if(i != argc - 1)	
			{
				if(!is_int(argv[++i]))
				{
					char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -mt option, %s is not a positive number\n", argv[i]) + 1));
					sprintf(msg, "Error! expected number after -mt option, %s is not a positive number\n", argv[i]);
					fsm_log(error, msg);               
					free(msg);
					print_help(argv[0]);
					exit(1);
				}
				sscanf(argv[i],  "%llu", &max_time);
			}
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -mt option\n") + 1));
				sprintf(msg, "Error! expected number after -mt option\n");
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		else if(!strcmp(argv[i], "-mm"))
		{
			if(i != argc - 1)	
			{
				if(!is_int(argv[++i]))
				{
					char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -mm option, %s is not a positive number\n", argv[i]) + 1));
					sprintf(msg, "Error! expected number after -mm option, %s is not a positive number\n", argv[i]);
					fsm_log(error, msg);               
					free(msg);
					print_help(argv[0]);
					exit(1);
				}
				sscanf(argv[i],  "%llu", &max_mem);
			}
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -mm option\n") + 1));
				sprintf(msg, "Error! expected number after -mm option\n");
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		else if(!strcmp(argv[i], "-ll"))
		{
			if(i != argc - 1)	
			{
				if(!is_int(argv[++i]))
				{
					char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -ll option, %s is not a positive number\n", argv[i]) + 1));
					sprintf(msg, "Error! expected number after -ll option, %s is not a positive number\n", argv[i]);
					fsm_log(error, msg);               
					free(msg);
					print_help(argv[0]);
					exit(1);
				}
				sscanf(argv[i],  "%llu", &log_level);
				if(log_level > debug)
				{
					char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! log level: %s is greater than 7 (debug)\n", argv[i]) + 1));
					sprintf(msg, "Error! log level: %s is greater than 7 (debug)\n", argv[i]);
					fsm_log(error, msg);               
					free(msg);
					print_help(argv[0]);
					exit(1);
				}
			}
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected number after -ll option\n") + 1));
				sprintf(msg, "Error! expected number after -ll option\n");
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		else if(!strcmp(argv[i], "-lt"))
		{
			if(i != argc - 1)	
			{
				if(!strcmp(argv[++i], "local") && !strcmp(argv[i], "syslog"))
				{
					char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected value after -lt option, \"%s\" is not in (local | syslog)\n", argv[i]) + 1));
					sprintf(msg, "Error! expected value after -lt option, \"%s\" is not in (local | syslog)\n", argv[i]);
					fsm_log(error, msg);               
					free(msg);
					print_help(argv[0]);
					exit(1);
				}
				log_type = (!strcmp(argv[i], "local"))?local:syslog_log;
			}
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected string after -lt option\n") + 1));
				sprintf(msg, "Error! expected string after -lt option\n");
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		else if(!strcmp(argv[i], "-cf"))
		{
			if(i != argc - 1)	
			{
				//call func to process the cf file	
			}
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! expected string after -cf option\n") + 1));
				sprintf(msg, "Error! expected string after -cf option\n");
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		else
		{
			if(i == argc - 1)
				filename = argv[i];
			else
			{
				char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! unrecognized option: %s\n", argv[i]) + 1));
				sprintf(msg, "Error! unrecognized option: %s\n", argv[i]);
				fsm_log(error, msg);               
				free(msg);
				print_help(argv[0]);
				exit(1);
			}
		}
		i++;
	}
	if(!strcmp(filename, ""))
	{
		fsm_log(error, "Error! an FSM filename analyize is expected!\n");
		print_help(argv[0]);
		exit(1);
	}
}

void hs(fsm_arr *fsm)
{
	integer_set *initial_states = create_integer_set();
	size_t j;
	if(!initial_states)
	{
		fsm_log(error, "Error! hs: could not allocate initial states space\n");
		exit(1);
	}
	
	//for (j = 0; j < fsm->maxS; j++)
	for (j = 0; j < 4; j++)
		integer_set_add(initial_states, j);
	
	display_hs(fsm, initial_states, stdout);
	return;
}


int main(int argc, char **argv)
{
	char *input;
	fsm_arr *fsm;
	
	log_level = error;
	log_type = none;
	log_name = "FSMHSGen.log"; 
	max_time = 0;
	max_mem = 0;
	max_length = 0;
	filename = "";

	parse_args(argc, argv);
	
//	if(log_type == none)
//		log_type = local;

	input = textFromFile(filename);

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

	//print_fsm_arr(fsm);

	hs(fsm);
	
	delete_fsm_arr(fsm);

	return 0;
}
