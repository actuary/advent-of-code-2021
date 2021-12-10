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
std::map<char, int> const kCompletionPoints {{')', 3},{']', 57},{'}', 1197}, {'>', 25137}};

long corruptValue(std::string const& line, int pos, std::stack<char>& close) {
  if (pos == line.length()) {
    return 0;
  }

  if (kBrackets.count(line[pos])) {
    close.push(kBrackets.at(line[pos]));
    return corruptValue(line, pos + 1, close);
  }

  char expected_closing = close.top();
  if (expected_closing != line[pos]) {
    return kCompletionPoints.at(line[pos]); //corrupt
  }
 
  close.pop();
  return corruptValue(line, pos + 1, close);
}

void computeSyntaxErrorScore(std::vector<std::string> data) {
  long total = 0;
  for (auto const& line: data) {
    std::stack<char> close = {};

    long line_score = corruptValue(line, 0, close);
    if (line_score > 0 ) {
      total += line_score;
    }
  }

  std::cout << "Total syntax error score of " << total << "\n"; 
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
