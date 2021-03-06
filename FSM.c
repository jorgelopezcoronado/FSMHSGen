/*
 * FSM.c: file to define the FSM functions  
 */

#include "FSM.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

/*
 * struct definition of FSM data structures 
 */

#ifdef __cplusplus
extern "C" {
#endif


/*
 * create_fsm_ll_: initializes an fsmll data structure 
 */

fsm_ll *create_fsm_ll()
{
	fsm_ll *retds = (fsm_ll*)malloc(sizeof(fsm_ll));
	retds->si = NULL;
	retds->size = 0;
	retds->maxS = 0;
	retds->maxI = 0;
	retds->maxO = 0;
	retds->maxT = 0;
	retds->trans= 0;
	retds->init = 0;
	retds->sizeofI = 0;
	retds->sizeofO = 0;
	
	return retds;
}

/**
 * delete_fsm_ll deletes the FSM. Warning! the method recursively deletes the elements inside of it!
 */

void delete_fsm_ll (fsm_ll *machine)
{
	size_t i;
	
	for (i = 0; i < machine->size; i++)
		if(machine->si[i])
			delete_linked_list(machine->si[i]);

	free(machine->si);
	free(machine);
}

/*
 *
 *lg2 function that computes lg2 of x, i.e., necessary n of an n-bit integer
 *
 * */
unsigned char lg2(size_t x)
{
	unsigned char curr = sizeof(x) * 4; //*8 bytes to bits and / 2 middle pos 
	unsigned char count = 1;
	x--;
	while (curr)
	{
		if(x >> curr)
		{
			x >>= curr;
			count +=curr;
		}
		curr >>=1;
	}
	return count;
}

/*
 *
 * set struct member functions, e.g., set_maxi sets the maximal number of inputs 
 *
 * */

void set_maxs(fsm_ll *machine, size_t states)
{
	if(!states)
	{
		fsm_log(warning, "Warning! number of states = 0 is invalid!\n");
		return;
	}
	machine->maxS = states;
	if(machine->maxS * machine->maxI)
	{
		machine->size = 1 + (((machine->maxS - 1) << machine->sizeofI) | (machine->maxI - 1));//+1 since 0 needs to be also stored
		if(machine->si)
			free(machine->si);
		machine->si = (linked_list**)malloc(machine->size * sizeof(linked_list*));
		if(!machine->si)
		{
			fsm_log(error, "Error! Could not allocate memory for tranitions set_maxs!\n");
			exit(error);
		}
		memset (machine->si, 0, machine->size * sizeof(linked_list*));	
			
	}
	
}

void set_maxi(fsm_ll *machine, size_t inputs)
{
	if(!inputs)
	{
		fsm_log(warning, "Warning! input size = 0 is invalid!\n");
		return;
	}
	machine->maxI = inputs;
	machine->sizeofI = lg2(inputs);

	if(machine->maxS * machine->maxI)
	{
		machine->size = 1 + (((machine->maxS - 1) << machine->sizeofI) | (machine->maxI - 1));//+1 since 0 needs to be also stored
		if(machine->si)
			free(machine->si);
		machine->si = (linked_list**)malloc(machine->size * sizeof(linked_list*));
		if(!machine->si)
		{
			fsm_log(error, "Error! Could not allocate memory for tranitions set_maxi!\n");
			exit(critical);
		}

		memset (machine->si, 0, machine->size * sizeof(linked_list*));	
			
	}
	
}

void set_maxo(fsm_ll *machine, size_t outputs)
{
	if(!outputs)
	{
		fsm_log(warning, "Warning! output size = 0 is invalid!\n");
		return;
	}
	machine->maxO = outputs;
	
	machine->sizeofO = lg2(outputs);
}

void set_init(fsm_ll *machine, size_t init)
{
	if(init >= machine->maxS)
	{
		fsm_log(warning, "Warning! initial state is invalid!\n");
		return;
	}
	machine->init = init;
}

void set_maxt(fsm_ll *machine, size_t trans)
{
	machine->maxT = trans;
}

char comp_trans(void *el1, void *el2)
{
	size_t one = *((size_t*)el1);
	size_t two = *((size_t*)el2);
	if (one < two)
		return (char)-1;
	else if(one > two)
		return (char)1;
	else
		return (char)0;	
}

/*
 * add_fsm_ll_transition: adds a trnsition to the given FSM
 * */

void add_fsm_ll_transition (fsm_ll *machine, size_t state, size_t input, size_t output, size_t next_state)
{
	size_t index=0, *transition_ptr = NULL;
	char *logmsg = NULL, *logerror = "Error! Could not write to log!\n";	

	if(!machine || !machine->si)
	{
		logmsg =(char*)malloc(400*sizeof(char));
		if(sprintf(logmsg, "Warning, Transition(i=%llu,s=%llu,n=%llu,o=%llu) not added, empty machine\n", input, state, next_state, output) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);
		return;

	}

	//printf("insert (s=%llu,i=%llu,o=%llu,n=%llu)\n", state, input, output, next_state);
	if(machine->trans >= machine->maxT)
	{
		logmsg =(char*)malloc(400*sizeof(char));
		if(sprintf(logmsg, "Warning, Transition(s=%llu,i=%llu,n=%llu,o=%llu) not added, transition count= %llu, reached maximal transitons = %llu\n",  state, input, next_state, output, machine->trans, machine->maxT ) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);
		return;
	}
	if(state >= machine->maxS)
	{
		logmsg =(char*)malloc(400*sizeof(char));
		if(sprintf(logmsg, "Warning, Transition(s=%llu,i=%llu,n=%llu,o=%llu) not added, state %llu is bigger than max_state = %llu\n", state, input, next_state, output, state, machine->maxS ) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);
		return;
	}
	if(input >= machine->maxI)
	{
		logmsg =(char*)malloc(400*sizeof(char));
	
		if(sprintf(logmsg, "Warning, Transition(s=%llu,i=%llu,n=%llu,o=%llu) not added, input %llu is bigger than max_input = %llu\n", state, input,  next_state, output, input, machine->maxI) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);
		return;
	}
	if(output >= machine->maxO)
	{
		logmsg =(char*)malloc(1000*sizeof(char));
		if(sprintf(logmsg, "Warning, Transition(s=%llu,i=%llu,n=%llu,o=%llu) not added, output %llu is bigger than max_output = %llu\n", state, input, next_state, output, output, machine->maxO) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);
		return;
	}	
	if(next_state >= machine->maxS)
	{
		logmsg =(char*)malloc(400*sizeof(char));
		if(sprintf(logmsg, "Warning, Transition(s=%llu,i=%llu,n=%llu,o=%llu) not added, next state %llu is bigger than max_state = %llu\n", state, input, next_state, output, next_state, machine->maxS) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);
		return;
	}

	index = state << machine->sizeofI  | input;
	
	transition_ptr  = (size_t*)malloc(sizeof(size_t));
	*transition_ptr = next_state << machine->sizeofO | output;

	//printf("ET index=%llu trans=%llu <-> (s=%llu,i=%llu,o=%llu,n=%llu)?\n", index, *transition_ptr, state, input, output, next_state);
	if (!machine->si[index])
		machine->si[index] = create_linked_list(); 	
	
	//printf("m size=%llu maxs=%llu maxi=%llu\n", machine->size, machine->maxS, machine->maxI);
	
	if(!linked_list_add_unique(machine->si[index], transition_ptr, comp_trans))
	{
		logmsg =(char*)malloc(400*sizeof(char));
		if(sprintf(logmsg, "Warning! unable to add transition (s=%llu,i=%llu,n=%llu,o=%llu)! \n", state, input, next_state, output) < 0)
			fsm_log(error, logerror);
		else 
			fsm_log(warning, logmsg);
		free(logmsg);
		return;

	}

	machine->trans++;
	
}

/*
 *
 * print_fsm_ll: prints the FSM transitions in state/input order
 *
 * */

void print_fsm_ll(fsm_ll *machine)
{
	size_t j, *transition, sizeofIComp, sizeofOComp;
	size_t i,o,s,n;
	void *trans;
	sizeofIComp  = sizeof(size_t) * 8 - machine->sizeofI;
	sizeofOComp = ((size_t)(sizeof(size_t) * 8)) - machine->sizeofO;
	
	for(j = 0; j < machine->size; j++)
	{
		s = j >> machine->sizeofI;
		i = (j << sizeofIComp) >> sizeofIComp;

		if(linked_list_transverse(machine->si[j], &trans))
		{
			transition = (size_t*)trans;
			n = *transition >> machine->sizeofO;
			o = (*transition << sizeofOComp) >> sizeofOComp;
			//printf("in=%llu, tr=%llu <-> (s=%llu,i=%llu,o=%llu,n=%llu)\n", j, *transition, s,i,o,n);
			printf("(s=%llu,i=%llu,o=%llu,n=%llu)\n", s,i,o,n);
		}
		while(linked_list_transverse(NULL, &trans))
		{
			transition = (size_t*)trans;
			n = *transition >> machine->sizeofO;
			o = (*transition << sizeofOComp) >> sizeofOComp;
			printf("(s=%llu,i=%llu,o=%llu,n=%llu)\n", s,i,o,n);
		}
	}
}

/*
 * fsm_ll_to_fsm_arr: function that transforms an FSM using a linked list to an FSM array
 * */

fsm_arr *fsm_ll_to_fsm_arr(fsm_ll *machine)
{
	fsm_arr *fsm = (fsm_arr*)malloc(sizeof(fsm_arr));
	size_t current_index;
	size_t j, *transition, sizeofIComp, sizeofOComp;
	void *trans;

	if(!machine)
		return NULL;

	fsm->size = machine->size; 
	fsm->indices =  (size_t*)malloc(machine->size*sizeof(size_t));
	fsm->transitions = (size_t*)malloc(machine->trans * sizeof(size_t));
	fsm->sizeofI = machine->sizeofI;
	fsm->sizeofO = machine->sizeofO;
	fsm->maxO= machine->maxO;
	fsm->maxI = machine->maxI;
	fsm->maxS = machine->maxS;
	fsm->trans = machine->trans;
	
	sizeofIComp  = sizeof(size_t) * 8 - machine->sizeofI;
	sizeofOComp = ((size_t)(sizeof(size_t) * 8)) - machine->sizeofO;

	current_index = 0;

	//copy each index and for each index allocate transitions
	for (j = 0; j < machine->size; j++)
	{
		fsm->indices[j] = current_index;
		if(linked_list_transverse(*(machine->si + j), &trans))
		{
			transition = (size_t*)trans;
			if(current_index >= fsm->trans)
			{
				fsm_log(warning, "Warning! more transitions are defined than the stated number!");
				continue;
			}
			fsm->transitions[current_index] = *transition;
			current_index++;
		}
		while(linked_list_transverse(NULL, &trans))
		{
			transition = (size_t*)trans;
			if(current_index >= fsm->trans)
			{
				fsm_log(warning, "Warning! more transitions are defined than the stated number!");
				continue;
			}
			fsm->transitions[current_index] = *transition;
			current_index++;
		}
	}
	
	return fsm;
}

/*
 * delete fsm_arr: unallocates / deletes the fsm_arr machine
 * */
void delete_fsm_arr(fsm_arr *fsm)
{
	free(fsm->indices);
	free(fsm->transitions);	
	free(fsm);
}


/*
 * print fsm_arr: prints the fsm_arr in state/input order
 * */

void print_fsm_arr(fsm_arr *fsm)
{
	size_t j, k, m, transition, sizeofIComp, sizeofOComp;
	size_t i,o,s,n;
	sizeofIComp  = sizeof(size_t) * 8 - fsm->sizeofI;
	sizeofOComp = ((size_t)(sizeof(size_t) * 8)) - fsm->sizeofO;
	printf("FSM\nMaxS = %llu\tMaxI = %llu\tSize = %llu\tTrans = %llu\nArrays:\nIndices:", fsm->maxS, fsm->maxI, fsm->size, fsm->trans);
	for(j = 0; j < fsm->size; j++)
		printf("[%llu] = %llu ", j, fsm->indices[j]);
	printf("\nTransitions:");
	for(j = 0; j < fsm->trans; j++)
		printf("[%llu] = %llu ", j, fsm->transitions[j]);
	printf("\nTuple Representation:\n");

	for(j = 0; j < fsm->size; j++)
	{
		s = j >> fsm->sizeofI;
		i = (j << sizeofIComp) >> sizeofIComp;
		
		m = (j != fsm->size - 1)? fsm->indices[j+1] : fsm->trans;
		
		for(k = fsm->indices[j]; k < m; k++)	
		{
			transition = fsm->transitions[k];
			n = transition >> fsm->sizeofO;
			o = (transition << sizeofOComp) >> sizeofOComp;
			printf("(s=%llu,i=%llu,o=%llu,n=%llu)\n", s,i,o,n);
		}
	}
}


#ifdef __cplusplus
}
#endif
