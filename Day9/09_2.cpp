#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <list>
#include <numeric>

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

struct Cave {
  std::vector<std::string> const& data;
  std::vector<std::vector<bool>>& visited;
  size_t const max_rows;
  size_t const max_cols;
};

struct Position {
  int row, col;
};

bool isLowPoint(Cave const& cave, Position const& pos) {
  bool result = true; 

  if (pos.row < cave.max_rows)
    result = result && (cave.data[pos.row][pos.col] < cave.data[pos.row + 1][pos.col]);

  if (pos.col < cave.max_cols)
    result = result && (cave.data[pos.row][pos.col] < cave.data[pos.row][pos.col + 1]);

  if (pos.row > 0)
    result = result && (cave.data[pos.row][pos.col] < cave.data[pos.row - 1][pos.col]);

  if (pos.col > 0)
    result = result && (cave.data[pos.row][pos.col] < cave.data[pos.row][pos.col - 1]);

  return result;
}

void getBasinSize(Cave& cave, Position const& pos, int& basin_size) {
  if (cave.visited[pos.row][pos.col] || cave.data[pos.row][pos.col] == '9')
    return;

  cave.visited[pos.row][pos.col] = true;
  basin_size++;

  if (pos.row < cave.max_rows && !cave.visited[pos.row+1][pos.col])
    getBasinSize(cave, {pos.row+1, pos.col}, basin_size);

  if (pos.col < cave.max_cols && !cave.visited[pos.row][pos.col+1])
    getBasinSize(cave, {pos.row, pos.col+1}, basin_size);

  if (pos.row > 0 && !cave.visited[pos.row-1][pos.col])
    getBasinSize(cave, {pos.row-1, pos.col}, basin_size);

  if (pos.col > 0 && !cave.visited[pos.row][pos.col-1])
    getBasinSize(cave, {pos.row, pos.col-1}, basin_size);
  
}
void addTop3BasinSize(int const basin_size, std::list<int>& top3) {

  for (auto it = top3.end(); it != top3.begin();) {
    it--;
    if (*it < basin_size) {
      it++;
      top3.insert(it, basin_size);
      it--;
      top3.erase(top3.begin());
      break;
    }
  }
}

int getProductOfTop3Basins(std::vector<std::string>& data) {
  size_t line_len = data[0].length();
  size_t line_num = data.size();
  std::vector<std::vector<bool>> visited;
  visited.resize(line_num);
  for (auto& line: visited) 
    line.resize(line_len);
  Cave cave = {data, visited, line_num - 1, line_len - 1}; 

  std::list<int> top3 = {0, 0, 0};

  int basin_size = 0;
  int total_risk_levels = 0;
  for(int col = 0; col < line_len; col++) {
    for(int row = 0; row < line_num; row++) {
      if (isLowPoint(cave, {row, col})) {
        int basin_size = 0;
        getBasinSize(cave, {row, col}, basin_size);
        addTop3BasinSize(basin_size, top3);
      }
    }
  }

  auto result = std::accumulate(begin(top3), end(top3), 1, std::multiplies<double>()); 

  return result;
}

int test() {
  std::vector<std::string> data = {"2199943210",
                                   "3987894921", 
                                   "9856789892", 
                                   "8767896789", 
                                   "9899965678"};
  
  std::cout << getProductOfTop3Basins(data) << "\n";

  return 0;
}

int main() {
  test();

  std::ifstream in("input.data");

  auto data = getData(in);
  in.close();
  std::cout << getProductOfTop3Basins(data) << "\n";

  return 0;
}
