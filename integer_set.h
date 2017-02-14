/*
 *
 * integer_set.h: header file for integer set data structure 
 *
*/

#include <stdlib.h>
#include "helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef set
typedef struct integer_set_node_tag
{
	size_t number;
	struct integer_set_node_tag *next;
}integer_set_node;

typedef struct set_tag
{
	integer_set_node *head;
}integer_set;
#endif

/*create integer set node
 */
integer_set_node *create_integer_set_node();

/*
 * create_set
 * receives the function that compares two given elements of the set, less than, equal, or greater than
 */
integer_set *create_integer_set();

/*
 * set_add: add an element to the set
 * */
BOOL integer_set_add(integer_set *s, size_t element);

/*
 * set_delete, deletes a given element of the set
 * */
void integer_set_delete(integer_set *s, size_t element);

/*
 * set_clone: clones a set
 * */

integer_set *integer_set_clone(integer_set *s);

/*
 * compare if sets are equal?
 * */

BOOL integer_set_equals (integer_set *s1, integer_set *s2);

/*
 *print the set
 * */

void print_integer_set(integer_set *s);

#ifdef __cplusplus
}
#endif
