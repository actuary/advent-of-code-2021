#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <algorithm>
#include <stack>

std::vector<std::string> getData(std::ifstream& in) {
  std::string line;
  std::vector<std::string> result;

  while (std::getline(in, line)) {
    result.push_back(line);
  }

  return result;
}

std::array<char, 4> const kOpenBrackets = {'(', '[', '{', '<'};
std::array<char, 4> const kClosBrackets = {')', ']', '}', '>'};
std::array<int, 4> const kPoints = {3, 57, 1197, 25137};

bool isIncomplete(std::string const& line) {
  return true;
}

int corruptValue(std::string const& line, int pos, std::stack<char>& open, std::stack<char>& close) {
  if (pos == line.length())
    return 0;

  int open_idx = std::distance(kOpenBrackets.begin(), std::find(kOpenBrackets.begin(), kOpenBrackets.end(), line[pos]));
  int clos_idx = std::distance(kClosBrackets.begin(), std::find(kClosBrackets.begin(), kClosBrackets.end(), line[pos]));

  if (open_idx < 4) {
    open.push(line[pos]);
    close.push(kClosBrackets[open_idx]);
    return corruptValue(line, pos + 1, open, close);
  }

  char expected_closing = close.top();

  if (expected_closing != kClosBrackets[clos_idx]) {
    return kPoints[clos_idx];
  }
 
  open.pop();
  close.pop();
  return corruptValue(line, pos + 1, open, close);
}

void computeSyntaxErrorScore(std::vector<std::string> data) {
  long total = 0;
  for (auto const& line: data) {
    std::stack<char> open = {};
    std::stack<char> close = {};
    total += corruptValue(line, 0, open, close);
  }
 
  std::cout << "Total syntax error score is: " << total << "\n";
}

void test() {
  std::vector<std::string> data = {"[({(<(())[]>[[{[]{<()<>>",
                                   "[(()[<>])]({[<{<<[]>>(",
                                   "{([(<{}[<>[]}>{[]{[(<()>",
                                   "(((({<>}<{<{<>}{[]{[]{}",
                                   "[[<[([]))<([[{}[[()]]]",
                                   "[{[{({}]{}}([{[{{{}}([]",
                                   "{<[[]]>}<{[{[{[]{()[[[]",
                                   "[<(<(<(<{}))><([]([]()",
                                   "<{([([[(<>()){}]>(<<{{",
                                   "<{([{{}}[<[[[<>{}]]]>[]]"};


  computeSyntaxErrorScore(data);

}

void solution() {
  std::ifstream in("input.data");
  auto data = getData(in);
  computeSyntaxErrorScore(data);
  in.close();
}
int main() {
  test();
  solution();

  return 0;
}
