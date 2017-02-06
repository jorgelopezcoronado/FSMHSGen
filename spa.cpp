/*
 * static program analyizer 101
 */
using namespace std;
#include "paren.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <iostream>

int yyparse (paren **parenobj, yyscan_t scan);

paren *getAST (char *source_code)
{
	paren *pobj; //root of AST

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
		//error parsing
		return NULL;
	}
        
	yy_delete_buffer(state, scanner);
	yylex_destroy(scanner);

	return pobj;
}

char *getASTText(void *node, asttype t)
{
	paren *pobj;
	char *leftT = NULL, *rightT = NULL, *myT = NULL, *retT = NULL;
	short symboladdings = 0;
	if(node == NULL)		
		return "";
	if(t == PARENTHESIS)
	{
		pobj = (paren*)node;
		if(pobj->type == PAREN || pobj->type == BRACK)
		{
			symboladdings = 5; //()\n\t\0 OR []\n\t\0
		}
		else 
			symboladdings = 2; //\t\0

		leftT = getASTText(pobj->left, PARENTHESIS);
		rightT = getASTText(pobj->right, PARENTHESIS);
		retT = (char*)malloc(strlen(leftT)+strlen(rightT)+symboladdings);
		*retT = 0;
		if(pobj->type == PAREN)
			strcat(retT, "()\n\t"); 
		else if(pobj->type == BRACK)
			strcat(retT, "[]\n\t"); 
		else
		{
			strcat(retT, "");
		}
		strcat(retT,leftT);	
		if(pobj->type == NONE)
		{
			strcat(retT,"\t");
			strcat(retT,rightT);	
		}
	}
	return retT;
	//else if()
}

void printAST(paren *node, int indent)
{
	int i = 0;
	if (node == NULL)
		return;
	if(node->type == PAREN)
	{
		for (i = 0; i< indent; i++)
			cout << "\t";

		cout << "()\n";
		printAST(node->left, indent + 1);
	}
	else if(node->type == BRACK)
	{
		for (i = 0; i< indent; i++)
			cout <<"\t";

		cout << "[]\n";
		printAST(node->left, indent + 1);
	}
	else if(node->type == CBRACK)
	{
		for (i = 0; i< indent; i++)
			cout << "\t";

		cout << "{}\n";
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
		cout << "Error reading file " << filename <<"\n";
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
		cout << "Error! Unable to read the file " << filename << "!\n";
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
		cout << "Error! a filename to statically analyize is expected!\n";
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

