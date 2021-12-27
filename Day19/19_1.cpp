#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <tuple>
#include <cmath>

enum class kAxes {X, Y, Z, nX, nY, nZ};
enum class kDirection {POSITIVE, NEGATIVE};

std::map<kAxes, kDirection> kSigns = {
  {kAxes::X, kDirection::POSITIVE}, 
  {kAxes::Y, kDirection::POSITIVE}, 
  {kAxes::Z, kDirection::POSITIVE}, 
  {kAxes::nX, kDirection::NEGATIVE}, 
  {kAxes::nY, kDirection::NEGATIVE}, 
  {kAxes::nZ, kDirection::NEGATIVE} 
};

std::map<kAxes, kAxes> kNegate = {
  {kAxes::X, kAxes::nX}, 
  {kAxes::Y, kAxes::nY}, 
  {kAxes::Z, kAxes::nZ}, 
  {kAxes::nX, kAxes::X}, 
  {kAxes::nY, kAxes::Y}, 
  {kAxes::nZ, kAxes::Z} 
};

//24 possible orientations the ship could be in. 
//This is the absolute (canonical) orientation
//  Z
//  |
//  |   / Y
//  |  /
//  | /
//  |/_____ X
//             
//

struct Orientation {
  kAxes x, y, z;
};

std::vector<Orientation> kOrientations = {
  {kAxes::X,  kAxes::nX, kAxes::Z},
  {kAxes::X,  kAxes::Y,  kAxes::Z}, //canonical 
  {kAxes::nY, kAxes::X,  kAxes::Z},
  {kAxes::nX, kAxes::nY, kAxes::Z},
  {kAxes::nZ, kAxes::nY, kAxes::nX},
  {kAxes::Z,  kAxes::nY, kAxes::X},
  {kAxes::nY, kAxes::nX, kAxes::nZ},
  {kAxes::X,  kAxes::nY, kAxes::nZ},
  {kAxes::Y,  kAxes::X,  kAxes::nZ},
  {kAxes::nX, kAxes::Y,  kAxes::nZ},
  {kAxes::Z,  kAxes::Y,  kAxes::nZ},
  {kAxes::nZ, kAxes::Y,  kAxes::X},
  {kAxes::Z,  kAxes::nX, kAxes::nY},
  {kAxes::X,  kAxes::nZ, kAxes::Y},
  {kAxes::nZ, kAxes::X,  kAxes::nY},
  {kAxes::nX, kAxes::nZ, kAxes::nY},
  {kAxes::nY, kAxes::Z,  kAxes::nX},
  {kAxes::Y,  kAxes::Z,  kAxes::X},
  {kAxes::nZ, kAxes::nX, kAxes::Y},
  {kAxes::X,  kAxes::Z,  kAxes::nY},
  {kAxes::Z,  kAxes::X,  kAxes::Y},
  {kAxes::nX, kAxes::Z,  kAxes::Y},
  {kAxes::Y,  kAxes::nZ, kAxes::nX},
  {kAxes::nY, kAxes::nZ, kAxes::X}
};

struct Position {
  int x, y, z;
  friend std::ostream& operator<<(std::ostream& os, Position pos) {
    os << '(' << pos.x << ',' << pos.y << ',' << pos.z << ')';
    return os;
  }
  bool operator==(const Position &other) const { 
    return (x == other.x
            && y == other.y
            && z == other.z);
  }

};

Position operator-(Position const& left, Position const& right) {

  return {
    left.x - right.x,
    left.y - right.y,
    left.z - right.z
  };
}

template <>
struct std::hash<Position>
{
  std::size_t operator()(const Position& k) const
  {
    using std::hash;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return ((hash<int>()(k.x)
             ^ (hash<int>()(k.y) << 1)) >> 1)
             ^ (hash<int>()(k.z) << 1);
  }
};

int rotateCoord(Position const& orig, kAxes axis) {
  int sign = 1;
  if (kSigns.at(axis) == kDirection::NEGATIVE) {
    sign = -1;
    axis = kNegate.at(axis);
  }
  
  int val;
  switch (axis) {
    case kAxes::X: val = sign * orig.x; break;
    case kAxes::Y: val = sign * orig.y; break;
    case kAxes::Z: val = sign * orig.z; break;
    default: assert(false);
  }

  return val;
}

Position rotate(Position const& orig, Orientation o) {
  return {rotateCoord(orig, o.x), rotateCoord(orig, o.y), rotateCoord(orig, o.z)};
}

void printData(std::vector<std::vector<Position>> scanners) {
  for (int i = 0; i < scanners.size(); i++) {
    std::cout << "--- scanner " << i << " ---\n";
    for(auto const& beacon : scanners[i]) {
      std::cout << beacon.x << ',' << beacon.y << ',' << beacon.z << '\n';
    }
    std::cout << '\n';
  }
}

std::vector<std::vector<Position>> getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  
  std::vector<std::vector<Position>> scanners = {};
  
  std::string line;
  std::string num;
  while (std::getline(in, line)) {
    if (line.substr(0, 3) == "---") {
      std::vector<Position> beacons = {};
      while (std::getline(in, line) && line.length() != 0) {
        Position position; 
        std::istringstream csv(line);
        csv >> num; 
        position.x = std::stoi(num);
        csv >> num; 
        position.y = std::stoi(num);
        csv >> num; 
        position.z = std::stoi(num);
        beacons.push_back(position);
      }
      scanners.push_back(beacons);
    }
  }
  in.close();
  return scanners;
}

int taxicabDist(Position p, Position q) {
  return std::abs(p.x - q.x) + std::abs(p.y - q.y) + std::abs(p.z - q.z);  
}

struct IndexPair {
  int i, j;
};

std::map<int, std::vector<Position>> getDistances(std::vector<Position>const& scanner) {
  std::map<int, std::vector<Position>> result;

  int dist;
  for (int i = 1; i < scanner.size(); i++ ) {
    for (int j = 0; j < i; j++) {
      dist = taxicabDist(scanner[i], scanner[j]);
      if (result.find(dist) != result.end()) {
        std::cout << "double dist!\n";
      } else {
        result.insert({dist, {}});
      }
      result.at(dist).push_back(scanner[i]);
      result.at(dist).push_back(scanner[j]);
    }
  }

  return result;
}

Position compareScanners(
    std::vector<Position> const& scannerL, std::vector<Position> const& scannerR) {
  
  std::unordered_map<Position, int> counts;
  for (auto const& beaconL : scannerL) {
    for (auto const& beaconR : scannerR) {
      counts = {};
      for (auto const& orientation: kOrientations) {
        Position rotated = rotate(beaconR, orientation);
        
        beaconL - rotated;
        if (counts.find(rotated) == counts.end()) {
          counts.insert({rotated, 0});
        }
        counts[rotated]++;

        if (counts[rotated] >= 5)
          break;
      }
    }
  }

  for (auto const& [position, count] : counts) { //clang-tidy NOLINT
    std::cout << position << ": " << count << '\n';
  }

  return {0, 0, 0};
}

void solve(std::vector<std::vector<Position>> const& scanners) {
  //printData(scanners);

  for (int i = 1; i < 2/*scanners.size()*/; i++) {
    for (int j = 0; j < i; j++) {
      compareScanners(scanners[i], scanners[j]);
    }
  }
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
