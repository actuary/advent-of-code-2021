#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <assert.h>
#include <stack>
#include <set>

void getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  in.close();
}

struct PlayerStart {
  int player1;
  int player2;
};

int roll(bool reset = false) {
  static int next_roll = 1;

  if (reset) {
    next_roll = 1;
    return 0;
  }

  int ret = next_roll*3 + 1 + 2;
  next_roll = (next_roll + 3 - 1) % 100 + 1;

  return ret;
}

void solve(PlayerStart ps) {
  std::pair<int, int> scores = {0, 0};
  std::pair<int, int> positions = {ps.player1, ps.player2};

  int no_times = 0;
  while (scores.first < 1000 && scores.second < 1000) {
    positions.first = (positions.first + roll() - 1) % 10 + 1;
    scores.first += positions.first; 
    no_times+=3;
    if (scores.first >= 1000)
      break;
    positions.second = (positions.second + roll() - 1) % 10 + 1;
    scores.second += positions.second; 
    no_times+=3;
  }

  std::cout << std::min(scores.first, scores.second) * no_times << "\n";
}

void test() {
  solve({4, 8});
}

void solution() {
  solve({8, 7});
}

int main() {
  test();
  roll(true);
  solution();
  return 0;
}
