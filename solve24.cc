#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <set>

struct Operation {
  unsigned char opcode;
  unsigned char dst;
  unsigned char src;
  long long immed;
};

unsigned ncode = 0;
unsigned ninput = 0;
Operation code[65536];
unsigned input_index[14];

void rd()
{
  char str[64];

  while(fgets(str, 64, stdin)==str) {
    assert(ncode < 65536);
    if (!strncmp(str, "inp", 3)) code[ncode].opcode = 0;
    else if (!strncmp(str, "add", 3)) code[ncode].opcode = 1;
    else if (!strncmp(str, "mul", 3)) code[ncode].opcode = 2;
    else if (!strncmp(str, "div", 3)) code[ncode].opcode = 3;
    else if (!strncmp(str, "mod", 3)) code[ncode].opcode = 4;
    else if (!strncmp(str, "eql", 3)) code[ncode].opcode = 5;
    else assert(0);
    code[ncode].dst = str[4]-'w';
    code[ncode].src = str[6]-'w';
    assert (code[ncode].dst < 4);
    if (code[ncode].src > 4 && code[ncode].opcode != 0) {
      code[ncode].src = 255;
      int i = sscanf(str+6, "%lld", &code[ncode].immed);
      assert(i==1);
    }
    if (code[ncode].opcode == 0) {
      assert(ninput < 14);
      input_index[ninput++] = ncode;
    }
    ncode++;
  }
}

struct State {
  long long vv[4];
  unsigned i;
  bool operator==(State const &r) const {
    return i==r.i && vv[0]==r.vv[0] && vv[1]==r.vv[1] && vv[2]==r.vv[2] && vv[3]==r.vv[3];
  }
  bool operator<(State const &r) const {
    if (i!=r.i) return i<r.i;
    if (vv[0]!=r.vv[0]) return vv[0]<r.vv[0];
    if (vv[1]!=r.vv[1]) return vv[1]<r.vv[1];
    if (vv[2]!=r.vv[2]) return vv[2]<r.vv[2];
    return vv[3]<r.vv[3];
  }
};

std::set<State> dead_states[15];
int tasknum = 1;

long long foo(long long const v0[4], unsigned i)
{
  long long var[4];
  var[0] = v0[0];
  var[1] = v0[1];
  var[2] = v0[2];
  var[3] = v0[3];
  State st{v0[0],v0[1],v0[2],v0[3], i};
  if (dead_states[i].find(st) != dead_states[i].end()) return -1;

  for(unsigned ip = (i ? input_index[i-1]+1 : 0); ip < ncode; ++ip) {
    long long s = (code[ip].src < 4) ? var[code[ip].src] : code[ip].immed;
    long long *d = var + code[ip].dst;
    switch(code[ip].opcode) {
    case 0:
      if (tasknum == 1) {
	for(s = 9; s >= 1; --s) {
	  *d = s;
	  long long t = foo(var, i+1);
	  if (t >= 0) return(s + 10ll*t);
	}
      }
      else {
	for(s = 1; s <= 9; ++s) {
	  *d = s;
	  long long t = foo(var, i+1);
	  if (t >= 0) return(s + 10ll*t);
	}
      }
      dead_states[i].insert(st);
      return -1;
      break;
    case 1:
      *d += s;
      break;
    case 2:
      *d *= s;
      break;
    case 3:
      *d /= s;
      break;
    case 4:
      *d %= s;
      break;
    case 5:
      *d = (*d == s) ? 1 : 0;
      break;
    }
  }
  return var[3] ? -1 : 0;
}

int main()
{
  rd();
  long long var[4] = {0, 0, 0, 0};
  long long t = foo(var, 0);
  printf("Task 1 digits (backwards!) %lld\n", t);
  tasknum = 2;
  t = foo(var, 0);
  printf("Task 2 digits (backwards!) %lld\n", t);
  return 0;
}
