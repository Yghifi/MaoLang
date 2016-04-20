#include "stdafx.h"
#include "parser.h"
#include "manipulate.h"
#include <math.h>

Expression* NewExpr(void)
{
	Expression* expr = (Expression*)malloc(sizeof(Expression));
	
	expr->is_int = false;
	expr->is_float = false;
	expr->is_variable = false;
	expr->left_expr = NULL;
	expr->right_expr = NULL;
	expr->result = 0.0;	//Is it Okay?
	expr->sign = 1;
	expr->value = malloc(sizeof(double));
	expr->operat = '\0';
	expr->varname = (char*)malloc(WORD_LENGTH_MAX*sizeof(char));
	expr->varname[0] = '\0';
	return expr;
}

Expression* SetExpr(Expression* expr, char operat, Expression* left, Expression* right)
{
	expr->is_int = false;
	expr->is_float = false;
	expr->is_variable = false;
	*((int*)expr->value) = 0;
	*((double*)expr->value) = 0;
	expr->operat = operat;
	expr->left_expr = left;
	expr->right_expr = right;

	return expr;
}

void PrintRPN(Expression* expr)
{
	if (expr->is_int && !expr->is_variable && expr->operat == '\0')
	{
		printf("%d ", *((double*)expr->value));
		
	}
	else if (expr->is_float && !expr->is_variable && expr->operat == '\0')
	{
		printf("%.6lf ", *((double*)expr->value));
	}
	else if (expr->is_variable)
	{
		printf("%s ", expr->varname);
	}
	else
	{
		//post-order travelsal
		PrintRPN(expr->left_expr);
		printf(" ");
		PrintRPN(expr->right_expr);
		printf(" ");
		printf("%c ", expr->operat);
	}
}

void PrintExprResult(Expression* expr)
{
	if (expr->is_int)
	{
		printf("%d\n", (int)expr->result);
	}
	else
	{
		printf("%.6lf\n", expr->result);
	}
	return;
}

void GetResult(Expression* expr, Trienode* var_map)
{
	if (expr->is_int && !expr->is_variable && expr->operat == '\0')
	{
		expr->result = *((int*)expr->value) * expr->sign;
	}
	else if (expr->is_float && !expr->is_variable && expr->operat == '\0')
	{
		expr->result = *((double*)expr->value) * expr->sign;
	}
	else if (expr->is_variable)
	{
		char* _type = (char*)malloc(WORD_LENGTH_MAX * sizeof(char));
		void* _value = (void*)malloc(sizeof(double));
		GetValue(var_map, expr->varname, _type, _value);
		if (strcmp(_type, "int") == 0)
		{
			expr->is_int = true;
			expr->result = *((int*)_value) * expr->sign;
		}
		else if (strcmp(_type, "double") == 0)
		{
			expr->is_float = true;
			expr->result = *((double*)_value) * expr->sign;
		}
	}
	else
	{
		if (expr->operat == '=')
		{
			GetResult(expr->right_expr, var_map);
			SetValue(var_map, expr->left_expr->varname, &expr->right_expr->result);
			expr->result = expr->right_expr->result;
		}
		else
		{
			//post-order travelsal
			GetResult(expr->left_expr, var_map);
			GetResult(expr->right_expr, var_map);
			if (expr->left_expr->is_float || expr->right_expr->is_float)
			{
				expr->is_float = true;
			}
			else
			{
				expr->is_int = true;
			}
			switch (expr->operat)
			{
			case '+':
				expr->result = (expr->left_expr->result + expr->right_expr->result) * expr->sign;
				break;
			case '-':
				expr->result = (expr->left_expr->result - expr->right_expr->result) * expr->sign;
				break;
			case '*':
				expr->result = (expr->left_expr->result * expr->right_expr->result) * expr->sign;
				break;
			case '/':
				if (fabs(expr->right_expr->result - 0) < 1e-12)
				{
					printf("divided by ZERO\n");
					exit(0);	//According to the project requirement;
				}
				else
				{
					expr->result = (expr->left_expr->result / expr->right_expr->result) * expr->sign;
					if (expr->is_int)
					{
						expr->result = trunc(expr->result);
					}
				}
				break;

			default:
				printf("Error!\n");
				break;
			}
		}
	}
	return;
}

Expression* GetVariable(char** input_string)
{
	char* _read_string = *input_string;
	char* _temp_varname = (char*)malloc(WORD_LENGTH_MAX * sizeof(char));
	int _len = 0;
	int _sign = 1;
	bool is_begin = true;
	bool is_var = false;

	while (*_read_string == ' ')
	{
		_read_string++;
	}

	while (1)
	{
		char c = *_read_string;
		if ((IsAlpha(c) && is_begin) || ((IsAlpha(c) || IsDigit(c)) && !is_begin))
		{
			_temp_varname[_len] = c;
			_len++;
			_temp_varname[_len] = '\0';
			is_begin = false;
			is_var = true;
		}
		else if ((c == '-' || c == '+') && is_begin)
		{
			if (c == '-')
				_sign = -1;
		}
		else
		{
			break;
		}
		_read_string++;
	}
	if (is_var)
	{
		*input_string = _read_string;
		Expression* _expr_var = NewExpr();
		_expr_var->is_variable = true;
		_expr_var->sign = _sign;
		strcpy(_expr_var->varname, _temp_varname);
		
		return _expr_var;
	}
	else
	{
		return NULL;
	}
}

Expression* GetNumber(char** input_string)
{
	int _value_i = 0;
	double _value_f = 0.0;
	double _k;
	int _sign = 1;
	bool is_number = false;
	bool is_float = false;
	bool is_begin = true;

	char* _read_string = *input_string;

	while (*_read_string == ' ')
	{
		_read_string++;
	}

	while (1)
	{
		char c = *_read_string;
		if (IsDigit(c))
		{
			if (is_float)
			{
				_value_f += _k * (c - '0');
				_k *= 0.1;
				is_number = true;
			}
			else
			{
				_value_i = 10 * _value_i + (c - '0');
			}
			is_number = true;
			is_begin = false;
		}
		else if (c == '.')
		{
			_value_f = _value_i;
			_k = 0.1;
			is_float = true;
		}
		else if (c == '-' && is_begin)
		{
			_sign = -1;
		}
		else if (c == '+' && is_begin)
		{
			_sign = 1;
		}
		else
		{
			break;
		}
		_read_string++;
	}
	if (is_number)
	{
		*input_string = _read_string;
		Expression* _expr_num = NewExpr();
		_expr_num->sign = _sign;
		if (is_float)
		{
			_expr_num->is_float = true;
			*((double*)_expr_num->value) = _value_f;
		}
		else
		{
			_expr_num->is_int = true;
			*((int*)_expr_num->value) = _value_i;
		}
		return _expr_num;
	}
	else
	{
		//printf("Expression missing here!\n");
		return NULL;
	}
}

Expression* GetFactor(char** input_string)
{
	int _sign = 1;

	Expression* _expr_fac = NewExpr();

	if (_expr_fac = GetNumber(input_string))
	{
		return _expr_fac;
	}
	else if (_expr_fac = GetVariable(input_string))
	{
		return _expr_fac;
	}
	else
	{
		char* _read_string = *input_string;

		if (IsPrefix(&_read_string, "-"))
		{
			_sign = -1;
		}
		else if (IsPrefix(&_read_string, "+"))
		{
			_sign = 1;
		}

		if (IsPrefix(&_read_string, "("))
		{
			_expr_fac = GetAssign(&_read_string);
			if (IsPrefix(&_read_string, ")"))
			{
				_expr_fac->sign = _sign;
				*input_string = _read_string;
				return _expr_fac;
			}
			else
			{
				free(_expr_fac);
				printf("\")\" missing here\n");
				return NULL;
			}
		}
		else
		{
			free(_expr_fac);
			//printf("Expression missing here!(GF)\n");
			return NULL;
		}
	}
}

Expression* GetTerm(char** input_string)
{
	Expression* _expr_term = NewExpr();
	char* _read_string = *input_string;

	_expr_term = GetFactor(&_read_string);

	while (1)
	{
		char _operat = '\0';
		if (IsPrefix(&_read_string, "*"))
		{
			_operat = '*';
		}
		else if (IsPrefix(&_read_string, "/"))
		{
			_operat = '/';
		}
		else
			break;

		if (_operat != '\0')
		{
			if (_expr_term != NULL)
			{
				Expression* _new_expr = NewExpr();
				_expr_term = SetExpr(_new_expr, _operat, _expr_term, GetFactor(&_read_string));
			}
			else
			{
				free(_expr_term);
				//printf("Expression missing here!(GT)\n");
				return NULL;
			}
		}
	}
	*input_string = _read_string;
	return _expr_term;
}

Expression* GetExpr(char** input_string)
{
	Expression* _expr = NewExpr();
	char* _read_string = *input_string;

	_expr = GetTerm(&_read_string);

	while (1)
	{
		char _operat = '\0';
		if (IsPrefix(&_read_string, "+"))
		{
			_operat = '+';
		}
		else if (IsPrefix(&_read_string, "-"))
		{
			_operat = '-';
		}
		else
			break;


		if (_operat != '\0')
		{
			if (_expr != NULL)
			{
				Expression* _new_expr = NewExpr();
				_expr = SetExpr(_new_expr, _operat, _expr, GetTerm(&_read_string));
			}
			else
			{
				free(_expr);
				//printf("Expression missing here!(GE)\n");
				return NULL;
			}
		}
	}
	*input_string = _read_string;
	return _expr;
}

Expression* GetAssign(char** input_string)
{
	Expression* _expr_assign = NewExpr();
	char* _read_string = *input_string;

	_expr_assign = GetFactor(&_read_string);

	if (_expr_assign->is_variable)
	{
			char _operat = '\0';
			if (IsPrefix(&_read_string, "="))
			{
				_operat = '=';
			}
			else
			{
				_read_string = *input_string;
				Expression* _new_expr = NewExpr();
				if (_new_expr = GetExpr(&_read_string))
				{
					_expr_assign = _new_expr;
				}

			}

			if (_operat != '\0')
			{
				if (_expr_assign != NULL)
				{
					Expression* _new_expr = NewExpr();
					_expr_assign = SetExpr(_new_expr, _operat, _expr_assign, GetAssign(&_read_string));
				}
				else
				{
					free(_expr_assign);
					//printf("Assignment missing here!(GA)\n");
					return NULL;
				}
			}
	}
	else
	{
		_read_string = *input_string;
		Expression* _new_expr = NewExpr();
		if (_new_expr = GetExpr(&_read_string))
		{
			_expr_assign = _new_expr;
		}
	}
	*input_string = _read_string;
	return _expr_assign;
}
