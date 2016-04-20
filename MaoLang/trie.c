#include "stdafx.h"
#include "trie.h"
#include "manipulate.h"
#include "parser.h"

void AddToTrie(Trienode* current_node, char * lexeme, char* type, void* value)
{
	if (strlen(lexeme) == 0)
		return;

	int index;
	ChangeToIndex(lexeme[0], &index);

	if (current_node->child[index] == NULL)
	{
		current_node->child[index] = NewNode();
		current_node->child[index]->node_char = lexeme[0];
	}

	lexeme += 1;

	if (strlen(lexeme) == 0)
	{
		if (strcmp(type, "int") == 0)
		{
			current_node->child[index]->type = int_var;
			*((int*)current_node->child[index]->value) = *((int*)value);
		}
		else if (strcmp(type, "double") == 0)
		{
			current_node->child[index]->type = float_var;
			*((double*)current_node->child[index]->value) = *((double*)value);
		}
	}

	AddToTrie(current_node->child[index], lexeme, type, value);
	
	return;
}

void GetValue(Trienode* current_node, char* lexeme, char* type, void* value)
{
	int index;
	ChangeToIndex(lexeme[0], &index);
	if (current_node->child[index] != NULL)
	{
		lexeme += 1;
		if (strlen(lexeme) == 0)
		{
			if (current_node->child[index]->type == int_var)
			{
				strcpy(type, "int");
				*((int*)value) = *((int*)current_node->child[index]->value);
			}
			else if (current_node->child[index]->type == float_var)
			{
				strcpy(type, "double");
				*((double*)value) = *((double*)current_node->child[index]->value);
			}
		}
		else
		{
			GetValue(current_node->child[index], lexeme, type, value);
		}
	}
	else
	{
		printf("Undefined variable!\n");
		return;
	}
}

void SetValue(Trienode* current_node, char* lexeme, void* value)
{
	int index;
	ChangeToIndex(lexeme[0], &index);
	if (current_node->child[index] != NULL)
	{
		lexeme += 1;
		if (strlen(lexeme) == 0)
		{
			if (current_node->child[index]->type == int_var)
			{
				*((int*)current_node->child[index]->value) = (int)*((double*)value);
			}
			else if (current_node->child[index]->type == float_var)
			{
				*((double*)current_node->child[index]->value) = *((double*)value);
			}
		}
		else
		{
			SetValue(current_node->child[index], lexeme, value);
		}
	}
	return;
}

Trienode* NewNode(void)
{
	Trienode* root = (Trienode*)malloc(sizeof(Trienode));

	for (int i = 0; i < LETTER_DIGIT_MAX; i++)
	{
		root->child[i] = NULL;
	}
	root->node_char = '\0';
	root->type = none;
	root->value = (void*)malloc(sizeof(double));  //here's a problem!
	return root;
}

void PrintTrie(Trienode* root)
{
	for (int i = 0; i < LETTER_DIGIT_MAX; i++)
	{
		if (root->child[i] != NULL)
		{
			printf("%c", root->child[i]->node_char);
			if (root->child[i]->type != 0)
			{
                
				if (root->child[i]->type == 1)
				{
					printf(": %d (integer)\n", *((int*)root->child[i]->value));
				}
				else if (root->child[i]->type == 2)
				{
					printf(": %lf (double)\n", *((double*)root->child[i]->value));
				}
			}

			PrintTrie(root->child[i]);
		}
	}
}
