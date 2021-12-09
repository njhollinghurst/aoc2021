#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cctype>
#include <algorithm>

unsigned h, w;
std::vector<std::string> data;

void rd()
{
	std::string s, s1, s2;
	while (true) {
		std::cin >> s;
		if (std::cin.eof()) break;
		if (s.size() >= 0) {
			assert(w == 0 || w == s.size());
			data.push_back(s);
			w = s.size();
			++h;
		}
	}
}

struct Basin {
	unsigned area;
	std::vector<std::vector<bool> > map;
	Basin(unsigned w, unsigned h);
	Basin(unsigned w, unsigned h, unsigned x, unsigned y) : Basin(w,h) { grow(x,y); }
	void grow(unsigned x, unsigned y);
	bool operator<(Basin const &b2) const { return area < b2.area; }
};

Basin::Basin(unsigned w, unsigned h) : area(0)
{
	for(unsigned y = 0; y < h; ++y) {
		map.emplace_back(w, false);
	}
}

void Basin::grow(unsigned x, unsigned y)
{
	map[y][x] = true;
	area++;
	if (x > 0 && !map[y][x-1] && data[y][x-1] >= data[y][x] && data[y][x-1] != '9') {
		grow(x-1, y);
	}
	if (x+1 < w && !map[y][x+1] && data[y][x+1] >= data[y][x] && data[y][x+1] != '9') {
		grow(x+1, y);
	}
	if (y > 0 && !map[y-1][x] && data[y-1][x] >= data[y][x] && data[y-1][x] != '9') {
		grow(x, y-1);
	}
	if (y+1 < h && !map[y+1][x] && data[y+1][x] >= data[y][x] && data[y+1][x] != '9') {
		grow(x, y+1);
	}
}

std::vector<Basin> basins;

unsigned task1_and_find_basins()
{
	unsigned sum = 0;
	for(unsigned y = 0; y < h; ++y) {
		for(unsigned x = 0; x < w; ++x) {
			char c = data[y][x];
			assert(std::isdigit(c));
			if (x > 0 && c >= data[y][x-1]) continue;
			if (x+1 < w && c >= data[y][x+1]) continue;
			if (y > 0 && c >= data[y-1][x]) continue;
			if (y+1 < h && c >= data[y+1][x]) continue;
			sum += (c + 1 - '0'); 
			basins.emplace_back(w, h, x, y);
		}
	}
	return sum;
}

int main()
{
	rd();
	std::cout << "Task 1: " << task1_and_find_basins() << std::endl;
	assert(basins.size() >= 3);
	std::sort(basins.rbegin(), basins.rend()); // reverse sort
	unsigned a = basins[0].area * basins[1].area * basins[2].area;
	std::cout << "Task 2: " << a << std::endl;
	return 0;
}

