#include <stdio.h>
#include <assert.h>

int target_xlo, target_xhi, target_ylo, target_yhi;

void rd()
{
  int n = scanf("target area: x=%d..%d, y=%d..%d",
		&target_xlo, &target_xhi,
		&target_ylo, &target_yhi);
  assert(n == 4);
}

void task1()
{
  int ubest = 0, vbest = 0, hbest = 0;
  int n = 0;

  int bound_xlo = (target_xlo < 0) ? target_xlo : 0;
  int bound_xhi = (target_xhi > 0) ? target_xhi : 0;
  int bound_ylo = (target_ylo < 0) ? target_ylo : 0;

  // guess an upper bound for initial vertical velocity,
  // noting that we always return to y=0, and steps are discrete
  int v0max = 65536;
  if (target_yhi < 0) {
    v0max = -target_ylo - 1;
  }
  else if (target_ylo > 0) {
    v0max = target_yhi;
  }
  else {
    printf("No vertical limit!\n");
  }

  for(int u0 = bound_xlo; u0 <= bound_xhi; u0++) {
    for(int v0 = bound_ylo; v0 <= v0max; v0++) {
      int u = u0;
      int v = v0;
      int x = 0;
      int y = 0;
      int h = 0;
      while (y >= bound_ylo && bound_xlo <= x && x <= bound_xhi &&
	     (u != 0 || (target_xlo <= x && x <= target_xhi))) {
	x += u;
	y += v;
	if (u > 0) u--; else if (u < 0) u++;
	v--;
	if (y > h) h = y;
	if (x >= target_xlo && x <= target_xhi && y >= target_ylo && y <= target_yhi) {
	  // Success!
	  ++n;
	  if (h > hbest) {
	    ubest = u0;
	    vbest = v0;
	    hbest = h;
	  }
	  break;
	}
      }
      if (y > target_yhi || (u==0 && 0 <= x && x < target_xlo)) {
	// if it went out of bounds with y too high,
	// or stalled and fell short, stop increasing v0
	break;
      }
    }
  }

  printf("Best velocity (%d,%d) reaches height %d\n", ubest, vbest, hbest);
  printf("Number: %d\n", n);
}

int main()
{
  rd();
  task1();
  return 0;
}
