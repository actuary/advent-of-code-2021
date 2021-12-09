#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>

int const WORDS_IN_LINE = 14;
int const OUTPUT_START = 10;
using LineOWords = std::array<std::string, WORDS_IN_LINE>;
std::vector<LineOWords> readData(std::ifstream& in) {

  std::vector<LineOWords> result;
  std::string line, word;

  int idx;
  while (getline(in, line)) {
    std::istringstream iss(line);
    idx = 0;
    LineOWords low;
    while (getline(iss, word, ' ')) {
      if (word != "|") {
        low[idx++] = word;
      }
    }
    result.push_back(low);
  }

  return result;
}

void printData(std::vector<LineOWords> data) {
  for (auto const& line : data) {
    int idx = 0;
    for (auto const& word: line) {
      if (idx == OUTPUT_START) std::cout << "| ";
      std::cout << word << " ";
      idx++; 
    }
    std::cout << "\n";
  }
}

// 1,4,7,8 have a unique number of lines in digital format
int countUniqueOutputValues(std::vector<LineOWords> const& data) {

  int unique_values[4] = {2, 4, 3, 7}; //corresponds to 1, 4, 7, 8

  int found = 0;
  for (auto const& line : data) {
    for (int i = OUTPUT_START; i < WORDS_IN_LINE; i++) {
      if (std::find(std::begin(unique_values), std::end(unique_values), line[i].length()) != std::end(unique_values)) 
        found++;
    }
  }

  return found;
}



int main() {
  std::ifstream in("input.data");
   
  auto data = readData(in);

  std::cout << countUniqueOutputValues(data) << std::endl;
  in.close();

  return 0;
}
