#pragma once

#ifndef _PARSER_H_
#define _PARSER_H_

#include "trie.h"

typedef struct _expression
{
	double result;		//Here...
	struct _expression* left_expr;
	struct _expression* right_expr;
	
	bool is_int;
	bool is_float;
	bool is_variable;

	int sign;
	void* value;		//and here, how to do it better?
	char* varname;
	char operat;

} Expression;

Expression* NewExpr(void);

void PrintRPN(Expression* expr);

void PrintExprResult(Expression* expr);

void GetResult(Expression* expr, Trienode* var_map);

Expression* GetVariable(char** input_string);

Expression* GetNumber(char** input_string);

Expression* GetFactor(char** input_string);

Expression* GetTerm(char** input_string);

Expression* GetExpr(char** input_string);

Expression* GetAssign(char** input_string);

#endif
