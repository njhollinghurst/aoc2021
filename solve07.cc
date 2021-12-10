#include <stdio.h>
#include <limits.h>
#include <vector>

std::vector<int> crabs;
int minpos = INT_MAX;
int maxpos = INT_MIN;

void rd()
{
	int i;
	while (scanf("%d", &i) >= 0) {
		crabs.push_back(i);
		if (i > maxpos) maxpos = i;
		if (i < minpos) minpos = i;
		i = fgetc(stdin);
		if (i > 0 && i != ',') ungetc(i, stdin);
	}
}

int opt()
{
	int best = minpos, best_cost = INT_MAX;
	for(int i = minpos; i <= maxpos; ++i) {
		int cost = 0;
		for(unsigned j = 0; j < crabs.size(); ++j) {
			int d = crabs[j] - i;
			if (d < 0) d = -d;
			cost += d;
			if (cost > best_cost) break;
		}
		if (cost < best_cost) best=i, best_cost=cost;
	}
	printf("Best position %d\n", best);
	return best_cost;
}

int opt2()
{
	int best = minpos, best_cost = INT_MAX;
	for(int i = minpos; i <= maxpos; ++i) {
		int cost = 0;
		for(unsigned j = 0; j < crabs.size(); ++j) {
			int d = crabs[j] - i;
			if (d < 0) d = -d;
			d = d*(d+1) >> 1; // <-- cost function
			cost += d;
			if (cost > best_cost) break;
		}
		if (cost < best_cost) best=i, best_cost=cost;
	}
	printf("Best position %d\n", best);
	return best_cost;
}


int main()
{
	rd();
	printf("There are %u crabs\n", crabs.size());
	printf("Best cost %d\n", opt());
	printf("Part two: %d\n", opt2());
	return 0;
}

