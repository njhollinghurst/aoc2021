#include <iostream>
#include <cctype>
#include <cassert>
#include <vector>
#include <string>

using namespace std;

unsigned char the_map[512];

unsigned W = 0, H = 0, phase = 0;
vector<vector<unsigned char> > grid[2];

bool bgFill = false; // Grrr!!!

void rd()
{
  string s;
  cin >> s;
  assert(!cin.eof() && s.size()==512);
  for(unsigned i = 0; i < 512; ++i) {
    assert(s[i]=='.' || s[i]=='#');
    the_map[i] = (s[i]=='#');
  }

  unsigned n = 0;
  for(;;) {
    s.clear();
    cin >> s;
    if (cin.eof()) break;
    assert(s.size() > 0 && (W == 0 || W == s.size()));
    W = s.size();
    grid[0].emplace_back(vector<unsigned char>(W));
    for(unsigned i = 0; i < W; ++i) {
      assert(s[i]=='.' || s[i]=='#');
      grid[0][H][i] = (s[i]=='#') ? 1 : 0;
      if (s[i]=='#') ++n;
    }
    H++;
  }
  cout << "Read W=" << W << ", H=" << H << ", num=" << n << endl;
}

unsigned enhance()
{
  W += 2;
  H += 2;
  phase = !phase;

  grid[phase].resize(H);
  for(unsigned i = 0; i < H; ++i) {
    grid[phase][i].resize(W, 0);
  }

  unsigned n = 0;
  for(unsigned i = 0; i < H; ++i) {
    for(unsigned j = 0; j < W; ++j) {
      unsigned ctx = 0;
      for(unsigned ii = 0; ii < 3; ++ii) {
	for(unsigned jj = 0; jj < 3; ++jj) {
	  ctx = 2*ctx;
	  if (i+ii >= 2 && i+ii < H && j+jj >= 2 && j+jj < W) {
	    ctx += ((grid[1-phase][i+ii-2][j+jj-2]) ? 1 : 0);
	  }
	  else if (bgFill) {
	    ctx++;
	  }
	}
      }
      assert(ctx >= 0 && ctx < 512);
      if (the_map[ctx]) n++;
      assert(i < H && j < W);
      grid[phase][i][j] = the_map[ctx];
    }
  }

  bgFill = the_map[bgFill ? 511 : 0];

  return n;
}


int main()
{
  rd();

  enhance();
  unsigned n = enhance();
  cout << "Task 1: " << n << endl;

  for(unsigned i = 0; i < 48; ++i)
    n = enhance();
  cout << "Task 2: " << n << endl;

  return 0;
}
