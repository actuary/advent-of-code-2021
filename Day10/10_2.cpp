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

std::map<char, char> const kBrackets = {{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
std::map<char, int> const kCompletionPoints {{')', 1},{']', 2},{'}', 3}, {'>', 4}};

long incompleteValue(std::string const& line, int pos, std::stack<char>& close) {
  if (pos == line.length()) {
    long total = 0;
    while (!close.empty()) {
      total = total * 5 + kCompletionPoints.at(close.top());
      close.pop();
    }
    return total;
  }

  if (kBrackets.count(line[pos])) {
    close.push(kBrackets.at(line[pos]));
    return incompleteValue(line, pos + 1, close);
  }

  char expected_closing = close.top();
  if (expected_closing != line[pos]) {
    return 0; //corrupt
  }
 
  close.pop();
  return incompleteValue(line, pos + 1, close);
}

void computeSyntaxErrorScore(std::vector<std::string> data) {
  std::vector<long> results;
  for (auto const& line: data) {
    std::stack<char> close = {};

    long line_score = incompleteValue(line, 0, close);
    if (line_score > 0 ) {
      results.push_back(line_score);
    }
  }

  std::sort(results.begin(), results.end());
  std::cout << "Total syntax error score of " << results[results.size()/2] << "\n"; 
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
