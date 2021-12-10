#include <stdio.h>
#include <ctype.h>
#include <assert.h>

// How many instances of 2, 4, 3 and 7 segments in second part?
int do_line_part1(char const * str)
{
	int count = 0;
	while (*str && *str!='|') ++str;
	while (*str) {
		int n = 0;
		while (*str && !isalpha(*str)) ++str;
		while (*str && isalpha(*str)) ++str,++n;
		if (n==2 || n==4 || n==3 || n==7) ++count;
	}
	return count;
}

// Full solution
int do_line_part2(char const * str)
{
	unsigned char count[14];
	unsigned char pattern[14];
	unsigned char digit_pattern[10];
	signed char value[14];

	for(int i = 0; i < 10; ++i)
		digit_pattern[i] = 0;

	for(int i = 0; i < 14; ++i) {
		while (*str && !isalpha(*str)) ++str;
		assert(*str);
		count[i] = 0;
		pattern[i] = 0;
		value[i] = -1;
		while (*str && isalpha(*str)) {
			assert(*str >= 'a' && *str <= 'g');
			count[i]++;
			pattern[i] |= 1 << (*str++ - 'a');
		}
		switch (count[i]) {
		case 2:
			value[i] = 1;
			digit_pattern[1] = pattern[i];
			break;
		case 4:
			value[i] = 4;
			digit_pattern[4] = pattern[i];
			break;
		case 3:
			value[i] = 7;
			digit_pattern[7] = pattern[i];
			break;
		case 7:
			value[i] = 8;
			digit_pattern[8] = pattern[i];
			break;
		default:
			assert(count[i] == 5 || count[i] == 6);
		}
	}
	assert(digit_pattern[1] && digit_pattern[4] && digit_pattern[7] && digit_pattern[8]==0x7F);

	// second pass
	for(int i = 0; i < 14; ++i) {
		switch (count[i]) {
		case 5: // could be '2','3','5'
			if (!(digit_pattern[1] & ~pattern[i])) {
				value[i] = 3; // covers '1' => must be '3'
			}
			else {
				// does it have more than one segment in common with '4' not with '1'
				unsigned char tmp = digit_pattern[4] & ~digit_pattern[1];
				tmp &= pattern[i];
				tmp &= tmp-1;
				if (tmp) {
					value[i] = 5;
				} else {
					value[i] = 2;
				}
			}
			break;
		case 6:	// could be '0','6','9'
			if (digit_pattern[7] & ~pattern[i]) {
				value[i] = 6; // fails to cover '7'
			}
			else if (digit_pattern[4] & ~pattern[i]) {
				value[i] = 0; // fails to cover '4'
			}
			else {
				value[i] = 9;
			}
			break;
		}
	}

	printf("%d%d%d%d%d%d%d%d%d%d | %d%d%d%d\n",
			value[0], value[1], value[2], value[3], value[4],
			value[5], value[6], value[7], value[8], value[9],
			value[10], value[11], value[12], value[13]);

	return 1000*value[10] + 100*value[11] + 10*value[12] + value[13];
}


int main()
{
	char str[128];
	int count = 0, sum = 0;
	while (fgets(str, 128, stdin)==str) {
		count += do_line_part1(str);
		sum += do_line_part2(str);
	}
	printf("There were %d easy digits\n", count);
	printf("Sum: %d\n", sum);

	return 0;
}
		

