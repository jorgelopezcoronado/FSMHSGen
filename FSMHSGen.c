#include "FSM.h"
#include <stdio.h>

void test_fsm_ll()
{
	fsm_ll *myfsm = create_fsm_ll();
	set_maxs(myfsm, 4);
	set_maxi(myfsm, 4);
	set_maxo(myfsm, 3);
	set_maxt(myfsm, 8);
	set_init(myfsm, 0);
	
	add_fsm_ll_transition(myfsm, 0, 1, 0, 1);
	add_fsm_ll_transition(myfsm, 0, 0, 0, 2);
	add_fsm_ll_transition(myfsm, 1, 3, 1, 3);
	add_fsm_ll_transition(myfsm, 2, 1, 1, 0);
	add_fsm_ll_transition(myfsm, 1, 1, 1, 2);
	add_fsm_ll_transition(myfsm, 0, 2, 0, 3);
	add_fsm_ll_transition(myfsm, 1, 3, 2, 2);
	add_fsm_ll_transition(myfsm, 1, 0, 1, 1);

	print_fsm_ll(myfsm);

	delete_fsm_ll(myfsm);
	
}
	

int main()
{
	test_fsm_ll();
	return 0;
}
