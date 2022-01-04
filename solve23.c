#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define IA(c) ((c)>=64)     // isAmphipod, replaces isalpha()
#define AT(c) (((c)-1)&3)   // Amphipod type code

int can_move_room_hallway(char const * state, int i, int j)
{
  int j1 = 17+(i&3); // alcove is just to the right of this hall position
  if (j1 > j) {
    for(++j; j <= j1; ++j) if (IA(state[j])) return 0;
  }
  else if (j1 < j) {
    for(--j; j > j1; --j) if (IA(state[j])) return 0;
  }
  return 1;
}

int distance_room_hallway(int i, int j)
{
  assert(i >= 0 && i < 16 && j >= 16 && j < 23);
  static int xpos[7] = { 0, 1, 3, 5, 7, 9, 10 };
  int d = (i>>2)+1;
  i = 2 + 2*(i&3);
  j = xpos[j-16];
  if (i>j) d += i-j; else d += j-i;
  return d;
}

unsigned long long heur(char const * state, unsigned long long bc)
{
  int i;
  int c = 0;
  static const int MOVE_COSTS[4] = { 1, 10, 100, 1000 };
  for(i = 0; i < 16 && c < bc; ++i) {
    if (IA(state[i]) && AT(state[i]) != (i&3)) {
      int xd = AT(state[i]) - (i&3);
      if (xd < 0) xd = -xd;
      c += (2*xd+2+(i>>2))*MOVE_COSTS[AT(state[i])];
    }
  }
  for(i = 16; i < 23 && c < bc; ++i) {
    if (IA(state[i]))
      c += MOVE_COSTS[AT(state[i])] * distance_room_hallway(AT(state[i]), i);
  }
  return c;
}

unsigned long long best_cost(char * state, unsigned long long bc)
{
  static const int MOVE_COSTS[4] = { 1, 10, 100, 1000 };

  unsigned long long h = heur(state, bc);
  if (h == 0) return 0;

  for(int j = 16; j < 23 && bc >= h; ++j) {
    if (IA(state[j])) {
      int t = AT(state[j]);
      if (IA(state[t])) continue;
      if (IA(state[t+4])  && AT(state[t+4]) != t) continue;
      if (IA(state[t+8])  && AT(state[t+8]) != t) continue;
      if (IA(state[t+12]) && AT(state[t+12]) != t) continue;
      int i = t-4;
      while (i < 12 && !IA(state[i+4])) i += 4;
      if (i < 0) continue;
      unsigned long long c = MOVE_COSTS[t]*distance_room_hallway(i, j);
      if (c < bc && can_move_room_hallway(state, i, j)) {
        state[i]=state[j];
        state[j]='.';
        c += best_cost(state, bc-c);
        if (c < bc) bc = c;
        state[j]=state[i];
        state[i]='.';
      }
    }
  }
  if (bc <= h) return bc;
  for(int i = 0; i < 16; ++i) {
    if (IA(state[i])) {
      int t = AT(state[i]);
      if (i>=4  && IA(state[i-4])) continue;
      if (i>=8  && IA(state[i-8])) continue;
      if (i>=12 && IA(state[i-12])) continue;
      if (t == (i&3)) {
        if ((i >= 12 || (IA(state[i+4]) && AT(state[i+4])==t)) &&
            (i >= 8  || (IA(state[i+8]) && AT(state[i+8])==t)) &&
            (i >= 4  || (IA(state[i+12]) && AT(state[i+12])==t)))
          continue;
      }
      for(int j = 16; j < 23; ++j) {
        if (!IA(state[j]) && can_move_room_hallway(state, i, j)) {
          unsigned long long c = MOVE_COSTS[t]*distance_room_hallway(i, j);
          if (c < bc) {
            state[j]=state[i];
            state[i]='.';
            c += best_cost(state, bc-c);
            if (c < bc) bc = c;
            state[i]=state[j];
            state[j]='.';
          }
        }
      }
    }
  }
  return bc;
}



int main()
{
  char state[24];

  //strncpy(state, "BCBDADCAABCDABCD.......", 24); // example data (with extra rows for part 1)
  //strncpy(state, "BBCDDCAAABCDABCD.......", 24); // input data (with extra rows for part 1)
  //strncpy(state, "BCBDDCBADBACADCA.......", 24); // updated example
  strncpy(state, "BBCDDCBADBACDCAA.......", 24); // full input data for part 2

  unsigned long long bc = best_cost(state, 0xFFFFFFFFull);
  printf("%llu\n", bc);
  return 0;
}
