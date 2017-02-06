
/*
 * paren.h: holds the AST object for a balanced parenthesis
 */

#ifdef __cplusplus                                                                                                                                           
extern "C" {
#endif

#ifndef _paren
#define _paren

typedef enum asttypestag
{
	PARENTHESIS	
}asttype;

//which type of parenthesis 
typedef enum parentypetag
{
	NONE,
	PAREN,
	BRACK,
	CBRACK
}ptype;

//balanced parenthesis AST type
typedef struct partag
{
	ptype type; //is this a parenthesis or a bracket?

	struct partag *left; 
	struct partag *right;
}paren;

//simple function to create a balanced parenthesis structure
paren *createParen (ptype type, paren *left, paren *right);

//simple fucntion to delete a balanced parenthesis
void deleteParen(paren *p);

#endif

#ifdef __cplusplus                                                                                                                                           
}
#endif

