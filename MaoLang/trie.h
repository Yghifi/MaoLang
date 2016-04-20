#pragma once

#ifndef _TRIE_H_
#define _TRIE_H_

typedef enum _nodetype
{
	none, int_var, float_var, num_const
} Nodetype;

typedef struct _node
{
	struct _node *child[LETTER_DIGIT_MAX];
	char node_char;
	Nodetype type;
	void* value;
} Trienode;


Trienode* NewNode(void);

void AddToTrie(Trienode* current_node, char* lexeme, char* type, void* value);

void GetValue(Trienode* current_node, char* lexeme, char* type, void* value);

void SetValue(Trienode* current_node, char* lexeme, void* value);

void PrintTrie(Trienode* root);

#endif