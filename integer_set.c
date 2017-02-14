/*
 *
 * integer_set.c: integer set data structure operations
 *
*/

#include <stdio.h>

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
 *print the set
 * */

void print_integer_set(integer_set *s)
{
	integer_set_node *aux = NULL; 

	if(s == NULL)
	{
		printf("NaIS\n"); //Not a set
		return;
	}

	aux = s->head;

	printf("{");

	if(aux)
	{
		printf("%llu", aux->number);
		aux = aux->next;
	}
	
	while(aux)
	{
		printf(", %llu", aux->number);
		aux = aux->next;
	}
	
	printf("}\n");
	
}

void test_integer_set()
{
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
	
	integer_set_delete(s1,12);
	integer_set_delete(s1,12);

	integer_set_delete(s2,2);
	integer_set_delete(s2,13);

	printf("s1 =");
	print_integer_set(s1);
	printf("s2 =");
	print_integer_set(s2);
	
	printf("sets equal? %u\n",integer_set_equals(s1,s2));

	integer_set_delete(s2,10);
	integer_set_delete(s2,11);
	integer_set_delete(s2,12);
	integer_set_delete(s2,12);

	printf("s1 =");
	print_integer_set(s1);
	printf("s2 =");
	print_integer_set(s2);
	
	printf("sets equal? %u\n",integer_set_equals(s1,s2));
}


#ifdef __cplusplus
}
#endif
