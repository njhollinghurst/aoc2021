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

unsigned total_flashes = 0;

unsigned step()
{
  unsigned old_total = total_flashes; // added for task 2

  for(unsigned y = 0; y < h; ++y) {
    for(unsigned x = 0; x < w; ++x) {
      assert(isdigit(data[y][x]));
      data[y][x]++;
    }
  }

  bool any_flash;
  do {
    any_flash = false;
    for(unsigned y = 0; y < h; ++y) {
      for(unsigned x = 0; x < w; ++x) {
	if (data[y][x] > '9') {
	  data[y][x] = '0';
	  for(unsigned yy = (y>0)?y-1:y; yy < h && yy <= y+1; ++yy) {
	    for(unsigned xx = (x>0)?x-1:x; xx < w && xx <= x+1; ++xx) {
	      if (data[yy][xx] != '0')
                data[yy][xx]++;
	    }
	  }
	  total_flashes++;
	  any_flash = true;
	}
      }
    }
  } while (any_flash);

  return total_flashes - old_total; // added for task 2
}


int main()
{
  rd();

  bool seen_all = false; // added for task 2
  for(unsigned i = 0; i < 100 || !seen_all; ++i) {
    unsigned n = step();
    if (n == w*h) {
      printf("All flash at %u\n", i+1);
      seen_all = true;
    }
    if (i+1 == 100)
      printf("After 100 steps: %d\n", total_flashes);
  }

  return 0;
}
