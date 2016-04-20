#include "stdafx.h"
#include "scanner.h"
#include "trie.h"
#include "parser.h"
#include "manipulate.h"


void scan(Trienode* var_map, char* stmt)
{
	char *_temp_id = (char *)malloc(LINE_LENGTH_MAX * sizeof(char));
	char *_type = (char *)malloc(WORD_LENGTH_MAX * sizeof(char));
	void *_value = NULL;
	char *_stmt_o = stmt;
	

	if (IsPrefix(&stmt, "int") || IsPrefix(&stmt, "double"))		
	{
        //variables declaration
		int _tt = 0;
		double _ff = 0.0;

		if (IsPrefix(&_stmt_o, "int"))
		{
			_type = "int";
			_value = &_tt;
		}
		else
		{
			_type = "double";
			_value = &_ff;
		}

		_temp_id = strtok(stmt, ",");
		while (_temp_id != NULL)
		{
			if (!IdCheck(&_temp_id))
			{
				printf("Invalid identifier!\n");
				break;
			}
		  
			AddToTrie(var_map, _temp_id, _type, _value);

			_temp_id = strtok(NULL, ",");
		}
	}
	else if (IsPrefix(&stmt, "print"))
	{
		//print statements

		char* _temp_stmt = (char*)malloc(LINE_LENGTH_MAX * sizeof(char));
		int _pos_i = 0, _pos_j = strlen(stmt);

		while (stmt[_pos_i] != '(')
			_pos_i++;
		while (stmt[_pos_j] != ')')
			_pos_j--;
		if (strchr(stmt, '(') == NULL)
			printf("\"(\" is missing here!\n");
		if (strchr(stmt, ')') == NULL)
			printf("\")\" is missing here!\n");

		_temp_stmt = GetSubstring(stmt, _pos_i + 1, _pos_j - _pos_i - 1);
		//printf("%s\n", _temp_stmt);
	
	
		Expression* _expr = GetAssign(&_temp_stmt);

		GetResult(_expr, var_map);

		PrintExprResult(_expr);

	}
	else
	{
		//normal statements

		Expression* _expr = GetAssign(&stmt);
		
		while (*stmt == ' ' || *stmt == ';')
		{
			stmt++;
		}

		if (strlen(stmt) != 0)
		{
			free(_expr);
			printf("Extra characters!\n");
		}
		else
		{
			GetResult(_expr, var_map);
			/*
			PrintRPN(_expr);
			printf("\n");
			PrintExprResult(_expr);
			*/
		}
	}

	return;
}
