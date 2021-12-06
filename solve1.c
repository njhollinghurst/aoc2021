#include <stdio.h>
int main()
{
	int cur, num=0, count=0, c2=0;
	int p1=0, p2=0, p3=0;
	while (scanf("%d", &cur) > 0) {
		if (num >= 1 && cur>p1) count++;
		if (num >= 3 && cur>p3) c2++;
		p3 = p2;
		p2 = p1;
		p1 = cur;
		num++;
	}
	printf("%d, %d\n", count, c2);
	return 0;
}
