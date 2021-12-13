#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <algorithm>

using namespace std;

typedef pair<unsigned,unsigned> POINT;
set<POINT> points;

POINT maxpoint(0,0);

void fold(char dir, unsigned val)
{
  set <POINT> after;
  unsigned v2 = 2*val;

  for(auto p : points) {
    unsigned x, y;
    x = p.first;
    y = p.second;
    if (dir=='x' && x >= val) {
      assert(x <= v2);
      x = v2-x;
    }
    if (dir=='y' && y >= val) {
      assert(y <= v2);
      y = v2-y;
    }
    after.insert(POINT(x,y));
  }
  points = move(after);
  printf("After fold %c=%u, there are %u points\n", dir, val,
         (unsigned)(points.size()));
  if (dir=='x' && val < maxpoint.first) maxpoint.first = val;
  if (dir=='y' && val < maxpoint.second) maxpoint.second = val;
}

int main()
{
  char str[1024];
  char dir;
  unsigned x, y;

  while (fgets(str, 1024, stdin) == str) {
    if (sscanf(str, "%u,%u", &x, &y) >= 2) {
      points.insert(POINT(x,y));
      if (x > maxpoint.first) maxpoint.first = x;
      if (y > maxpoint.second) maxpoint.second = y;
    }
    else if (sscanf(str, "fold along %c=%u", &dir, &x) >= 2) {
      assert(dir=='x' || dir=='y');
      fold(dir, x);
    }
  }

  assert(maxpoint.first < 1023);
  for(y = 0; y <= maxpoint.second; ++y) {
    memset(str, '.', maxpoint.first+1);
    str[maxpoint.first+1] = '\0';
    for(auto p : points) {
      if (p.second == y) str[p.first]='#';
    }
    printf("%s\n", str);
  }

  return 0;
}


