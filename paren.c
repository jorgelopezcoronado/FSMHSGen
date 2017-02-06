#include "paren.h"
#include <stdlib.h>

//simple function to create a balanced parenthesis structure
paren *createParen (ptype t, paren *l, paren *r)
{
	paren *result = (paren*)malloc(sizeof(paren));
	result->type = t;
	result->left = l;
	result->right = r;
	return result; // too simple of a func, I know...
}

//simple fucntion to delete a balanced parenthesis
void deleteParen(paren *p)
{
	if(p->left != NULL)
		deleteParen(p->left);
	if(p->right != NULL)
		deleteParen(p->right);
	free (p);
}
