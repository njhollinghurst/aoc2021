#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<vector<char> > sea;
unsigned w, h;

void rd()
{
  string s;
  for(;;) {
    cin >> s;
    if (cin.eof()) break;
    vector<char> v;
    for(unsigned x = 0; x < s.size(); ++x) v.push_back(s[x]);
    sea.push_back(v);
  }
  h=sea.size();
  w=sea[0].size();
}

int step()
{
  int nmove = 0;
  for(unsigned y = 0; y < h; y++) {
    vector<char> v0(sea[y]);
    for(unsigned x = 0; x < w; x++) {
      if (v0[x]=='>' && v0[(x+1)%w]=='.') {
	sea[y][x]='.';
	sea[y][(x+1)%w]='>';
	nmove++;
      }
    }
  }
  vector<vector<char> > sea0(sea); // I'm being lazy
  for(unsigned y = 0; y < h; y++) {
    for(unsigned x = 0; x < w; x++) {
      if (sea0[y][x]=='v' && sea0[(y+1)%h][x]=='.') {
	sea[y][x]='.';
	sea[(y+1)%h][x]='v';
	nmove++;
      }
    }
  }

  return nmove;
}

void task1()
{
  int k = 0;
  do k++; while (step());
  cout << k << endl;
}

int main()
{
  rd();
  task1();
  return 0;
}
