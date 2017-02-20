/*
 *
 * FSM_Gedanken_experiments.c: implemented gedanken experiments over an FSM data structure
 *
*/

#include "FSM_Gedanken_experiments.h"
#include "helpers.h"

#include <time.h>
#include <string.h>

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
 * print_node: useful function to print a successor tree node, i.e., a list of subsets of states
 * */
void print_node(linked_list *ll)
{
	linked_list_node *aux = NULL;
	printf("{\n");
	aux = ll->head;
	while(aux)	
	{
		print_integer_set((integer_set*)aux->element);
		aux  = aux->next;
	}
	printf("}\n");
}

//useful to use the grouped outputs
#ifndef integer_set_output
typedef struct integer_set_output_tag
{
	size_t output;	
	integer_set *set;
}integer_set_output;
#endif
/*
 * i-successor: given a list of subsets of states of a given FSM, and an input i, returns a list (set) of subsets of successor states grouped by output
 * */

linked_list *i_successor(fsm_arr *fsm, linked_list *state_subsets, size_t input)
{
	linked_list *set_output_list = NULL, *node = NULL; 
	linked_list_node *subsets_ptr = NULL, *set_output_node = NULL;
	integer_set *subset = NULL;
	integer_set_node *state_ptr = NULL;
	integer_set_output *set_output = NULL;
	size_t s, index, start, end, j, o, n, transition, sizeofOComp;
	unsigned char found;

	if(!fsm || !state_subsets || input >= fsm->maxI)
		return NULL;

	sizeofOComp = ((size_t)(sizeof(size_t) * 8)) - fsm->sizeofO;
	subsets_ptr = state_subsets->head;

	node = create_linked_list();
	if(!node)
	{
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! Could not allocate memory for i-succ list\n")));
		sprintf(msg, "Error! Could not allocate memory for i-succ list\n");
		fsm_log(error, msg);               
		free(msg);
		exit(1);
	}
	
	while(subsets_ptr)
	{
		set_output_list = create_linked_list();
		if(!set_output_list)
		{
			char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! Could not allocate memory for i-succ list\n")));
			sprintf(msg, "Error! Could not allocate memory for i-succ list\n");
			fsm_log(error, msg);               
			free(msg);
			exit(1);
		}

		subset = (integer_set*)subsets_ptr->element;	
		
		state_ptr = (subset)?subset->head:NULL;
		
		while(state_ptr)
		{
			s = state_ptr->number;	
			index = s << fsm->sizeofI | input;
			start = fsm->indices[index];
			end = (s == (fsm->maxS - 1) && input == (fsm->maxI - 1))?fsm->trans:fsm->indices[index + 1];
			
			for (j = start; j < end; j++)//get all transitions classified by output
			{
				transition = fsm->transitions[j];
				n = transition >> fsm->sizeofO;
				o = (transition << sizeofOComp) >> sizeofOComp;
				set_output_node = set_output_list->head;
				found = 0;
				while(!found && set_output_node)
				{	
					//find output, add to corresponding set
					set_output = (integer_set_output*)set_output_node->element;
					if(set_output->output == o)
					{
						integer_set_add(set_output->set, n);
						found = 1;
						break;
					}
					set_output_node = set_output_node->next;
				}

				if(!found)//need to add to the working list
				{
					integer_set *new_subset = create_integer_set(); 
					integer_set_output *new_set_output = malloc(sizeof(integer_set_output));
							
					if(!new_subset || !new_set_output)
					{
						char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! Could not allocate memory for i-succ list\n")));
						sprintf(msg, "Error! Could not allocate memory for i-succ list\n");
						fsm_log(error, msg);               
						free(msg);
						exit(1);
					}
					
					integer_set_add(new_subset, n);
					new_set_output->output = o;
					new_set_output->set = new_subset;
					
					linked_list_add(set_output_list, new_set_output);
				}
			}
		
			state_ptr = state_ptr->next;

		}

		//convert set output list to a simple list of subsets, i.e., from set_output_list -> node
		set_output_node = set_output_list->head;
		while(set_output_node)
		{
			set_output = (integer_set_output*)set_output_node->element;
			linked_list_add(node, set_output->set);
			set_output_node = set_output_node->next;
		}
		
		del_linked_list(set_output_list);
		subsets_ptr = subsets_ptr->next;
	}
	
	return node;
	
}

/*
 * defined_inputs: given a list of subsets of states of a given FSM, get a "list" of defined inputs for all states on the list of subsets. The list is given as an boolean array
 * */

unsigned char* defined_inputs(fsm_arr *fsm, linked_list *state_subsets)
{
	unsigned char *inputs  = NULL;
	integer_set *set = NULL;
	linked_list_node *set_ptr = NULL;
	integer_set_node *state_ptr = NULL;
	size_t s, i, index, start, end;
	
	
	if (!fsm || !state_subsets)
		return NULL;
	inputs = (unsigned char*)malloc(fsm->maxI * sizeof(unsigned char));
	if (!inputs)
	{
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! Could not allocate memory for defined inputs\n")));
		sprintf(msg, "Error! Could not allocate memory for defined inputs\n");
		fsm_log(error, msg);               
		free(msg);
		exit(1);
	}
	memset(inputs, 1, fsm->maxI * sizeof(unsigned char));	
	
	set_ptr = state_subsets->head;
	
	while(set_ptr)	
	{
		set = (integer_set*)set_ptr->element;
		state_ptr = (set)?set->head:NULL;
		while(state_ptr)
		{
			s = state_ptr->number;
			for(i = 0; i < fsm->maxI; i++)
			{
				index = s << fsm->sizeofI | i;
				start = fsm->indices[index];
				end = (s == (fsm->maxS - 1) && i == (fsm->maxI - 1))?fsm->trans:fsm->indices[index + 1];
				if(start == end)
					inputs[i] = 0;
			}
			state_ptr = state_ptr->next;
		}
		set_ptr = set_ptr->next;
	}

	return inputs;
	
	
}

/*
 * del_subset_from_node: remove subsets in node, auxiliary function applied after i-successor to remove proper subsets of it
 * */
void del_subsets_from_node(linked_list **node)
{
	linked_list *ll = NULL;
	linked_list_node *subset_node = NULL, *subset_nodeaux = NULL, *subset_prev = NULL, *subset_prevaux;
	
	if(!node)
		return;

	ll = *node;

	if(!ll)
		return;
	subset_node = ll->head;
	subset_prev = ll->head;
	
	while(subset_node)
	{
		unsigned char head_deleted = 0;
		integer_set *A = (integer_set*)subset_node->element;
		subset_prevaux = subset_node;
		subset_nodeaux = subset_node->next;
		while(subset_nodeaux)
		{
			unsigned char compare;
			integer_set *B = (integer_set*)subset_nodeaux->element;
			compare = integer_set_compare(A, B);
			if(compare == AEQUALSB || compare == ACONTAINSB)//delete b, simple
			{
				subset_prevaux->next = subset_nodeaux->next; //
				subset_nodeaux->next = NULL;
				subset_nodeaux->element = NULL;
				delete_integer_set(B);
				free(subset_nodeaux);
				subset_nodeaux = subset_prevaux->next;
			}
			else if(compare == BCONTAINSA)//delete a
			{
				if(subset_node == ll->head) //take head into consideration and move head
				{	
					ll->head = ll->head->next;
					subset_prev = ll->head;					
					head_deleted = 1;
				}
				else
					subset_prev->next = subset_node->next;
				subset_node->next = NULL;
				subset_node->element = NULL;
				delete_integer_set(A);
				free(subset_node);
				subset_node = subset_prev;
				break;
			}
			else //delete nothing
			{
				subset_prevaux = subset_prevaux->next;
				subset_nodeaux = subset_nodeaux->next;
			}
		}
		if(!head_deleted)
		{
			subset_prev = subset_node;
			subset_node = subset_node->next;
		}
	}
	
}

//current usage variables
clock_t initial_time;
size_t used_mem = 0;
size_t HS_counter = 0;

///state_subsets is a ll which contains integer sets represetning the subsets of states
void display_hs_preset_derivation(fsm_arr *fsm, linked_list *state_subsets, char *prefix, FILE *fd, size_t level)
{
	//check for HS rules
	size_t spent_time = (clock() - initial_time) / CLOCKS_PER_SEC;
	if(state_subsets_are_singletons(state_subsets)) //Found a HS
	{	
		char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Homing Sequence Found: %s\n", prefix) + 1));
		sprintf(msg, "Homing Sequence Found: %s\n", prefix);
		fsm_log(info, msg);
		free(msg);
		fprintf(fd, "%s", prefix);
		minlength_of_HSs = (minlength_of_HSs > level || !minlength_of_HSs)?level:minlength_of_HSs; 
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
void display_hs (fsm_arr *fsm, integer_set *init_states, FILE *fd)
{
	integer_set_node *aux = NULL;
	linked_list *ll = NULL, *isucc = NULL; 
	integer_set *test_set = NULL;

	unsigned char *defined_i = NULL;
	size_t i;

	if(!fsm || !init_states || !fd)
		return;

	//check states are all valid for the fsm
	aux = init_states->head;
	while(aux)
	{
		if(aux->number >= fsm->maxS)
		{
			char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "Error! initial state %llu is greater or equal than FSM max state = %llu", aux->number, fsm->maxS ) + 1));
			sprintf(msg, "Error! initial state %llu is greater or equal than FSM max state = %llu", aux->number, fsm->maxS);
			fsm_log(error, msg);
			free(msg);
			return;
		}
		aux = aux->next;
	}

	
		
	ll = create_linked_list();
//	linked_list_add(ll, init_states);

	test_set = create_integer_set();
	integer_set_add(test_set, 0);
	integer_set_add(test_set, 1);
	integer_set_add(test_set, 2);
	linked_list_add(ll, test_set);

	test_set = create_integer_set();
	integer_set_add(test_set, 2);
	integer_set_add(test_set, 3);
	linked_list_add(ll, test_set);


	separator = '.'; //very ascii, very common separator
	initial_time = clock();
	minlength_of_HSs = 0; 
	HS_counter = 0;

	//calculate the size of the fsm
	used_mem = (fsm->size + fsm->trans) * sizeof(size_t);
	//call the hs process
	print_node(ll);
	printf("\n\n\n");

	isucc = i_successor(fsm, ll, 1);
	
	test_set = create_integer_set();
	integer_set_add(test_set, 13);
	integer_set_add(test_set, 19);
	linked_list_add(isucc, test_set);
	test_set = create_integer_set();
	integer_set_add(test_set, 13);
	integer_set_add(test_set, 19);
	linked_list_add(isucc, test_set);

	
	print_node(isucc);

	printf("\n\n\n");

	del_subsets_from_node(&isucc);

	print_node(isucc);

}

#ifdef __cplusplus
}
#endif
