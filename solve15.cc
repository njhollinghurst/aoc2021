#include <cassert>
#include <cctype>
#include <climits>
#include <vector>
#include <iostream>

using namespace std;

enum Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  UNKNOWN_DIR
};

struct Pos {
  unsigned  enter_risk;
  unsigned  exit_cost;
  Direction dir;
  bool visited; // for Dijkstra version, below
};

unsigned W = 0, H = 0;
vector<vector<Pos> > grid;

void rd()
{
  string s;
  for(;;) {
    cin >> s;
    if (cin.eof()) break;
    assert(W == 0 || W == s.size());
    W = s.size();

    grid.push_back(vector<Pos>(W));
    for(unsigned i = 0; i < W; ++i) {
      assert(isdigit(s[i]));
      grid[H][i].enter_risk = s[i] - '0';
      grid[H][i].exit_cost = 0x10000000;
      grid[H][i].dir = UNKNOWN_DIR;
      grid[H][i].visited = false;
    }
    H++;
  }
}

bool pass(Direction dir)
{
  bool any = false;
  unsigned y, x;

  switch (dir) {
  case LEFT:
    for(y = 0; y < H; ++y) {
      for(x = 1; x < W; ++x) {
	if (grid[y][x-1].enter_risk + grid[y][x-1].exit_cost < grid[y][x].exit_cost) {
	  grid[y][x].exit_cost = grid[y][x-1].enter_risk + grid[y][x-1].exit_cost;
	  grid[y][x].dir = LEFT;
	  any = true;
	}
      }
    }
    break;
  case RIGHT:
    for(y = 0; y < H; ++y) {
      for(x = W-1; x-- > 0;) {
	if (grid[y][x+1].enter_risk + grid[y][x+1].exit_cost < grid[y][x].exit_cost) {
	  grid[y][x].exit_cost = grid[y][x+1].enter_risk + grid[y][x+1].exit_cost;
	  grid[y][x].dir = RIGHT;
	  any = true;
	}
      }
    }
    break;
  case UP:
    for(y = 1; y < H; ++y) {
      for(x = 0; x < W; ++x) {
	if (grid[y-1][x].enter_risk + grid[y-1][x].exit_cost < grid[y][x].exit_cost) {
	  grid[y][x].exit_cost = grid[y-1][x].enter_risk + grid[y-1][x].exit_cost;
	  grid[y][x].dir = UP;
	  any = true;
	}
      }
    }
    break;
  case DOWN:
    for(y = H-1; y-- > 0;) {
      for(x = 0; x < W; ++x) {
	if (grid[y+1][x].enter_risk + grid[y+1][x].exit_cost < grid[y][x].exit_cost) {
	  grid[y][x].exit_cost = grid[y+1][x].enter_risk + grid[y+1][x].exit_cost;
	  grid[y][x].dir = DOWN;
	  any = true;
	}
      }
    }
    break;
  default:
    assert(0);
  }
  return any;
}

void task1()
{
  grid[H-1][W-1].exit_cost = 0;

  bool any;
  do {
    // It's unlikely but possible that we could have to go LEFT or UP.
    // XXX does that imply that iteration is required? Not sure...
    any = pass(RIGHT);
    any |= pass(DOWN);
    any |= pass(LEFT);
    any |= pass(UP);
  } while (any);

  cout << "Total path cost: " << grid[0][0].exit_cost << endl;
}

void frob_for_task2()
{
  for(unsigned y = 0; y < H; ++y) {
    grid[y].reserve(5*W);
    for(unsigned x = 0; x < W; ++x) {
      grid[y][x].exit_cost = 0x10000000;
      grid[y][x].dir = UNKNOWN_DIR;
      grid[y][x].visited = false;
    }
    for(unsigned x = W; x < 5*W; ++x) {
      grid[y].push_back(grid[y][x-W]);
      grid[y][x].enter_risk = (grid[y][x].enter_risk %9) + 1;
    }
  }
  W *= 5;
  for(unsigned y = H; y < 5*H; ++y) {
    grid.push_back(grid[y-H]);
    for(unsigned x = 0; x < W; ++x) {
      grid[y][x].enter_risk = (grid[y][x].enter_risk % 9) + 1;
    }
  }
  H *= 5;
}


//////////////////////////////////////////////////////////////////////
// The above is fine for both tasks, taking about 250ms for task 2;
// but the following may be faster for very large grids.
//
// Hmm. The STL has a "priority_queue" but it apparently has no
// facility for re-ordering existing elements of the queue(?)
//////////////////////////////////////////////////////////////////////

struct QEnt {
  unsigned short y;
  unsigned short x;
  QEnt(unsigned yy, unsigned xx) : y(yy), x(xx) { }
  unsigned cost() const { return grid[y][x].exit_cost; }
  bool operator==(QEnt const &r) const { return y==r.y && x==r.x; }
  bool operator!=(QEnt const &r) const { return y!=r.y || x!=r.x; }
};

vector<QEnt> the_q;  // keep this vector in REVERSE-SORTED ORDER

void q_check()
{
  for(unsigned i = 1; i < the_q.size(); ++i) {
    assert(the_q[i-1] != the_q[i]);
    assert(the_q[i-1].cost() >= the_q[i].cost());
  }
}

void q_reinsert(unsigned y, unsigned x, unsigned old_cost, unsigned new_cost)
{
  QEnt e(y,x);

  // find where it used to be
  unsigned old_pos = 0;
  unsigned hi = the_q.size();
  while (hi > old_pos) {
    unsigned p = (old_pos + hi) >> 1;
    if (the_q[p] == e) {
      old_pos = p;
      break;
    }
    if (the_q[p].cost() > old_cost)
      old_pos = p+1;
    else
      hi = p;
  }
  while (old_pos < the_q.size() && the_q[old_pos] != e && the_q[old_pos].cost() >= old_cost)
    ++old_pos;

  // find where it ought to go
  unsigned new_pos = old_pos;
  hi = the_q.size();
  while (hi > new_pos) {
    unsigned p = (new_pos + hi) >> 1;
    if (the_q[p] != e && the_q[p].cost() > new_cost)
      new_pos = p+1;
    else
      hi = p;
  }

  // if it was never found, just insert at new_pos
  if (old_pos >= the_q.size() || the_q[old_pos] != e) {
    the_q.insert(the_q.begin() + new_pos, e);
    return;
  }

  if (old_pos+1 < new_pos) {
    new_pos--;
    for(; old_pos < new_pos; ++old_pos) the_q[old_pos] = the_q[old_pos+1];
    the_q[new_pos] = e;
  }

  //q_check();
}


void dijkstra()
{
  grid[H-1][W-1].exit_cost = 0;

  the_q.clear();
  the_q.emplace_back(H-1,W-1);

  while (the_q.size() > 0) {

    // get the lowest-cost candidate node
    unsigned y = the_q.back().y;
    unsigned x = the_q.back().x;
    unsigned c = grid[y][x].exit_cost + grid[y][x].enter_risk;
    assert(!grid[y][x].visited);
    grid[y][x].visited = true;
    the_q.pop_back();

    // consider neighbours
    for(int dy = -1; dy <= 1; dy+=2) {
      if (y+(unsigned)dy < H) {
	if (!grid[y+dy][x].visited && c < grid[y+dy][x].exit_cost) {
	  unsigned c0 = grid[y+dy][x].exit_cost;
	  grid[y+dy][x].exit_cost = c;
	  grid[y+dy][x].dir = (dy < 0) ? DOWN : UP;
	  q_reinsert(y+dy, x, c0, c);
	}
      }
    }
    for(int dx = -1; dx <= 1; dx+=2) {
      if (x+(unsigned)dx < W) {
	if (!grid[y][x+dx].visited && c < grid[y][x+dx].exit_cost) {
	  unsigned c0 = grid[y][x+dx].exit_cost;
	  grid[y][x+dx].exit_cost = c;
	  grid[y][x+dx].dir = (dx < 0) ? RIGHT : LEFT;
	  q_reinsert(y, x+dx, c0, c);
	}
      }
    }
  }

  cout << "Total path cost: " << grid[0][0].exit_cost << endl;
}


int main()
{
  rd();
  task1();
  frob_for_task2();
#if 0
    task1();
#else
    dijkstra();
#endif
  return 0;
}
