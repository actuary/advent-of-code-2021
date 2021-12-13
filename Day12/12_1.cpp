#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

std::vector<std::string> getData(std::ifstream& in) {
  std::vector<std::string> result;
  std::string line;
  while (std::getline(in, line)) {
    result.push_back(line);
  }

  return result;
}


struct CaveInfo {
  std::map<std::string, std::set<std::string>> paths;
  std::map<std::string, bool> small_caves;
  std::map<std::string, int> large_caves;
  int num_routes = 0;
};

bool isSmallCave(std::string const& cave_name) {
  return (std::islower(cave_name[0]));
}

bool isLargeCave(std::string const& cave_name) {
  return (std::isupper(cave_name[0]));
}

CaveInfo getMap(std::vector<std::string> data) {
  std::map<std::string, std::set<std::string>> paths;
  std::map<std::string, bool> small_caves;
  std::map<std::string, int> large_caves;

  std::string left;
  std::string right;
  for (auto const& line : data) {
    left = line.substr(0, line.find("-"));
    right = line.substr(line.find("-")+1, line.length()-line.find("-")+1);

    if (!paths.count(left))
      paths.insert({left, {}});

    if (!paths.count(right))
      paths.insert({right, {}});

    paths.at(left).insert(right);
    paths.at(right).insert(left);

    if (isSmallCave(left))
      small_caves.insert({left, false});
    else if (isLargeCave(left)) {
      large_caves.insert({left, 0});
    }

    if (isSmallCave(right))
      small_caves.insert({right, false});
    else if (isLargeCave(right)) {
      large_caves.insert({right, 0});
    }

     
  }
  small_caves.at("start") = true;

  return {paths, small_caves, large_caves};
}

void findPaths(CaveInfo& cave, std::string const& cave_name) {
  if (isSmallCave(cave_name))
    cave.small_caves.at(cave_name) = true;

  if(cave_name == "end") 
    cave.num_routes++;
  else {
    auto& routes = cave.paths.at(cave_name);
    for (auto const& route : routes) {
      if (!(isSmallCave(route) && cave.small_caves.at(route)))
        findPaths(cave, route);
    }
  }


  if (isSmallCave(cave_name))
    cave.small_caves.at(cave_name) = false;
}

void solve(CaveInfo& cave) {
  findPaths(cave, "start");

  std::cout << "Number of paths found: " << cave.num_routes << "\n";
}

void testLarge() {
  std::vector<std::string> data = {
    "dc-end",
    "HN-start",
    "start-kj",
    "dc-start",
    "dc-HN",
    "LN-dc",
    "HN-end",
    "kj-sa",
    "kj-HN",
    "kj-dc"
  };

  auto cave = getMap(data);
  solve(cave);

}

void testEvenLarger() {
  std::vector<std::string> data = {
    "fs-end",
    "he-DX",
    "fs-he",
    "start-DX",
    "pj-DX",
    "end-zg",
    "zg-sl",
    "zg-pj",
    "pj-he",
    "RW-he",
    "fs-DX",
    "pj-RW",
    "zg-RW",
    "start-pj",
    "he-WI",
    "zg-he",
    "pj-fs",
    "start-RW"
  };

  auto cave = getMap(data);
  solve(cave);

}

void test() {
  std::vector<std::string> data = {
    "start-A",
    "start-b",
    "A-c",
    "A-b",
    "b-d",
    "A-end",
    "b-end"
  };

  auto cave = getMap(data);
  solve(cave);

}

void solution() {
  std::ifstream in("input.data");
  auto data = getData(in);
  auto cave = getMap(data);

  solve(cave);
  in.close();

}

int main() {
  test();
  testLarge();
  testEvenLarger();
  solution();
  return 0;
}
