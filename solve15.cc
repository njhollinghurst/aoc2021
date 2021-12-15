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

int main()
{
  rd();
  task1();
  frob_for_task2();
  task1();
  return 0;
}
