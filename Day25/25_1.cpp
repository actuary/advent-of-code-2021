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

std::vector<std::string> getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  
  std::vector<std::string> result;
  std::string line;
  while (std::getline(in, line)) {
    result.push_back(line);
  }
  in.close();

  return result;
}

void printData(std::vector<std::string> const& state) {
  for (auto const& line : state) { 
    std::cout << line << "\n";
  }

  std::cout << "\n";
}

bool advanceEast(std::vector<std::string>& state, std::vector<std::string>& new_state) {
  bool advanced = false;
  int length = state.size();
  int width = state[0].length();
  for (int row = 0; row < length; row++) {
    for (int col = 0; col < width; col++) {
      char ch = state[row][col];
      if (ch == '>' && state[row][(col + 1) % width] == '.') {
        new_state[row][col] = '.';
        new_state[row][(col + 1) % width] = ch;
        advanced = true;
      }
    }
  }
  return advanced;
}

bool advanceSouth(std::vector<std::string>& state, std::vector<std::string>& new_state) {
  bool advanced = false;
  int length = state.size();
  int width = state[0].length();
  for (int row = 0; row < length; row++) {
    for (int col = 0; col < width; col++) {
      char ch = state[row][col];
      if (ch == 'v' && state[(row + 1) % length][col] == '.') {
        new_state[row][col] = '.';
        new_state[(row + 1) % length][col] = ch;
        advanced = true;
      }
    }
  }
  return advanced;
}

void solve(std::vector<std::string>& data) {
  bool advanced;
  int count = 1;
  std::vector<std::string> state = data;
  while (true) { 
    advanced = advanceEast(data, state);
    data = state;
    advanced = advanceSouth(data, state) || advanced;
    if (!advanced)
      break;

    count++;
    data = state;
  }

  std::cout << "Count: " << count << "\n";
}

void test() {
  std::vector<std::string> data = {
    "v...>>.vv>",
    ".vv>>.vv..",
    ">>.>v>...v",
    ">>v>>.>.v.",
    "v>v.vv.v..",
    ">.>>..v...",
    ".vv..>.>v.",
    "v.v..>>v.v",
    "....v..v.>"
  };

  solve(data);
}

void solution() {
  std::vector<std::string> data = getData();
  solve(data);
}

int main() {
  test();
  solution();
  return 0;
}
