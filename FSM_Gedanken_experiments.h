
/*
 *
 * FSM_Gedanken_experiments.h: header file of implemented gedanken experiments over an FSM data structure
 *
*/

#include "FSM.h"
#include "integer_set.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//extern variables to control the truncating rule of the tree generation 
extern size_t max_time;
extern size_t max_mem;
extern size_t max_length;

//variables for reporting the average lenght of HS
size_t minlength_of_HSs;

/*
 * displays to fd the homing sequences to the file descriptor, takes the max parameters as external arguments, the initial set should be an integer set with the initial states of the FSM (potentially all states) 
 * */
void display_hs (fsm_arr *fsm, integer_set *init_states, FILE *fd);

#ifdef __cplusplus
}
#endif
