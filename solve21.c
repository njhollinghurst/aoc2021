#include <stdio.h>
#include <assert.h>
#include <string.h>

#define NPLAYERS 2
int start_pos[NPLAYERS];

void rd()
{
  for(int i = 0; i < NPLAYERS; ++i) {
    assert(scanf("Player %*d starting position: %d\n", &start_pos[i]) >= 1);
  }
}

void task1()
{
  int pos[2];
  int score[2];
  int dstate = 1;
  int rolls = 0;
  int i;

  for(i = 0; i < NPLAYERS; ++i) {
    pos[i] = start_pos[i];
    score[i] = 0;
  }

  for(i = 0; ; i = !i) {
    pos[i] += dstate;
    dstate = (dstate % 100) + 1;
    pos[i] += dstate;
    dstate = (dstate % 100) + 1;
    pos[i] += dstate;
    dstate = (dstate % 100) + 1;
    rolls += 3;

    pos[i] = (pos[i]-1)%10 + 1;
    score[i] += pos[i];
    if (score[i] >= 1000) {
      printf("Player %d wins with %d after %d rolls: %d\n",
	     i+1, score[i], rolls, rolls*score[1-i]);
      return;
    }
  }
}


void foo()
{
  static int hist[16];
  for(int d1 = 1; d1 <= 3; ++d1) {
    for(int d2 = 1; d2 <= 3; ++d2) {
      for(int d3 = 1; d3 <= 3; ++d3) {
	hist[d1+d2+d3]++;
      }
    }
  }
  for(int i = 0; i < 16; ++i) printf("%d,", hist[i]);
}


void task2()
{
  static unsigned long long state[2][21*21*10*10];
  unsigned long long wins[2] = { 0, 0 };
  unsigned long long universes = 1;

  memset(state, 0, sizeof(state));
  state[0][10*(start_pos[1]-1) + (start_pos[0]-1)] = 1;
  universes = 1;

  while (universes) {
    for(int p = 0; p < 2; ++p) {
      for(int i = 0; i < 21*21*10*10; ++i) {
	unsigned long long n = state[p][i];
	if (n==0) continue;
	int pos = p ? ((i/10)%10)   : (i%10);
	int scr = p ? ((i/2100)%21) : ((i/100)%21);
	int pos_other = p ? (i%10)       : ((i/10)%10);
	int scr_other = p ? ((i/100)%21) : ((i/2100)%21);
	universes -= n;
	state[p][i] = 0;
	static const unsigned long long HIST[10] = { 0,0,0,1,3,6,7,6,3,1 };
	for(int j = 3; j < 10; ++j) {
	  int pnext = (pos + j) % 10;
	  int snext = scr + pnext + 1;
	  if (snext >= 21) {
	    wins[p] += HIST[j]*n;
	  }
	  else {
	    int inext = p ?
	      (2100*snext + 100*scr_other + 10*pnext + pos_other) :
	      (2100*scr_other + 100*snext + 10*pos_other + pnext);
	    state[1-p][inext] += HIST[j]*n;
	    universes += HIST[j]*n;
	  }
	}
      }
    }
  }
 
  printf("%llu %llu max=%llu\n", wins[0], wins[1],
	 (wins[0]>wins[1]) ? wins[0] : wins[1]);
}


int main()
{
  rd();
  task1();
  //foo();
  task2();
  return 0;
}
