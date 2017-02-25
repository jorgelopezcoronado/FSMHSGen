/*
 * specification_metrics.c: file that implements the metric methods of a spec modelled as an FSM, which can be possibly partial, nondeterministic, and nonobservable 
 */

#include "specification_metrics.h"
#include "integer_set.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//external flag variables defining the functioning
//extern unsigned char hslm = 0, ndsm = 0, nosm = 0, psm = 0;
//extern size_t minlength_of_HSs;

/*displays the metrics to the corresponding file descriptor given an FSM*/
//requires the flag variables hslm, psm, etc... probably I overused extern here... well... sorry, it's friday night, I just want to finish :) 
void display_metrics(fsm_arr *fsm, FILE *fd)
{
	size_t undef = 0, nondet = 0, nonobs = 0;
	size_t i = 0, s = 0, index = 0, start = 0, end = 0;
	long double undef_m = 0, nondet_m = 0, nonobs_m = 0;
	//let's do all at once
	for(s = 0; s < fsm->maxS; s++)
	{
		for(i = 0; i< fsm->maxI; i++)
		{
			index = s << fsm->sizeofI | i;
			start = fsm->indices[index];
			end = (s == (fsm->maxS - 1) && i == (fsm->maxI - 1))?fsm->trans:fsm->indices[index + 1];
			if(start == end)//undef transition
				undef++;
			else if(end - start > 1)//more than one transition
			{
				nondet += end - start;
				{//nonobservable
					integer_set *outputs = create_integer_set();
					integer_set_node *aux = NULL;
					size_t j, o, sizeofOComp;
	
					sizeofOComp = ((size_t)(sizeof(size_t) * 8)) - fsm->sizeofO;
					for(j = start; j < end; j++)
					{	
						o = (fsm->transitions[j] << sizeofOComp) >> sizeofOComp; 
						integer_set_add(outputs, o);
					}
	
					aux = outputs->head;
					while(aux)
					{
						for(j = start; j < end; j++)
						{
							o = (fsm->transitions[j] << sizeofOComp) >> sizeofOComp; 
							if(o == aux->number)
								nonobs++;
						}
						nonobs--; //at least there will be one o = o' which it is the same
						aux = aux->next;
					}

					free(aux);
					delete_integer_set(outputs);
				}
			}
			
		}
	}	
	
	undef_m = (long double)undef / (long double)fsm->trans;
	nondet_m = (long double)nondet / (long double)fsm->trans;
	nonobs_m = (long double)nonobs / (long double)fsm->trans;
	if(psm)
		fprintf(fd, "PSM = %Lg (%llu / %llu)\n", undef_m, undef, fsm->trans );

	if(ndsm)
		fprintf(fd, "NDSM = %Lg (%llu / %llu)\n", nondet_m, nondet, fsm->trans );

	if(nosm)
		fprintf(fd, "NOSM = %Lg (%llu / %llu)\n", nonobs_m, nonobs, fsm->trans );

	if(hslm)
		fprintf(fd, "HSLM = %llu\n", minlength_of_HSs);

	//still hsl

}

void metrics(fsm_arr *fsm)
{
	display_metrics(fsm, stdout);
}


#ifdef __cplusplus
}
#endif
