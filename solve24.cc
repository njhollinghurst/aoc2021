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
    if (code[ncode].src >= 4 && code[ncode].opcode != 0) {
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
  bool operator==(State const &r) const {
    return vv[0]==r.vv[0] && vv[1]==r.vv[1] && vv[2]==r.vv[2] && vv[3]==r.vv[3];
  }
  bool operator<(State const &r) const {
    if (vv[0]!=r.vv[0]) return vv[0]<r.vv[0];
    if (vv[1]!=r.vv[1]) return vv[1]<r.vv[1];
    if (vv[2]!=r.vv[2]) return vv[2]<r.vv[2];
    return vv[3]<r.vv[3];
  }
};

std::set<State> dead_states[15];
int tasknum = 1;

long long foo(State const * st0, unsigned i)
{
  State st;
  st.vv[0] = st0 ? st0->vv[0] : 0;
  st.vv[1] = st0 ? st0->vv[1] : 0;
  st.vv[2] = st0 ? st0->vv[2] : 0;
  st.vv[3] = st0 ? st0->vv[3] : 0;

  assert(i < 15);
  for(unsigned ip = (i ? input_index[i-1]+1 : 0); ip < ncode; ++ip) {
    long long s = (code[ip].src < 4) ? st.vv[code[ip].src] : code[ip].immed;
    long long *d = &st.vv[code[ip].dst];
    switch(code[ip].opcode) {
    case 0:
      st.vv[code[ip].dst] = 0;
      if (dead_states[i].find(st) == dead_states[i].end()) {
	if (tasknum == 1) {
	  for(s = 9; s >= 1; --s) {
	    st.vv[code[ip].dst] = s;
	    long long t = foo(&st, i+1);
	    if (t >= 0) return(s + 10*t);
	  }
	}
	else {
	  for(s = 1; s <= 9; ++s) {
	    st.vv[code[ip].dst] = s;
	    long long t = foo(&st, i+1);
	    if (t >= 0) return(s + 10*t);
	  }
	}
	st.vv[code[ip].dst] = 0;
	dead_states[i].insert(st);
      }
      return -1;
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
  return st.vv[3] ? -1 : 0;
}

long long rdigits(long long a)
{
  long long r = 0;
  while (a > 0) {
    r = r*10 + (a%10);
    a /= 10;
  }
  return r;
}

int main()
{
  rd();
  long long t = foo(0, 0);
  printf("Task 1: %lld\n", rdigits(t));
  tasknum = 2;
  t = foo(0, 0);
  printf("Task 2: %lld\n", rdigits(t));
  return 0;
}
