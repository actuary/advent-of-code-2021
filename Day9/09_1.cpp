#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> getData(std::ifstream& in) {
  std::string line;
  std::vector<std::string> result;
  while (std::getline(in, line)) {
    result.push_back(line);
  }

  return result;
}

void printData(std::vector<std::string> const& data) {
  for (auto& line : data) {
    std::cout << line << "\n"; 
  }
}

bool isLowPoint(std::vector<std::string> const& data, int row, int col, int max_rows, int max_cols) {
  bool result = true; 

  if (row < max_rows)
    result = result && (data[row][col] < data[row + 1][col]);

  if (col < max_cols)
    result = result && (data[row][col] < data[row][col + 1]);

  if (row > 0)
    result = result && (data[row][col] < data[row - 1][col]);

  if (col > 0)
    result = result && (data[row][col] < data[row][col - 1]);

  return result;
}

int test() {
  std::vector<std::string> data = {"2199943210",
                                   "3987894921", 
                                   "9856789892", 
                                   "8767896789", 
                                   "9899965678"};
  size_t line_len = data[0].length();
  size_t line_num = data.size();

  int total_risk_levels = 0;
  for(int col = 0; col < line_len; col++) {
    for(int row = 0; row < line_num; row++) {
      if (isLowPoint(data, row, col, line_num - 1, line_len - 1)) {
        total_risk_levels += data[row][col] - '0' + 1;
      }
    }
  }

  std::cout << total_risk_levels << "\n";

  return 0;
}

int main() {
  std::ifstream in("input.data");
 
  test();
  auto data = getData(in);
  size_t line_len = data[0].length();
  size_t line_num = data.size();
 
  int total_risk_levels = 0;
  for(int col = 0; col < line_len; col++) {
    for(int row = 0; row < line_num; row++) {
      if (isLowPoint(data, row, col, line_num - 1, line_len - 1))
        total_risk_levels += data[row][col] - '0' + 1;
    }
  }
  
  std::cout << total_risk_levels << "\n";
  
  in.close();
  return 0;
}
