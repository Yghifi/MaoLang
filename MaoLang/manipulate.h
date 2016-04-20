#pragma once

#ifndef _MANIPULATE_H_
#define _MANIPULATE_H_

bool IsPrefix(char** input_string, const char* text);

bool IdCheck(char** input_string);

char IsOperator(char* input_string);

bool IsAlpha(char _ch);

bool IsDigit(char _ch);

void ChangeToIndex(char _ch, int* index);

char* GetSubstring(const char* src, int pos, int len);

#endif
