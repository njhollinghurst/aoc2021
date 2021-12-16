#include <cassert>
#include <cctype>
#include <climits>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Represent the grid
//////////////////////////////////////////////////////////////////////

// This enum and the "dir" field are not currently used.
// They could be used for path finding or visualization.
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
  bool visited;
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

// Make the grid 25 times larger and reset it
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
// Solve using Dijkstra's algorithm
// Now using std::set to implement a "priority queue", sorted by cost
//////////////////////////////////////////////////////////////////////

struct QEnt {
  unsigned x;
  unsigned y;
  unsigned cost;
  QEnt(unsigned xx, unsigned yy, unsigned cc) : x(xx), y(yy), cost(cc) { }
  bool operator<(QEnt const &r) const {
    return cost < r.cost || (cost == r.cost &&
			     (y < r.y || (y == r.y && x < r.x)));
  }
  bool operator==(QEnt const &r) const {
    return cost == r.cost && y == r.y && x == r.x;
  }
};

void dijkstra()
{
  set<QEnt> the_q;
  the_q.emplace(W-1, H-1, 0);
  grid[H-1][W-1].exit_cost = 0;

  while (!the_q.empty()) {

    // get the lowest-cost candidate node
    unsigned y = the_q.begin()->y;
    unsigned x = the_q.begin()->x;
    unsigned c = grid[y][x].exit_cost + grid[y][x].enter_risk;
    assert(!grid[y][x].visited);
    the_q.erase(the_q.begin());
    grid[y][x].visited = true;

    // consider neighbours
    for(int dy = -1; dy <= 1; dy+=2) {
      if (y+(unsigned)dy < H) {
	if (!grid[y+dy][x].visited && c < grid[y+dy][x].exit_cost) {
	  QEnt e(x, y+dy, grid[y+dy][x].exit_cost);
	  the_q.erase(e);
	  e.cost = c;
	  grid[y+dy][x].exit_cost = c;
	  grid[y+dy][x].dir = (dy < 0) ? DOWN : UP;
	  the_q.insert(e);
	}
      }
    }
    for(int dx = -1; dx <= 1; dx+=2) {
      if (x+(unsigned)dx < W) {
	if (!grid[y][x+dx].visited && c < grid[y][x+dx].exit_cost) {
	  QEnt e(x+dx, y, grid[y][x+dx].exit_cost);
	  the_q.erase(e);
	  e.cost = c;
	  grid[y][x+dx].exit_cost = c;
	  grid[y][x+dx].dir = (dx < 0) ? RIGHT : LEFT;
	  the_q.insert(e);
	}
      }
    }
  }

  cout << "Total path cost: " << grid[0][0].exit_cost << endl;
}


int main()
{
  rd();
  dijkstra();
  frob_for_task2();
  dijkstra();
#if 0
  frob_for_task2(); // task 3?
  dijkstra();
  frob_for_task2(); // task 4?
  dijkstra();
#endif

  return 0;
}
