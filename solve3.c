#include <stdio.h>
#include <assert.h>

static unsigned history[65536];
int num = 0, len = 32;
int zeros[32] = { 0 };
int ones[32] = { 0 };

void read_input()
{
	char s[40];
	int i;
	while (fgets(s, 40, stdin)==s) {
		unsigned val = 0;
		for(i = 0; i < len; ++i) {
			val <<= 1;
			if (s[i]=='0') ++zeros[i];
			else if (s[i]=='1') ++ones[i],++val;
			else {
				assert(s[i] <= ' ');
				assert(i == len || len >= 32);
				len = i;
				break;
			}
		}
		assert(num < 65536);
		history[num++] = val;
	}
}

int count(unsigned mask, unsigned match, unsigned * matchval)
{
	int i, c = 0;
	match &= mask;
	int ix = 0;
	for(i = 0; i < num; ++i) {
		if ((history[i] & mask) == match) {
			ix = i;
			++c;
		}
	}
	if (matchval) *matchval = history[ix];
	return c;
}

int main()
{
	unsigned long gamma, epsilon;
	int i;

	read_input();
	gamma = 0;
	for(i = 0; i < len; ++i) {
		assert(ones[i]!=zeros[i]);
		if (ones[i]>zeros[i]) gamma += 1ul << (len-1-i);
	}
	epsilon = (1ul<<len)-1ul-gamma;
	printf("Num = %d, Len = %d, Gamma = %lu, Epsilon = %lu, product = %llu\n", num, len, gamma, epsilon, gamma*(unsigned long long)epsilon);

	gamma = 0;
	epsilon = 0;
	for(int rtype = 0; rtype < 2; ++rtype) {
		unsigned mask  = 0;
		unsigned match = 0;
		for(i = 0; i <= len; ++i) {
			unsigned thebit = 1ul << (len-1-i);
			unsigned matchval;
			int n = count(mask, match, &matchval);
			if (n == 1) {
				if (rtype == 0) gamma = matchval;
				epsilon = matchval;
				printf("Match: %d\n", matchval);
				break;
			}
			assert(n > 1 && i < len);
			mask |= thebit;
			int n1 = count(mask, match|thebit, 0);
			int n0 = n-n1;
			if (rtype ? (n1 < n0) : (n1 >= n0)) {
				match |= thebit;
			}
		}
	}
	printf("%llu\n", gamma*(unsigned long long)epsilon);

	return 0;
}
