#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
int main()
{
	char str[256];
	int xpos = 0, aim = 0, depth = 0;
	while (fgets(str, 256, stdin)==str) {
		char *ptr = str;
		char d = *ptr++;
		int amt;
		while (*ptr && !isdigit(*ptr)) ++ptr;
		amt = atoi(ptr);
		if (d=='f') xpos += amt, depth += amt*aim;
		else if (d=='d') aim += amt;
		else if (d=='u') aim -= amt;
		else printf("BAD\n");
	}
	printf("%d,%d -> %d\n", xpos, aim,  xpos*aim);
	printf("%d,%d -> %d\n", xpos, depth, xpos*depth);
	return 0;
}
