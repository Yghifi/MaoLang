//functions definitions
#include "stdafx.h"
#include "manipulate.h"

bool IsPrefix(char** input_string, const char* text)
{
	int _len_text = strlen(text);
	char* _read_string = *input_string;

	//remove blanks
	while (*_read_string == ' ')
	{
		_read_string++;
	}

	if (strncmp(_read_string, text, _len_text) == 0)
	{
		*input_string = _read_string + _len_text;
		return true;
	}
	else
	{
		return false;
	}
}

bool IdCheck(char** input_string)
{
	bool is_valid = true;
	char* _pchar = *input_string + strlen(*input_string) - 1;
	
	while (**input_string == ' ')
	{
		(*input_string)++;
	}
	while (*_pchar == ' ' || *_pchar == ';')
	{
		_pchar--;
	}

	if (*_pchar == ';')
	{
		*_pchar = '\0';
	}
	else 
	{
		*(_pchar + 1) = '\0';
	}

	_pchar = *input_string;
	while (*_pchar != '\0')
	{
		if (!IsAlpha(*_pchar) && !IsDigit(*_pchar))
		{
			is_valid = false;
		}
		_pchar++;
	}

	return is_valid;
}

char IsOperator(char* input_string)
{
	if (IsPrefix(&input_string, "+") || IsPrefix(&input_string, "-") || IsPrefix(&input_string, "*") || IsPrefix(&input_string, "/"))
	{
		//roll back;
		return input_string[-1];
	}
	else
	{
		return 0;
	}
}

bool IsAlpha(char _ch)
{
	if ('A' <= _ch && _ch <= 'Z' || 'a' <= _ch && _ch <= 'z')
		return true;
	else
		return false;
}

bool IsDigit(char _ch)
{
	if ('0' <= _ch && _ch <= '9')
		return true;
	else
		return false;
}

void ChangeToIndex(char _ch, int* index)
{
	if (IsAlpha(_ch) && ('a' <= _ch && _ch <= 'z'))
	{
		*index = _ch - 'a';
	}
	else if (IsAlpha(_ch) && ('A' <= _ch && _ch <= 'Z'))
	{
		*index = _ch - 'A' + 26;
	}
	else if (IsDigit(_ch))
	{
		*index = _ch - '0' + 52;
	}
	return;
}

char* GetSubstring(const char* src, int pos, int len)
{
	char* _des = (char*)malloc(strlen(src) * sizeof(char));
	int _i = 0;

	while (len--)
		_des[_i++] = src[pos++];

	_des[_i] = '\0';

	return _des;
}
