/*
 * specification_metrics.h: file to define the metric methods of a spec modelled as an FSM, which can be possibly partial, nondeterministic, and nonobservable 
 */

#include "FSM.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//external flag variables defining the functioning
extern unsigned char hslm, ndsm, nosm, psm;
extern size_t minlength_of_HSs;

//requires the flag variables hslm, psm, etc... probably I overused extern here... well... sorry, it's friday night, I just want to finish :) 
void metrics(fsm_arr *fsm);

#ifdef __cplusplus
}
#endif
