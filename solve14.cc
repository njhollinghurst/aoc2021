#include <cstdio>
#include <cctype>
#include <cassert>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

struct Rule {
  unsigned i1;
  unsigned i2;
  unsigned i3;
};

vector<char> names;
vector<unsigned long long> histogram;
vector<vector<unsigned long long> > bigrams;
vector<Rule> rules;
unsigned long long length = 0;
unsigned final = UINT_MAX;

unsigned find_atom(char name)
{
  unsigned i;
  for (i = 0; i < names.size(); ++i) if (names[i]==name) break;
  if (i >= names.size()) {
    names.push_back(name);
    histogram.push_back(0);
    for(unsigned j = 0; j < bigrams.size(); ++j) bigrams[j].push_back(0);
    bigrams.emplace_back(names.size(), 0u);
  }
  return i;
}
  

void add_atom(char name)
{
  unsigned i = find_atom(name);
  length++;
  histogram[i]++;
  if (final != UINT_MAX) bigrams[final][i]++;
  final = i;
}

void add_rule(char c1, char c2, char c3)
{
  Rule r;
  r.i1 = find_atom(c1);
  r.i2 = find_atom(c2);
  r.i3 = find_atom(c3);
  rules.push_back(r);
}

void step()
{
  vector<vector<unsigned long long> > new_bigrams = bigrams;

  for(auto r : rules) {
    unsigned long long n = bigrams[r.i1][r.i2];
    if (n) {
      new_bigrams[r.i1][r.i2] -= n;
      new_bigrams[r.i1][r.i3] += n;
      new_bigrams[r.i3][r.i2] += n;
      histogram[r.i3] += n;
      length += n;
    }
  }

  bigrams = move(new_bigrams);
}

int main()
{
  int c;
  for(c = getchar(); c > 0 && c != '\n'; c = getchar()) {
    add_atom(c);
  }
  while (isspace(c)) c = getchar();
  ungetc(c, stdin);

  char cc[2], c3;
  while (scanf("%2s -> %c", cc, &c3) >= 2) {
    add_rule(cc[0], cc[1], c3);
  }
  printf("Read %llu atoms and %u rules\n", length, (unsigned)rules.size());

  for(unsigned j = 0; j < 40; ++j) {
    step();
    unsigned imin=0, imax = 0;
    for(unsigned i = 1; i < histogram.size(); ++i) {
      if (histogram[i] < histogram[imin]) imin = i;
      if (histogram[i] > histogram[imax]) imax = i;
    }
    printf("After %2u steps: length: %llu; least common %c:%llu; most common %c:%llu; diff %llu\n",
	   j+1, length,
	   names[imin], histogram[imin],
	   names[imax], histogram[imax],
	   histogram[imax] - histogram[imin]);
  }
  
  return 0;	 
}
