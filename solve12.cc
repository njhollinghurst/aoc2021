#include <cctype>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <set>

std::vector<std::string> caves;
std::vector<std::set<size_t> > connections;

size_t find_cave(std::string name)
{
  for(size_t i = 0; i < caves.size(); ++i) {
    if (caves[i]==name) return i;
  }
  return caves.size();
}

size_t add_cave(std::string name)
{
  size_t i = find_cave(name);
  if (i < caves.size()) {
    return i;
  }
  else {
    caves.push_back(name);
    connections.emplace_back();
    return caves.size()-1;
  }
}

void rd()
{
  std::string s;
  for(;;) {
    std::cin >> s;
    if (std::cin.eof()) break;
    size_t dash = s.find_first_of('-');
    size_t i = add_cave(s.substr(0, dash));
    size_t j = add_cave(s.substr(dash+1, s.npos));
    connections[i].insert(j);
    connections[j].insert(i);
    std::cout << caves[i] << '-' << caves[j] << std::endl;
  }
}

unsigned npath(size_t start, size_t finish, std::set<size_t> &visited, bool task2_allowed)
{
  unsigned n = 0;
  bool task2_revisit = visited.find(start) != visited.end();

  // For silly premature-optimization reasons, visited is not stacked...
  if (std::islower(caves[start][0]) && !task2_revisit)
    visited.insert(start);

  for (std::set<size_t>::const_iterator ix = connections[start].begin();
       ix != connections[start].end(); ++ix) {
    size_t via = *ix;
    if (caves[via]=="start") continue;
    if (via == finish) {
      ++n;
      continue;
    }
    if (std::islower(caves[via][0]) && visited.find(via) != visited.end()) {
      if (!task2_allowed) continue;
      n += npath(via, finish, visited, false);
    }
    else {
      n += npath(via, finish, visited, task2_allowed);
    }
  }

  // ...so we must restore it on exit
  if (std::islower(caves[start][0]) && !task2_revisit)
    visited.erase(start);

  return n;
}


void task1()
{
  size_t start  = find_cave("start");
  size_t finish = find_cave("end");
  assert(start < caves.size() && finish < caves.size());
  std::set<size_t> visited;
  unsigned n = npath(start, finish, visited, false);
  std::cout << "Task 1: " << n << std::endl;
  assert(visited.empty());
  n = npath(start, finish, visited, true);
  std::cout << "Task 2: " << n << std::endl;
}

int main() {
  rd();
  task1();
  return 0;
}
