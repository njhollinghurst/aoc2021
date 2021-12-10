#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

char state[1024];
unsigned depth = 0;
unsigned bad = 0;
unsigned line = 1;
unsigned long long total_bad = 0;

unsigned long long completions[1024];
unsigned ncompletions = 0;

static int mydiff(const void * a, const void * b)
{
	unsigned long long aval = *(unsigned long long const *)a;
	unsigned long long bval = *(unsigned long long const *)b;
	return (aval < bval) ? -1 : (aval == bval) ? 0 : 1;

}

int main()
{
	for(;;) {
		int c = getchar();
		if (c <= 0 || c=='\n') {
			if (!bad && depth > 0) {
				unsigned long long score = 0;
				while (depth > 0) {
					score *= 5;
					switch (state[--depth]) {
						case '(': score += 1; break;
						case '[': score += 2; break;
						case '{': score += 3; break;
						case '<': score += 4; break;
						default: assert(0);
					}
				}
				assert(ncompletions < 1024);
				completions[ncompletions++] = score;
			}
			depth = 0;
			bad = 0;
			if (c <= 0) break;
			line++;
			continue;
		}

		if (bad || isspace(c))
			continue;

		if (c=='(' || c=='[' || c=='{' || c=='<') {
			assert(depth < 1024);
			state[depth++] = c;
		}
		else {
			unsigned char_score = 0;
			char k = '\0';
			switch (c) {
				case ')': char_score = 3; k = '('; break;
				case ']': char_score = 57; k = '['; break;
				case '}': char_score = 1197; k = '{'; break;
				case '>': char_score = 25137; k = '<'; break;
				default: assert(0);
			}
			if (k == state[depth-1]) {
				--depth;
			} else {
				bad = char_score;
				total_bad += bad;
			}
		}
	}

	printf("Total badness: %llu\n", total_bad);
	printf("Number to complete: %u\n", ncompletions);
	assert(ncompletions & 1);
	qsort(completions, ncompletions, sizeof(completions[0]), mydiff);
	printf("Middle score: %llu\n", completions[ncompletions>>1]);
	return 0;
}
