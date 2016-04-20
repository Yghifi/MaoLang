#include "stdafx.h"
#include "trie.h"
#include "parser.h"
#include "scanner.h"
#include "manipulate.h"


int main(int argc, char* argv[])
{

	//file open
	char* file_name = argv[1];
	//char* file_name = "test\\mytest.mao";
	if (file_name != NULL)
	{
		freopen(file_name, "r", stdin);
	}
	else
	{
		printf("Can not open file!\n", stderr);
		exit(1);
	}

	Trienode* var_map = (Trienode *)malloc(sizeof(Trienode)); //variables map
	char* stmt = (char*)malloc(1000 * sizeof(char)); //abbreviation for "statement"
	bool flag = false;
	char* type = (char*)malloc(10 * sizeof(char));
	var_map = NewNode();

	while (1)
	{
		if ((gets(stmt)) == NULL)
		{
			break;
		}

		scan(var_map, stmt);
	}

	//PrintTrie(var_map);
	fclose(stdin);
	return 0;
}
