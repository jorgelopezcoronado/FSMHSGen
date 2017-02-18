/*
 *
 * FSM_Gedanken_experiments.c: implemented gedanken experiments over an FSM data structure
 *
*/

#include "FSM_Gedanken_experiments.h"
#include "helpers.h"

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

//extern variables to control the truncating rule of the tree generation 
//extern size_t max_time;
//extern size_t max_mem;
//extern size_t max_length;

unsigned char state_subsets_are_singletons(linked_list *state_subsets)
{
	void *result = NULL;
	integer_set *state_subset = NULL;

	if(linked_list_transverse(state_subsets, &result))
	{
		state_subset = (integer_set*)result;
		if(integer_set_length(state_subset) != 1)//sin-gle-ton
			return 0;
	}
	while(linked_list_transverse(NULL, &result))
	{
		state_subset = (integer_set*)result;
		if(integer_set_length(state_subset) != 1)//sin-gle-ton
			return 0;
	}

	return 1;
}

//define how to separe a sequence, by dot or by space or by what?
char separator;

//variables for reporting the average lenght of HS
//size_t length_of_HSs;
//size_t number_of_HSs;

/*

size_t sequence_length (char *sequence)
{
	size_t length = 0;
	char *ptr = sequence;

	if(!sequence) //empty string
		return 0;
	
	while (ptr)
		if (*(ptr++) == separator)
			length++;

	//separator divides sequence in two sub sequences
	return length + 1;
} 
*/

/*
 * i-successor: given a list of subsets of states of a given FSM, and an input i, returns a list (set) of subsets of successor states grouped by output
 * */

linked_list *i-successor(fsm_arr *fsm, linked_list *state_subsets, size_t input)
{
	
}

/*
 * defined_inputs: given a list of subsets of states of a given FSM, get a "list" of defined inputs for all states on the list of subsets. The list is given as an boolean array
 * */

unsigned char* defined_inputs(fsm_arr *fsm, linked_list *state_subsets)
{
	char *inputs  = NULL;
	size_t index;
	
	
	if (!fsm || !state_subsets)
		return NULL;
	
	inputs = (unsigned char*)malloc(fsm->size * sizeof(unsigned char));
	if (!inputs)
	{
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! Could not allocate memory for defined inputs\n"));
					sprintf(msg, "Error! Could not allocate memory for defined inputs\n");
					fsm_log(error, msg);               
					free(msg);
					exit(1);
	}

	
	
}

//current usage variables
clock_t initial_time;
size_t used_mem = 0;

///state_subsets is a ll which contains integer sets represetning the subsets of states
void display_hs_preset_derivation(fsm_arr *fsm, linked_list *state_subsets, char *prefix, FILE fd, size_t level)
{
	//check for HS rules
	size_t spent_time = (clock - initial_time) / CLOCKS_PER_SEC;
	if(state_subsets_are_singletons(state_subsets)) //Found a HS
	{	
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Homing Sequence Found: %s\n", prefix) + 1));
		sprintf(msg, "Homing Sequence Found: %s\n", prefix);
		fsm_log(info, msg);
		free(msg);
		printf(fd, "%s", prefix);
		minlength_of_HSs = (min_length_of_HSs > level || !minlength_of_HSs)?level:minlength_of_HSs; 
		return;
	}

	if(max_length && level >= max_length) //default truncating rule for the tree, in exception of max_level being 0
	{
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Reached Max depth = %llu with sequence %s\n", level,  prefix) + 1));
		sprintf(msg, "Reached Max depth = %llu with sequence %s\n", level,  prefix);
		fsm_log(info, msg);
		free(msg);
		return;
	}
	
	if(max_time && spent_time >= max_time) //we finish inmediatelly, sorry :) 
	{
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Homing sequence time expired: current time= %llu, max time = %llu\n", spent_time, max_time) + 1));
		sprintf(msg, "Homing sequence time expired: current time= %llu, max time = %llu\n", spent_time, max_time);
		fsm_log(info, msg);
		free(msg);

		//metric on HS length print LHSM = minlength_of_HSs depending on certain flag	
	
		exit(0);//normal exit, time expired but forseen
	}

	if(max_mem && used_mem >= max_mem) //we finish inmediatelly, sorry :) 
	{
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Homing sequence memory limit reached: current usage= %llu, max mem = %llu\n", used_mem, max_mem) + 1));
		sprintf(msg, "Homing sequence memory limit reached: current usage= %llu, max mem = %llu\n", used_mem, max_mem);
		fsm_log(info, msg);
		free(msg);

		//metric on HS length print LHSM = minlength_of_HSs depending on certain flag	
	
		exit(0);//normal exit, time expired but forseen
	}	

}

/*
 * displays to fd the homing sequences to the file descriptor, takes the max parameters as external arguments, the initial set should be an integer set with the initial states of the FSM (potentially all states) 
 * */
void display_hs (fsm_arr *fsm, integer_set *init_states, FILE fd)
{

	if(!fsm || !init_states || !fd)
		return;

	separator = '.'; //very ascii, very common separator
	initial_time = clock();
	minlength_of_HSs = 0; 

	//calculate the size of the fsm
	used_mem = (fsm->size + fsm->trans) * sizeof(size_t);
}

#ifdef __cplusplus
}
#
