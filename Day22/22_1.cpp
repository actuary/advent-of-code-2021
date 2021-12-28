#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <assert.h>
#include <stack>
#include <set>

struct Range {
  int min, max;
};

struct RebootStep {
  Range x, y, z;
  bool on = false;

  friend std::ostream& operator<<(std::ostream& os, RebootStep step) {
    os << (step.on ? "on " : "off ") 
       << "x=" << step.x.min << ".." << step.x.max << ","
       << "y=" << step.y.min << ".." << step.y.max << ","
       << "z=" << step.z.min << ".." << step.z.max;

    return os;
  }
};

std::vector<RebootStep> getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);

  std::vector<RebootStep> result;
  
  std::string line;
  std::string word;
  while (std::getline(in, line)) {
    RebootStep rs;
    rs.on = line[1] == 'n';
    if (rs.on)
      line = line.substr(3, line.length() - 3);
    else
      line = line.substr(4, line.length() - 3);
   
    std::istringstream iss(line);
    
    iss.ignore(2);
    iss >> rs.x.min;
    iss.ignore(2);
    iss >> rs.x.max;
    iss.ignore(3);
    iss >> rs.y.min;
    iss.ignore(2);
    iss >> rs.y.max;
    iss.ignore(3);
    iss >> rs.z.min;
    iss.ignore(2);
    iss >> rs.z.max;

    result.push_back(rs);
  }

  in.close();
  return result;
}

void printData(std::vector<RebootStep> const& steps) {
  for (auto const& step : steps) {
    std::cout << step << "\n";
  }
}

bool inInitArea(int x, int y, int z) {
  return x >= -50 && x <= 50 && y >= -50 && y <= 50 && z >= -50 && z <= 50;
}

bool isValidCoord(int min, int max) {
  return (min >= -50 && min <= 50) || (max >= -50 && max <= 50);
}
bool isValidStep(RebootStep step) {
  return isValidCoord(step.x.min, step.x.max) 
      && isValidCoord(step.y.min, step.y.max)
      && isValidCoord(step.z.min, step.z.max);
}


void solve(std::vector<RebootStep> steps) {
  std::array<std::array<std::array<bool, 101>, 101>, 101> init_proc_area = {false};

  for (auto const& step : steps) {
    if (isValidStep(step)) {
      for (int x = std::max(step.x.min, -50); x <= std::min(step.x.max, 50); x++) {
        for (int y = std::max(step.y.min, -50); y <= std::min(step.y.max, 50); y++) {
          for (int z = std::max(step.z.min, -50); z <= std::min(step.z.max, 50); z++) {
            if (inInitArea(x, y, z)) {
              init_proc_area[x + 50][y + 50][z + 50] = step.on;
            }
          }
        }
      }
    }
  }

  int count = 0;
  for (int x = -50; x <= 50; x++) {
    for (int y = -50; y <= 50; y++) {
      for (int z = -50; z <= 50; z++) {
        count += (init_proc_area[x + 50][y + 50][z + 50] ? 1 : 0);
      }
    }
  }

  std::cout << count << "\n";
}

void test() {
  solve(getData("input.test"));
}

void solution() {
  solve(getData());
}

int main() {
  test();
  //solution();
  return 0;
}
