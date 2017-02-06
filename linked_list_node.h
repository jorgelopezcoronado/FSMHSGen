/*
 * linked_list_node.h: file to define the linked list node structure
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef linked_list_node
typedef struct linked_list_node_tag
{
	void *element;
	struct linked_list_node_tag *next;
}linked_list_node;
#endif

#ifdef __cplusplus
}
#endif
