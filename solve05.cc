#include <stdio.h>
#include <malloc.h>
#include <assert.h>

int H = 0;
int W = 0;
unsigned short ** p = 0;
#define SUS sizeof(unsigned short)

void chk_size(int rows, int cols)
{
	rows++;
	cols++;
	if (cols > W) {
		for(int i = 0; i < H; ++i) {
			p[i] = (unsigned short *)realloc(p[i], SUS*cols);
			for(int j = W; j < cols; ++j) p[i][j] = 0;
		}
		W = cols;
	}
	if (rows > H) {
		p = (unsigned short **)realloc(p, sizeof(unsigned short *)*rows);
		for(int i = H; i < rows; ++i) {
			p[i] = (unsigned short *)calloc(SUS, W);
		}
		H = rows;
	}
	
}

void free_all()
{
	for(int i = H-1; i >= 0; --i) free(p[i]);
	free(p);
}

void rd()
{
	int x1, y1, x2, y2;
	while (scanf("%d,%d -> %d,%d", &x1, &y1, &x2, &y2) >= 4) {
		chk_size(y1, x1);
		chk_size(y2, x2);
		printf("%d,%d -> %d,%d (W=%d,H=%d)\n", x1, y1, x2, y2, W, H);
		if (y2 < y1) {
			y1^=y2^=y1^=y2;
			x1^=x2^=x1^=x2;
		}
		if (x1==x2) {
			while (y1 <= y2) p[y1][x1]++, y1++;
		}
		else if (y1==y2) {
			if (x1>x2) x1^=x2^=x1^=x2;
			while (x1 <= x2) p[y1][x1]++, x1++;
		}
		else if (x2 > x1) {
			assert(x2-x1 == y2-y1);
			while (y1 <= y2) p[y1][x1]++, y1++, x1++;
		}
		else {
			assert(x1-x2 == y2-y1);
			while (y1 <= y2) p[y1][x1]++, y1++, x1--;
		}
	}
}

int count_overlaps()
{
	int n = 0;
	for(int y = 0; y < H; ++y) {
		for(int x = 0; x < W; ++x) {
			printf("%c", (p[y][x])?'0'+p[y][x]:'.');
			if (p[y][x]>1) ++n;
		}
		printf("\n");
	}
	printf("\n");
	return n;
}

int main()
{
	rd();
	int i = count_overlaps();
	printf("Overlap in %d points\n", i);
	free_all();
	return 0;
}
