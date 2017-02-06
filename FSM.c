/*
 * FSM.c: file to define the FSM functions  
 */

#include "FSM.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
	fsm_ll *retds = (fsm_ll*)malloc(1*sizeof(fsm_ll));
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
		if(*(machine->si + i))
			delete_linked_list(*(machine->si + i));
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
		fsm_log(warning, "Warning! number of states = 0 is invalid!");
		return;
	}
	machine->maxS = states;
	machine->size = machine->maxS * machine->maxI;
	if(machine->size)
	{
		machine->si = (linked_list**)realloc(machine->si, machine->size * sizeof(linked_list**));
		if(!machine->si)
		{
			fsm_log(error, "Error! Could not allocate memory for tranitions set_maxs!");
			exit(error);
		}
	}
	
}

void set_maxi(fsm_ll *machine, size_t inputs)
{
	if(!inputs)
	{
		fsm_log(warning, "Warning! input size = 0 is invalid!");
		return;
	}
	machine->maxI = inputs;
	machine->size = machine->maxS * machine->maxI;
	if(machine->size)
	{
		machine->si = (linked_list**)realloc(machine->si, machine->size * sizeof(linked_list**));
		if(!machine->si)
		{
			fsm_log(error, "Error! Could not allocate memory for tranitions set_maxi!");
			exit(critical);
		}
	}
	
	machine->sizeofI = lg2(inputs);
}

void set_maxo(fsm_ll *machine, size_t outputs)
{
	if(!outputs)
	{
		fsm_log(warning, "Warning! output size = 0 is invalid!");
		return;
	}
	machine->maxO = outputs;
	
	machine->sizeofO = lg2(outputs);
}

void set_init(fsm_ll *machine, size_t init)
{
	if(init >= machine->maxS)
	{
		fsm_log(warning, "Warning! initial state is invalid!");
		return;
	}
	machine->init = init;
}

void set_maxt(fsm_ll *machine, size_t trans)
{
	machine->maxT = trans;
}

/*
 * add_fsm_ll_transition: adds a trnsition to the given FSM
 * */

void add_fsm_ll_transition (fsm_ll *machine, size_t state, size_t input, size_t output, size_t next_state)
{
	size_t index, *transition_ptr;
	char* logmsg;	
	if(machine->trans >= machine->maxT)
	{
		logmsg =(char*) malloc(400);
		if(sprintf(logmsg, "Warning, Transition(%llu,%llu,%llu,%llu) not added, transition count= %llu, reached maximal transitons = %llu\n", input, state, next_state, output, machine->trans, machine->maxT ) < 0)
			fsm_log(3, "Error! Could not write to log!");
		else 
			fsm_log(4, logmsg);	
		free(logmsg);
		return;
	}
	if(state >= machine->maxS)
	{
		logmsg =(char*) malloc(400);
		if(sprintf(logmsg, "Warning, Transition(%llu,%llu,%llu,%llu) not added, state %llu is bigger than max_state = %llu\n", input, state, next_state, output, state, machine->maxS ) < 0)
			fsm_log(3, "Error! Could not write to log!");
		else 
			fsm_log(4, logmsg);	
		free(logmsg);
		return;
	}
	if(input >= machine->maxI)
	{
		logmsg =(char*) malloc(400);
		if(sprintf(logmsg, "Warning, Transition(%llu,%llu,%llu,%llu) not added, input %llu is bigger than max_input = %llu\n", input, state, next_state, output, input, machine->maxI) < 0)
			fsm_log(3, "Error! Could not write to log!");
		else 
			fsm_log(4, logmsg);	
		free(logmsg);
		return;
	}
	if(output >= machine->maxO)
	{
		logmsg =(char*) malloc(400);
		if(sprintf(logmsg, "Warning, Transition(%llu,%llu,%llu,%llu) not added, output %llu is bigger than max_output = %llu\n", input, state, next_state, output, output, machine->maxO) < 0)
			fsm_log(3, "Error! Could not write to log!");
		else 
			fsm_log(4, logmsg);	
		free(logmsg);
		return;
	}	
	if(next_state >= machine->maxS)
	{
		logmsg =(char*) malloc(400);
		if(sprintf(logmsg, "Warning, Transition(%llu,%llu,%llu,%llu) not added, next state %llu is bigger than max_state = %llu\n", input, state, next_state, output, next_state, machine->maxS) < 0)
			fsm_log(error, "Error! Could not write to log!");
		else 
			fsm_log(warning, logmsg);	
		free(logmsg);

		return;
	}

	index = state << machine->sizeofI  | input;
	
	transition_ptr  = (size_t*)malloc(sizeof(size_t));

	*transition_ptr = next_state << machine->sizeofO | output;

	//printf("ET index=%llu trans=%llu <-> (s=%llu,i=%llu,o=%llu,n=%llu)?\n", index, transition, state, input, output, next_state);
	if (!*(machine->si + index))
		*(machine->si + index) = create_linked_list(); 	
	
	
	if(!linked_list_add(*(machine->si + index), transition_ptr))
	{
		logmsg =(char*) malloc(400);
		if(sprintf(logmsg, "Warning! unable to add transition (%llu,%llu,%llu,%llu)! \n", input, state, next_state, output) < 0)
			fsm_log(error, "Error! Could not write to log!");

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

		if(linked_list_transverse(*(machine->si + j), &trans))
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

#ifdef __cplusplus
}
#endif
