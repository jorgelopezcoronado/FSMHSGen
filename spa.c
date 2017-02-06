/*
 * static program analyizer 101
 */

#include "paren.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

int yyparse (paren **parenobj, yyscan_t scan);

paren *getAST (char *source_code)
{
	paren *pobj; //root of AST change this depending on your AST data structure

	yyscan_t scanner;
	YY_BUFFER_STATE state;

	if (yylex_init(&scanner)) 
	{
		//couldn't initialize
		return NULL;
	}

	state = yy_scan_string(source_code, scanner); //tokenize source_code
	
	if (yyparse(&pobj, scanner)) //retreive the AST from parser using tokenize string
	{
		//error parsing not your issue
		return NULL;
	}
        
	yy_delete_buffer(state, scanner);
	yylex_destroy(scanner);

	return pobj; //this should be changed
}

void printAST(paren *node, int indent)
{
	int i = 0;
	if (node == NULL)
		return;
	if(node->type == PAREN)
	{
		for (i = 0; i< indent; i++)
			printf("\t");

		printf("()\n");
		printAST(node->left, indent + 1);
	}
	else if(node->type == BRACK)
	{
		for (i = 0; i< indent; i++)
			printf("\t");

		printf("[]\n");
		printAST(node->left, indent + 1);
	}
	else if(node->type == CBRACK)
	{
		for (i = 0; i< indent; i++)
			printf("\t");

		printf("{}\n");
		printAST(node->left, indent + 1);
	}
	else 
	{
		printAST(node->left, indent);
		printAST(node->right, indent);
	}
}

char *textFromFile(char *filename)
{
	FILE *file;
	size_t fileSize;
	char *text;
		

	file = fopen(filename, "rb");
	if (!file)
	{
		printf("Error reading file %s!\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END); 
	fileSize = ftell(file); // what's the position of the pointer in the end of this stream?
	rewind(file);
	
	text = (char*)malloc(fileSize + 1); //one mor to null terminate the string

	if(!text)
	{
		//failed to allocate text
		return NULL;
	}
	
	if(!fread(text, 1, fileSize, file))
	{
		printf("Error! Unable to read the file %s!", filename);
		free(text);
		return NULL;
	}

	fclose(file);
	
	return text;

}
	
int main(int argc, char **argv)
{
	char *input;
	
	if(argc < 2)
	{
		printf("Error! a filename to statically analyize is expected!\n");
		exit(1);
	}
	
	input = textFromFile(argv[1]);
	
	if(!input)
	{	
		//no input from file
		exit(1);
	}

	paren *root = getAST(input);
	
	if(!root)
	{
		//parse error! 
		exit(1);
	}
	
	printAST(root, 0);
	deleteParen(root);	
}

