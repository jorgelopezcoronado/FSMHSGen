/*
 * FSM.h: file to define the FSM data structure representations 
 */

#include "linked_list.h"
#include <stdlib.h>

/*
 * struct definition of FSM data structures 
 */

#ifdef __cplusplus
extern "C" {
#endif


//an FSM data structure with linked lists
#ifndef fsm_ll
typedef struct fsm_ll_tag
{
	linked_list **si; //state input "array" of lists
	size_t maxS;
	size_t maxI;
	size_t maxO;
	size_t maxT;
	size_t init;
	size_t size;
	size_t trans;
	unsigned char sizeofI;
	unsigned char sizeofO;
}fsm_ll;
#endif

//an fsm data structure with arrays
#ifndef fsm_arr
typedef struct fsm_arr_tag
{
	size_t size;
	size_t *indices; //array of indices
	size_t *transitions; //array of transitions
	size_t init;
	size_t trans;
	unsigned char sizeofI;
	unsigned char sizeofO;
	size_t maxI;
	size_t maxO;
}fsm_arr;
#endif

/*
 * create_fsm_ll_: initializes an fsmll data structure 
 */

fsm_ll *create_fsm_ll();

/**
 * delete_fsm_ll deletes the FSM. Warning! the method recursively deletes the elements inside of it!
 */

void delete_fsm_ll (fsm_ll *machine);

/*
 *
 * set struct member functions, e.g., set_maxi sets the maximal number of inputs 
 *
 * */

void set_maxs(fsm_ll *machine, size_t states);

void set_maxi(fsm_ll *machine, size_t inputs);

void set_maxo(fsm_ll *machine, size_t outputs);

void set_init(fsm_ll *machine, size_t init);

void set_maxt(fsm_ll *machine, size_t trans);

/*
 * add_fsm_ll_transition: adds a trnsition to the given FSM
 * */

void add_fsm_ll_transition (fsm_ll *machine, size_t state, size_t input, size_t output, size_t next_state);

/*
 *
 * print_fsm_ll: prints the FSM transitions in state/input order
 *
 * */

void print_fsm_ll(fsm_ll *machine);

 /*
 * fsm_ll_to_fsm_arr: function that transforms an FSM using a linked list to an FSM array
 * */

fsm_arr *fsm_ll_to_fsm_arr(fsm_ll *machine);

/*
 * delete fsm_arr: unallocates / deletes the fsm_arr machine
 * */
void delete_fsm_arr(fsm_arr *fsm);

/*
 * print fsm_arr: prints the fsm_arr in state/input order
 * */


void print_fsm_arr(fsm_arr *fsm);

#ifdef __cplusplus
}
#endif
