#include <cstdio>
#include <cassert>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

struct Pos3D {
  int x;
  int y;
  int z;
  Pos3D() : x(0), y(0), z(0) { }
  Pos3D(int i, int j, int k) : x(i), y(j), z(k) { }
  void sortAbs() {
    int i=abs(x), j = abs(y), k=abs(z);
    x = (i<=j && i<=k) ? i : (j<=k) ? j : k;
    z = (i>=j && i>=k) ? i : (j>=k) ? j : k;
    y = i+j+k-x-z;
  }
  Pos3D &operator+=(Pos3D const & r) {
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
  }
  Pos3D &operator-=(Pos3D const & r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
  }
  void flip() {
    x = -x;
    y = -y;
    z = -z;
  }
  void rotate(int mx[3][3]) {
    int x1, y1;
    x1 = x*mx[0][0] + y*mx[0][1] + z*mx[0][2];
    y1 = x*mx[1][0] + y*mx[1][1] + z*mx[1][2];
    z  = x*mx[2][0] + y*mx[2][1] + z*mx[2][2];
    x = x1;
    y = y1;
  }
  bool operator<(Pos3D const & r) const {
    return x<r.x || (x==r.x && (y<r.y || (y==r.y && z<r.z)));
  }
  bool operator==(Pos3D const & r) const {
    return x==r.x && y==r.y && z==r.z;
  }
};

struct Scanner {
  unsigned index;
  set<Pos3D> sightings;
};

vector<Scanner> scanners;

int all_directions[24][3][3];

void init_directions()
{
  memset(all_directions, 0, sizeof(all_directions));
  unsigned n = 0;
  for(unsigned i0 = 0; i0 < 3; ++i0) {
    for(unsigned i1 = 0; i1 < 3; ++i1) {
      if (i1 == i0) continue;
      unsigned i2 = 3-i0-i1;
      for(unsigned pattern = 0; pattern < 4; ++pattern) {
	all_directions[n][0][i0] = (pattern&1) ? -1 : 1;
	all_directions[n][1][i1] = (pattern&2) ? -1 : 1;
	all_directions[n][2][i2] = ((pattern ^ (pattern>>1))&1) ? -1 : 1;
	if (i1 != ((i0+1)%3)) all_directions[n][2][i2] *= -1; // yuck
	n++;
      }
    }
  }
  assert(n==24);
}


void rd()
{
  char str[256];
  int i, j, k;
  Scanner * ptr = 0;

    while (fgets(str, 256, stdin)==str) {
      if (sscanf(str, "--- scanner %d", &i) > 0) {
	assert(i == (int)scanners.size());
	scanners.emplace_back();
	scanners.back().index = i;
	ptr = &scanners.back();
      }
      else if (sscanf(str, "%d,%d,%d", &i, &j, &k) > 0) {
	assert(ptr);
	ptr->sightings.insert(Pos3D(i, j, k));
      }
    }
}


// Find best match between two scanners; returning number of matches,
// rotation index and vector (to transform s2 sightings to s1 coords)

unsigned foo(Scanner const &s1, Scanner const &s2,
	     unsigned * rot_ix, Pos3D * vector)
{
  unsigned best_score = 0;

  for(auto m1 : s1.sightings) {
    for(auto m2 : s2.sightings) {
      // guess that m1 and m2 are the same beacon
      // make histograms of a "direction-invariant"
      // function of position, and score for overlap
      map<Pos3D,unsigned> h1, h2;
      for(auto p : s1.sightings) {
	Pos3D dist = p;
	dist -= m1;
	dist.sortAbs();
	if (h1.find(dist) != h1.end()) {
	  h1[dist]++;
	}
	else {
	  h1[dist]=1;
	}
      }
      for(auto p : s2.sightings) {
	Pos3D dist = p;
	dist -= m2;
	dist.sortAbs();
	if (h2.find(dist) != h2.end()) {
	  h2[dist]++;
	}
	else {
	  h2[dist]=1;
	}
      }
      unsigned score = 0;
      auto z1 = h1.begin();
      auto z2 = h2.begin();
      while (z1 != h1.end() && z2 != h2.end()) {
	if (z1->first == z2->first) {
	  score += min(z1->second, z2->second);
	  z1++;
	  z2++;
	}
	else if (z1->first < z2->first) {
	  z1++;
	}
	else {
	  z2++;
	}
      }
      if (score < 12 || score <= best_score) continue;

      // Now we need to find the relative orientation
      for(unsigned rix = 0; rix < 24; ++rix) {
	set<Pos3D> transformed;
	Pos3D v = m2;
	v.flip();
	v.rotate(all_directions[rix]);
	v += m1;

	for(auto p : s2.sightings) {
	  Pos3D px = p;
	  px.rotate(all_directions[rix]);
	  px += v;
	  transformed.insert(px);
	}
	unsigned score = 0;
	auto z1 = s1.sightings.begin();
	auto z2 = transformed.begin();
	while (z1 != s1.sightings.end() && z2 != transformed.end()) {
	  if (*z1 == *z2) {
	    ++score;
	    z1++;
	    z2++;
	  }
	  else if (*z1 < *z2) {
	    z1++;
	  }
	  else {
	    z2++;
	  }
	}
	if (score > best_score) {
	  best_score = score;
	  *rot_ix = rix;
	  *vector = v;
	}
      }
    }
  }

  printf("For scanners %u and %u: Matched %d beacons\n", s1.index, s2.index, best_score);
  return best_score;
}

vector<Pos3D> scanpos; // <-- for task 2

void task1()
{
  scanpos.emplace_back(0,0,0);

  while (scanners.size() > 1) {
    for(unsigned j = 1; j < scanners.size(); ++j) {
      Pos3D v;
      unsigned rix;
      unsigned s = foo(scanners[0], scanners[j], &rix, &v);
      if (s >= 12) {
	printf("Merge scanners 0 and %u\n", scanners[j].index);
	for(auto p : scanners[j].sightings) {
	  Pos3D px = p;
	  px.rotate(all_directions[rix]);
	  px += v;
	  scanners[0].sightings.insert(px);
	}
	scanpos.push_back(v); // <-- for task 2
	if (j+1 < scanners.size()) {
	  scanners[j] = move(scanners.back());
	  --j;
	}
	scanners.pop_back();
      }
    }
    printf("Scanner 0 now has %u beacons\n", scanners[0].sightings.size());
  }
}

void task2()
{
  int max_mhd = 0;
  for(unsigned i = 0; i < scanpos.size(); ++i) {
    for(unsigned j = i+1; j < scanpos.size(); ++j) {
      int d =
	abs(scanpos[j].x - scanpos[i].x) +
	abs(scanpos[j].y - scanpos[i].y) +
	abs(scanpos[j].z - scanpos[i].z);
      if (d > max_mhd) max_mhd = d;
    }
  }
  printf("Task 2: %d\n", max_mhd);
}


int main()
{
  rd();
  init_directions(); // generate all 24 possible rotations
  task1();
  task2();
  return 0;
}

