#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>
#include <algorithm>
#include <queue>
#include <map>

using NumTable = std::vector<std::vector<long>>;
NumTable getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);

  if (!in)
    return {};

  NumTable result;

  std::string line;
  while (std::getline(in, line)) {
    std::vector<long> line_digits = {};
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
    std::cout << "\n";
  }

  std::cout << "\n";
}


NumTable duplicateNumTable(NumTable const& data, int times = 5) {
  if (times == 0)
    return data;

  NumTable result;

  int num_rows = data.size();
  int num_cols = data[0].size();
  for (int i = 0; i < times; i++) {
    for (int row = 0; row < num_rows; row++) {
      result.push_back({});
      for (int j = 0; j < times; j++) {
        std::vector<long> new_set = data[row];
        for (int col = 0; col < num_cols; col++) {
          new_set[col] = (data[row][col] - 1 + i + j) % 9 + 1;
        }
        result[row + i*num_rows].insert(result[row + i*num_rows].end(), new_set.begin(), new_set.end());
      }
    }
  }

  return result;
}


//thanks nathan for the hint on djikstra
void dijikstra_pq(NumTable const& data) {
  int const line_len = data[0].size();
  int const num_line = data.size();
  int const target = line_len * num_line - 1;

  using Point = std::pair<int, int>;
  std::vector<Point> neighbours = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  std::vector<int> prev(line_len * num_line, -1);
  std::vector<int> dist(line_len * num_line, INT_MAX);
  std::priority_queue<std::pair<int, int>> Q;

  Q.push({0, 0});
  dist[0] = 0;
  //https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
  while (!Q.empty()) {
    int u = Q.top().second;
    Q.pop();

    if (u == target) 
      break;

    for (auto const& neighbour : neighbours) {
      int row = u % line_len + neighbour.first;
      int col = u / line_len + neighbour.second;

      if (row < 0 || col < 0 || row == line_len || col == num_line) 
        continue;

      int v = row + col * line_len;
      int alt = dist[u] + data[row][col];

      if (alt < dist[v]) {
        dist[v] = alt;
        prev[v] = u;
        Q.push({-alt, v}); //- as minimum oh my god that took me a while
      }
    }
  }

  std::vector<int> shortest_path;
  int u = target;
  if (prev[u] != -1 || u == 0) {
    while (u != -1) {
      shortest_path.insert(shortest_path.begin(), u);
      u = prev[u];
    }
  }


  std::cout << dist[target] << "\n";
}

void solve(NumTable& data) {
  
  dijikstra_pq(data);
  NumTable bigger = duplicateNumTable(data, 5);
  dijikstra_pq(bigger);

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
