#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using NumTable = std::vector<std::vector<int>>;
NumTable getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);

  if (!in)
    return {};

  NumTable result;

  std::string line;
  while (std::getline(in, line)) {
    std::vector<int> line_digits = {};
    for (auto const& digit: line) {
      line_digits.push_back(std::stoi(std::string() + digit));
    }
    result.push_back(line_digits);
  }

  in.close();
  return result;
}

void printData(NumTable const& data, int pad = 2) {
  for (auto const& row : data) {
    for (auto const& digit : row) {
      std::cout << std::setfill(' ') << std::setw(pad) << digit;
    }
    std::cout << "\n";
  }

  std::cout << "\n";
}


void solve(NumTable& data) {
  int line_len = data[0].size();
  int num_line = data.size();
  
  for (int row = 0; row < num_line; row++) {
    for (int col = 0; col < line_len; col++) {
      if (col == 0 && row > 0) {
        data[row][col] += data[row-1][col]; 
      } else if (row == 0 && col > 0) {
        data[row][col] += data[row][col-1]; 
      } else if (row > 0 && col > 0) {
        data[row][col] += std::min(data[row-1][col], data[row][col-1]);
      }

    }
  }
  std::cout << "Lowest total risk " << data[num_line-1][line_len-1] << "\n";
}

void test() {
  NumTable data = {
      {1,1,6,3,7,5,1,7,4,2},
      {1,3,8,1,3,7,3,6,7,2},
      {2,1,3,6,5,1,1,3,2,8},
      {3,6,9,4,9,3,1,5,6,9},
      {7,4,6,3,4,1,7,1,1,1},
      {1,3,1,9,1,2,8,1,3,7},
      {1,3,5,9,9,1,2,4,2,1},
      {3,1,2,5,4,2,1,6,3,9},
      {1,2,9,3,1,3,8,5,2,1},
      {2,3,1,1,9,4,4,5,8,1}
  };
  
  solve(data);
}

void solution() {
  NumTable data = getData();

  solve(data);
}

int main() {

  test();
  solution();
  return 0;
}
