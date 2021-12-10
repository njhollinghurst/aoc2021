#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

unsigned long long A[10] = { 0 };
unsigned long long total = 0;

void rd()
{
	int i;
	while (scanf("%d", &i)>0) {
		assert(i>=0 && i < 10);
		A[i]++;
		total++;
		i = fgetc(stdin);
		if (i>0 && i!=',') ungetc(i, stdin);
	}
}

void gen()
{
	int i;
	unsigned long long foo = A[0];

	for(i = 0; i < 9; ++i)
		A[i] = A[i+1];
	A[9] = 0;

	A[6] += foo;
	A[8] += foo;
	total += foo;
}

int main(int argc, char **argv)
{
	int ngen = atoi(argv[1]);
	rd();
	printf("Initial count %llu\n", total);
	for(int i = 0; i < ngen; ++i) gen();
	printf("After %d days, %llu\n", ngen, total);
	return 0;
}
