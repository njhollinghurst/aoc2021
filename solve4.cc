#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>

std::vector<int> draw;

struct Board {
	unsigned marks;
	int numbers[5][5];
	bool done;
};

std::vector<Board> boards;

void rd()
{
	int c, i, j;
	c = fgetc(stdin);
	while (c!='\r' && c !='\n') {
		if (c != ',') ungetc(c, stdin);
		assert(scanf("%d", &i) > 0);
		draw.push_back(i);
		c = fgetc(stdin);
	}
	while (!feof(stdin)) {
		Board b;
		b.marks = 0;
		b.done = false;
		for(j = 0; j < 25; ++j) {
			if (scanf("%d", &b.numbers[j/5][j%5]) <= 0) 
				break;
		}
		assert(j == 0 || j == 25);
		if (j == 0) break;
		boards.push_back(b);
	}
}

long eval(Board const & b)
{
	bool house = false;
	for(int x = 0; x < 5; ++x) {
		if (!((0x108421 << x) & ~b.marks) ||
		    !((0x1F << (5*x)) & ~b.marks)) {
			house = true;
			break;
		}
	}
	if (!house) return -1;
	unsigned long score = 0;
	for(int y = 0; y < 5; ++y) {
		for(int x = 0; x < 5; ++x) {
			if (!(b.marks & (1 << (5*y+x))))
				score += b.numbers[y][x];
		}
	}
	return score;
}

void call(int i)
{
	for (unsigned b = 0; b < boards.size(); ++b) {
		if (boards[b].done) continue;
		bool marked = false;
		for(int y = 0; y < 5; ++y) {
			for(int x = 0; x < 5; ++x) {
				if (boards[b].numbers[y][x] == i) {
					boards[b].marks |= (1 << (5*y+x));
					marked = true;
				}
			}
		}
		if (marked) {
			long score = eval(boards[b]);
			if (score >= 0) {
				score *= i;
				printf("Board %d wins with score %d\n", b+1, score);
				boards[b].done = true;
			}
		}

	}
}

int main()
{
	rd();
	printf("There are %u boards and %u numbers\n", boards.size(), draw.size());

	for(std::vector<int>::const_iterator ix = draw.begin(); ix != draw.end(); ix++) {
		call(*ix);
	}


	return 0;
}
