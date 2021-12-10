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
std::array<int, 4> const kCompletionPoints = {1, 2, 3, 4};

bool isIncomplete(std::string const& line) {
  return true;
}

long incompleteValue(std::string const& line, int pos, std::stack<char>& open, std::stack<char>& close) {
  if (pos == line.length()) {
    long total = 0;
    while (!open.empty()) {
      int open_idx = std::distance(kOpenBrackets.begin(), std::find(kOpenBrackets.begin(), kOpenBrackets.end(), open.top()));
      total = total * 5 + kCompletionPoints[open_idx];
      open.pop();
      close.pop();
    }
    return total;
  }

  int open_idx = std::distance(kOpenBrackets.begin(), std::find(kOpenBrackets.begin(), kOpenBrackets.end(), line[pos]));
  int clos_idx = std::distance(kClosBrackets.begin(), std::find(kClosBrackets.begin(), kClosBrackets.end(), line[pos]));

  if (open_idx < 4) {
    open.push(line[pos]);
    close.push(kClosBrackets[open_idx]);
    return incompleteValue(line, pos + 1, open, close);
  }

  char expected_closing = close.top();
  if (expected_closing != kClosBrackets[clos_idx]) {
    return 0; //corrupt
  }
 
  open.pop();
  close.pop();
  return incompleteValue(line, pos + 1, open, close);
}

void computeSyntaxErrorScore(std::vector<std::string> data) {
  std::vector<long> results;
  for (auto const& line: data) {
    std::stack<char> open = {};
    std::stack<char> close = {};

    long line_score = incompleteValue(line, 0, open, close);
    if (line_score > 0 ) {
      results.push_back(line_score);
    }
  }

  std::sort(results.begin(), results.end());

  long score = results[results.size()/2];
  std::cout << "Total syntax error score of " << data.size() << " resulting in " << results.size() << " incomplete pages" << " is: " << score << "\n";
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
  in.close();

  computeSyntaxErrorScore(data);
}
int main() {
  test();
  solution();

  return 0;
}
