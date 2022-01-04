#include <cstdio>
#include <set>
#include <algorithm>
#include <cassert>

using namespace std;

struct Range {
  int x0, x1; // x1 is exclusive upper bound
  Range(int p=0, int q=0) : x0(p), x1(q) { }
  int length() const { return (x1>x0) ? (x1-x0) : 0; }
  bool operator==(Range const &r) const { return x0==r.x0 && x1==r.x1; }
  bool operator<(Range const &r) const { return x0<r.x0 || (x0==r.x0 && x1<r.x1); }
  bool intersects(Range const &r) const {
    return x1 > r.x0 && r.x1 > x0;
  }
  Range intersection(Range const &r) const {
    return Range(max(x0,r.x0), min(x1,r.x1));
  }
  Range before(Range const &r) const {
    if (r.x0 > x0) return Range(x0, min(r.x0,x1));
    else return Range(0,0);
  }
  Range after(Range const &r) const {
    if (r.x1 < x1) return Range(max(r.x1,x0),x1);
    else return Range(0,0);
  }
  Range zone(Range const &r, int z) const {
    return (z==2) ? after(r) : (z==1) ? intersection(r) : before(r);
  };
};

struct Cuboid {
  Range xyz[3];
  Cuboid() {}
  Cuboid(int x0, int x1, int y0, int y1, int z0, int z1)
    : xyz{Range(x0,x1),Range(y0,y1),Range(z0,z1)} {}
  Cuboid(Range xx, Range yy, Range zz) : xyz{xx,yy,zz} {}
  long long volume() const {
    return ((long long)(xyz[0].length())) * ((long long)(xyz[1].length())) * ((long long)(xyz[2].length()));
  }
  bool operator==(Cuboid const &r) const {
    return xyz[0]==r.xyz[0] && xyz[1]==r.xyz[1] && xyz[2]==r.xyz[2];
  }
  bool operator<(Cuboid const &r) const {
    if (!(xyz[0]==r.xyz[0])) return xyz[0]<r.xyz[0];
    if (!(xyz[1]==r.xyz[1])) return xyz[1]<r.xyz[1];
    return xyz[2]<r.xyz[2];
  };
  bool intersects(Cuboid const & r) const {
    return xyz[0].intersects(r.xyz[0]) && xyz[1].intersects(r.xyz[1]) && xyz[2].intersects(r.xyz[2]);
  }
  Cuboid intersection(Cuboid const & r) const {
    return Cuboid(xyz[0].intersection(r.xyz[0]),
		  xyz[1].intersection(r.xyz[1]),
		  xyz[2].intersection(r.xyz[2]));
  };
  set<Cuboid> diffs(Cuboid const & r) const {
    set<Cuboid> s;
    if (intersects(r)) {
      for(int xzone = 0; xzone < 3; ++xzone) {
	Range xx = xyz[0].zone(r.xyz[0], xzone);
	if (xx.length() <= 0) continue;
	for(int yzone = 0; yzone < 3; ++yzone) {
	  Range yy = xyz[1].zone(r.xyz[1], yzone);
	  if (yy.length() <= 0) continue;
	  for(int zzone = 0; zzone < 3; ++zzone) {
            Range zz = xyz[2].zone(r.xyz[2], zzone);
	    if (zz.length() <= 0) continue;
	    if (xzone != 1 || yzone != 1 || zzone != 1) {
	      s.insert(Cuboid(xx,yy,zz));
	    }
	  }
	}
      }
    }
    return s;
  }
};

////////////////

set<Cuboid> cuboids;
long long total_volume = 0;

void add_cuboid(Cuboid const & c)
{
  for(auto k : cuboids) {
    if (c.intersects(k)) {
      set<Cuboid> dd = c.diffs(k);
      for(auto d : dd) {
	add_cuboid(d);	
      }
      return;
    }
  }
  cuboids.insert(c);
  total_volume += c.volume();
}

void sub_cuboid(Cuboid const & c)
{
 again:
  for(auto k : cuboids) {
    if (c.intersects(k)) {
      set<Cuboid> dd = k.diffs(c);
      total_volume -= k.volume();
      cuboids.erase(k);
      for(auto d : dd) {
	add_cuboid(d);
      }
      goto again;
    }
  }
}

void task(int init)
{
  int x0, x1, y0, y1, z0, z1;
  char verb[4];
  Cuboid bounds(-50,51,-50,51,-50,51);

  while (scanf("%3s x=%d..%d,y=%d..%d,z=%d..%d\n", verb, &x0, &x1, &y0, &y1, &z0, &z1) == 7) {
    assert(verb[0]=='o');
    Cuboid c(x0,x1+1,y0,y1+1,z0,z1+1);
    if (init == 1) {
      if (!c.intersects(bounds)) continue;
      c = c.intersection(bounds);
    }
    if (verb[1]=='n') add_cuboid(c);
    else sub_cuboid(c);
  }
  printf("Total volume %lld\n", total_volume);
}

int main()
{
  //task(1);
  task(2);
  return 0;
}

