#include <nds.h>
#include <string.h>
#include "str.h"

bool intval(const char *string, s32 *return_value) {
	bool negative = false;
	u32 current_char = 0;

	if (string[0] == '-') {
		negative = true;
		current_char = 1;
	}

	s32 num = 0;
	while (string[current_char]) {
		if (string[current_char] < '0' || string[current_char] > '9') {
			return false;
		}

		num *= 10;
		num += (u8)string[current_char] - '0';
		current_char++;
	}

	if (negative) {
		num = -num;
	}

	*return_value = num;
	return true;
}

char *strval(s32 num) {
	char numstring[12] = "";
	static char string[12];
	u32 quotient = 1;
	u32 remainder = 0;
	char remainder_str[3] = "";
	u32 current_char = 0, current_char2 = 0;
	s32 number;

	if (num < 0) {
		number = -num;
		string[0] = '-';
		current_char2 = 1;
	} else {
		number = num;
	}
	
	if (number == 0) {
		strcpy(string, "0");
	} else {
		while (quotient != 0) {
			remainder = number % 10;
			quotient = number / 10;
			remainder_str[0] = remainder+48;
			remainder_str[1] = '\0';
			strcat(numstring, remainder_str);
			number = quotient;
		}
		
		current_char = strlen(numstring)-1;
		while (current_char != 0) {
			string[current_char2] = numstring[current_char];
			current_char--;
			current_char2++;
		}
		string[current_char2] = numstring[current_char];
		string[current_char2 + 1] = '\0';
	}
	
	return string;
}

void chop(char *string) {
	if (string[0]) {
		string[strlen(string) - 1] = '\0';
	}
}

