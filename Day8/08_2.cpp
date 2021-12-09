#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include <assert.h>

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

void printData(std::vector<LineOWords> const& data) {
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
int getOutputValue(LineOWords const& line) {
  std::array<std::set<char>, 10> decoded; 

  //first get uniques
  for (int i = 0; i < OUTPUT_START; i++) {
    std::set<char> elems(std::begin(line[i]), std::end(line[i]));

    //decode unique elements
    if (elems.size() == 2) {
      decoded[1] = elems;
    } else if (elems.size() == 4) {
      decoded[4] = elems;
    } else if (elems.size() == 3) {
      decoded[7] = elems;
    } else if (elems.size() == 7) {
      decoded[8] = elems;
    }
  }

  //then get rest
  for (int i = 0; i < OUTPUT_START; i++) {
    std::set<char> elems(std::begin(line[i]), std::end(line[i]));

    bool matched = false;
    std::set<int> intersect; //my god these are verbose
    if (elems.size() == 6) {

      //6 can be identified as only 6-len that doesn't contain 1 completely
      intersect.clear();
      std::set_intersection(elems.begin(), elems.end(), decoded[1].begin(), decoded[1].end(), std::inserter(intersect, intersect.end()));
      if (intersect.size() != 2) {
        decoded[6] = elems;
        matched = true;
      }

      intersect.clear();
      //9 can be identified as only 6-len that contains 4
      std::set_intersection(elems.begin(), elems.end(), decoded[4].begin(), decoded[4].end(), std::inserter(intersect, intersect.end()));
      if (intersect.size() == 4) {
        decoded[9] = elems;
        matched = true;
      }

      //otherwise 0
      if (!matched) {
        decoded[0] = elems;
        matched = true;
      }

    } else if (elems.size() == 5) {

      //3 can be identified as only 5-len that does contain 1 completely
      intersect.clear();
      std::set_intersection(elems.begin(), elems.end(), decoded[1].begin(), decoded[1].end(), std::inserter(intersect, intersect.end()));
      if (intersect.size() == 2) {
        decoded[3] = elems;
        matched = true;
      }

      //5 can be identified as containing more of 4
      intersect.clear();
      std::set_intersection(elems.begin(), elems.end(), decoded[4].begin(), decoded[4].end(), std::inserter(intersect, intersect.end()));
      if (!matched && intersect.size() == 3) {
        decoded[5] = elems;
        matched = true;
      }

      //otherwise 2
      if (!matched) {
        decoded[2] = elems;
        matched = true;
      }
    }

  }

  int result = 0;
  int fac = 1000;
  for (int i = OUTPUT_START; i < WORDS_IN_LINE; i++) {
     
    auto it = std::find_if(decoded.begin(), decoded.end(), [&line,&i] (std::set<char> elems) {
        return std::set<char>(std::begin(line[i]), std::end(line[i])) == elems;
    });
    result += (it - decoded.begin()) * fac;
    fac /= 10;
  }

  return result;
}

int main() {
  std::ifstream in("input.data");
   
  auto data = readData(in);


  int result = 0;
  for (auto const& line : data) {
    result += getOutputValue(line); 
  }

  std::cout << result << "\n";

  in.close();

  return 0;
}
