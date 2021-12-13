#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

struct Coord {
  int x, y;
  bool operator<(Coord const &r) const { 
    return y < r.y || (y == r.y && x < r.x); 
  } 

  bool operator==(Coord const& r) const {
    return x == r.x && y == r.y;
  }
};

struct Fold {
  char axis;
  int line;
};


struct Instructions {
  std::vector<Coord> coords = {};
  std::vector<Fold> folds = {};
};

Instructions getData(std::ifstream& in) {
  Instructions results;
  std::string line, before_equals, after_equals;
  
  std::string coord_delim = ",";
  std::string fold_delim = "=";
  while (std::getline(in, line)) {
    if (line.length() == 0) {
      //skip
    } else if (line[0] != 'f') { 
      results.coords.push_back({std::stoi(line.substr(0, line.find(coord_delim))),
                                std::stoi(line.substr(line.find(coord_delim)+1,
                                          line.length() - line.find(coord_delim) + 1))});
    } else {
      before_equals = line.substr(0, line.find(fold_delim));
      after_equals = line.substr(line.find(fold_delim) + 1,
                                 line.length() - line.find(fold_delim) + 1);
      results.folds.push_back({before_equals[before_equals.length() - 1],
                               std::stoi(after_equals)});
    }
  }

  return results;
}

void printCoords(Instructions const& data) {
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 100; x++) {
      bool exists = std::find(data.coords.begin(),
                              data.coords.end(), Coord{x, y}) != data.coords.end();
      if (exists) 
        std::cout << "#";
      else 
        std::cout << ".";
    }
    std::cout << "\n";
  }
  
}

void foldData(std::vector<Coord>& coords, Fold const& fold) {
  //on an x fold at x = a, a point  (b, c) translates to 
  for(auto& coord : coords) {
    int tmp_x = coord.x;
    int tmp_y = coord.y;
    if (fold.axis == 'x' && coord.x > fold.line) 
      coord.x = 2 * fold.line - coord.x;
    else if (fold.axis == 'y' && coord.y > fold.line) 
      coord.y = 2 * fold.line - coord.y;
  }
 
  //removing dupes
  std::sort(coords.begin(), coords.end());
  std::set<Coord> s(coords.begin(), coords.end());
  coords.assign(s.begin(), s.end());
}

void solve(Instructions& data) {
  for (auto const& fold : data.folds) {
    foldData(data.coords, fold);
  }

  printCoords(data);
  std::cout << data.coords.size() << " folds visible.\n";
}

void test() {
  Instructions data;

  data.coords = { 
    {6,10},
    {0,14},
    {9,10},
    {0,3},
    {10,4},
    {4,11},
    {6,0},
    {6,12},
    {4,1},
    {0,13},
    {10,12},
    {3,4},
    {3,0},
    {8,4},
    {1,10},
    {2,14},
    {8,10},
    {9,0}
  };

  data.folds = {{'y', 7}, {'x', 5}};
  
  solve(data);
}

void solution() {
  std::ifstream in("input.data");
  Instructions data = getData(in);
  in.close();

  solve(data);
}

int main() {
  //test();
  solution();
  return 0;
}
