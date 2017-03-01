/*
 *
 * integer_set.c: integer set data structure operations
 *
*/

#include <stdio.h>
#include <stdlib.h>

#include "integer_set.h"

#ifdef __cplusplus
extern "C" {
#endif

/*create integer set node
 */
integer_set_node *create_integer_set_node()
{
	integer_set_node *node = (integer_set_node*)malloc(sizeof(integer_set_node));
	if(!node)
		return NULL;
	node->number = 0;
	node->next = NULL;
	return node;
}

/*
 * create_set
 * receives the function that compares two given elements of the set, less than, equal, or greater than
 */
integer_set *create_integer_set()
{
	integer_set * set = (integer_set*)malloc(sizeof(integer_set));
	if(!set)
		return NULL;

	set->head = NULL;
	return set;
}

/*
 * set_add: add an element to the set, represented as an ordered list
 * */
BOOL integer_set_add(integer_set *s, size_t element)
{
	integer_set_node *aux = NULL, *aux2 = NULL, *node = NULL;
	
	if(!s)
		return FALSE;
	
	aux = s->head;
	aux2 = s->head;
	while(aux && aux->number < element)
	{
		aux2 = aux;
		aux = aux->next;
	}

	//printf("adding %llu aux->numnber = %llu aux null? = %u s=", element, (aux)?aux->number:0, (aux == NULL));
//	print_integer_set(s);
	if(aux && aux->number == element) //do not add, duplicate
		return TRUE; //as if added
	
	node = create_integer_set_node(); 
	
	if(!node)
		return FALSE;

	node->number = element;

	if(!aux2) //empty list
	{
		s->head = node;
		node->next = NULL;
	}
	else // since aux2 is one before aux, it means it is less than aux, insert after aux2 if aux2->number < element, otherwise insert left --> it is at the head!
	{
		if(aux2->number == element)
			return TRUE;
		else if(aux2->number < element)
		{
			node->next = aux2->next;
			aux2->next = node;
		}
		else //element < aux2->number
		{
			node->next = aux2;
			if(aux2 == s->head)
				s->head = node;
		}
	}
	return TRUE;
}

/*
 * set_delete, deletes a given element of the set
 * */

void integer_set_delete(integer_set *s, size_t element)
{
	integer_set_node *aux = NULL, *aux2 = NULL;	

	if(!s)
		return;
	
	aux = s->head;

	if(!aux)
		return; // nothing to do on an empty set

	if(aux->number == element)
	{
		s->head = aux->next;
		aux->next = NULL; 
		free(aux);
	}

	while(aux->next && aux->next->number != element)
		aux = aux->next;

	//two possibilities here, you are at one before the end and it means there is not that number or exacly the next needs to be deleted
	
	if(!aux->next) //nothing to do
		return;
	else
	{
		aux2 = aux->next;
		aux->next = aux2->next;
		aux2->next = NULL;
		free (aux2);
	}
}

/*
 * set_clone: clones a set
 * */

integer_set *integer_set_clone(integer_set *s)
{
	integer_set_node *node = NULL; 
	integer_set *clone = NULL;

	if(!s)
		return NULL;

	node = s->head;
	clone = create_integer_set();
	if(!clone)
		return NULL;

	while(node)
	{
		integer_set_add(clone, node->number);
		node = node->next;
	}

	return clone; 
}

/*
 * compare if sets are equal?
 * */

BOOL integer_set_equals (integer_set *s1, integer_set *s2)
{
	integer_set_node *aux1 = NULL, *aux2 = NULL;

	if((s1 == NULL && s2 != NULL) || (s1 != NULL && s2 == NULL))
		return FALSE; 
	
	if(s1 == NULL && s2 == NULL)
		return TRUE;
	
	aux1 = s1->head;
	aux2 = s2->head;
	
	while (aux1 && aux2)
	{
		if (aux1->number != aux2->number)
			return FALSE;
		aux1 = aux1->next;
		aux2 = aux2->next;
	}
		
	return aux1 == aux2; //if here, one is null, if equals both are null
}

/*
 *integer_set_to_string(): returns the string represetnation of the integer set
 * */

char *integer_set_to_string (integer_set *set)
{
	integer_set_node *aux = NULL; 
	char *string_rep  = NULL, *string_temp = NULL;

	if(set == NULL)
	{
		string_rep = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "NaIS") + 1));
		sprintf(string_rep, "NaIS");
		return string_rep;
	}

	aux = set->head;

	string_rep = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "{") + 1));
	sprintf(string_rep, "{");

	if(aux)
	{
		string_temp = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "%s", string_rep) + 1 ));
		sprintf(string_temp, "%s", string_rep);
		free(string_rep);
		string_rep = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "%s%llu", string_temp, aux->number) + 1));
		sprintf(string_rep, "%s%llu", string_temp,  aux->number);

		free(string_temp);
		aux = aux->next;
	}
	
	while(aux)
	{
		string_temp = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "%s", string_rep) + 1 ));
		sprintf(string_temp, "%s", string_rep);
		free(string_rep);
		string_rep = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "%s, %llu", string_temp, aux->number) + 1));

		sprintf(string_rep, "%s, %llu", string_temp,  aux->number);
		free(string_temp);
		aux = aux->next;
	}
	string_temp = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "%s", string_rep) + 1 ));
	sprintf(string_temp, "%s", string_rep);
	free(string_rep);
	string_rep = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, "%s}", string_temp) + 1));
	sprintf(string_rep, "%s}", string_temp);
	free(string_temp);

		
	return string_rep; 
}

/*
 *print the set
 * */

void print_integer_set(integer_set *s)
{
	char *str_rep = integer_set_to_string(s);
	printf("%s\n", str_rep);
	free(str_rep);
}

/*
 * length of an integer set
 * */

size_t integer_set_length(integer_set *s)
{
	integer_set_node *aux = NULL;
	size_t size = 0;
	if(!s)
		return 0;
	
	aux = s->head;
	while(aux)	
	{
		size++;
		aux = aux->next;
	}
	
	return size;
}

/*
 * integer_set_compare: compares the sets, returns an unsigned in as described in the constants below.
 * */
//#define A-DIFF-B (unsigned char 0)
//#define A-EQUALS-B (unsigned char 1)
//#define A-CONTAINS-B (unsigned char)2
//#define B-CONTAINS-A (unsigned char)3

unsigned char integer_set_compare(integer_set *A, integer_set *B)
{
	size_t not_presentA = 0, not_presentB = 0;
	integer_set_node *auxA = NULL, *auxB = NULL;
	
	if(!A || !B)
		return ADIFFB;//not really sets... so...
	
	auxA = A->head;
	auxB = B->head;

	while(auxA && auxB)
	{
		if(auxA->number == auxB->number)
		{
			auxA = auxA->next;	
			auxB = auxB->next;
		}
		else if(auxA->number < auxB->number)
		{
			not_presentB++;
			auxA = auxA->next;
		}
		else
		{
			not_presentA++;
			auxB = auxB->next;
		}
	}	

	while(auxA)
	{
		not_presentB++;
		auxA = auxA->next;
	}

	while(auxB)
	{
		not_presentA++;
		auxB = auxB->next;
	}

	if(not_presentA && not_presentB) //sets both have uncommon elements
		return ADIFFB;
	else if(!not_presentA && !not_presentB) //no uncommon elements ==> they're equal
		return AEQUALSB;
	else if(!not_presentA && not_presentB) //A has more elements than B but all of B's elements are in A
		return ACONTAINSB;
	else	//only one possibility left, B has elements that A does not have, therefore B contains A
		return BCONTAINSA; 
}


/*
 * traverse: traverses the integer_set, returns null when set is traversed
 * */

size_t *integer_set_traverse(integer_set *set)
{
	static integer_set_node *curr = NULL;
	
	curr = (set)?set->head:((curr)?curr->next:NULL);

	if (curr)
		return &curr->number;
	return NULL;
}

/*
 * delete_integer_set: deletes the integer set releasing all pointers and values
 * */

void delete_integer_set (integer_set *set)
{
	integer_set_node *aux = NULL, *aux2 = NULL;
	if(!set)
		return;
	aux = set->head;
	while(aux)
	{
		aux2 = aux;	
		aux = aux->next;
		aux2->next = NULL;
		free(aux2);
	}

	free(set);
}

/*
void test_integer_set()
{
	
	unsigned char i;
	size_t *element;

	integer_set *s1 = create_integer_set();
	integer_set *s2 = NULL;
	integer_set_add(s1, 10);
	integer_set_add(s1, 11);
	integer_set_add(s1, 11);
	integer_set_add(s1, 12);
	integer_set_add(s1, 10);
	integer_set_add(s1, 13);
	integer_set_add(s1, 12);
	integer_set_add(s1, 2);

	
	s2 = integer_set_clone(s1);

	printf("s1 =");
	print_integer_set(s1);
	printf("s2 =");
	print_integer_set(s2);
	
	printf("sets equal? %u\n",integer_set_equals(s1,s2));
	printf("compare sets result:%u\n", integer_set_compare(s1,s2));
	
	integer_set_delete(s1,12);
	integer_set_delete(s1,12);

	integer_set_delete(s2,2);
	integer_set_delete(s2,13);

	printf("s1 =");
	print_integer_set(s1);
	printf("s2 =");
	print_integer_set(s2);
	
	printf("sets equal? %u\n",integer_set_equals(s1,s2));
	printf("compare sets result:%u\n", integer_set_compare(s1,s2));

	integer_set_delete(s2,10);
	integer_set_delete(s2,11);
	integer_set_delete(s2,12);
	integer_set_delete(s2,12);

	printf("s1 =");
	print_integer_set(s1);
	printf("s2 =");
	print_integer_set(s2);
	
	printf("sets equal? %u\n",integer_set_equals(s1,s2));
	printf("compare sets result:%u\n", integer_set_compare(s1,s2));

	printf("s2 =");
	print_integer_set(s2);
	printf("s1 =");
	print_integer_set(s1);
	
	printf("sets equal? %u\n",integer_set_equals(s2,s1));
	printf("compare sets result:%u\n", integer_set_compare(s2,s1));

	printf("size of s1=%llu, of s2=%llu\n", integer_set_length(s1), integer_set_length(s2));
	
	element = integer_set_traverse(s2);
	printf("s2 element %d\n", (element)?*element:-1);

	for(i=0; i< 7; i++)
	{
		element = integer_set_traverse(NULL);
		printf("s2 element %d\n", (element)?*element:-1);
	}
	
	element = integer_set_traverse(s1);
	printf("s1 element %d\n", (element)?*element:-1);

	for(i=0; i< 7; i++)
	{
		element = integer_set_traverse(NULL);
		printf("s1 element %d\n", (element)?*element:-1);
	}

	element = integer_set_traverse(s1);
	printf("s1 element %d\n", (element)?*element:-1);

	delete_integer_set(s1);
	delete_integer_set(s2);

}

int main()
{
	test_integer_set();
	return 0;
}

//*/


#ifdef __cplusplus
}
#endif
