#ifndef _str_h
#define _str_h

// Convert string to integer
bool intval(const char *string, s32 *return_value);

// Convert integer to static string
char *strval(s32 num);

// Remove last character of string
void chop(char *string);

#endif

