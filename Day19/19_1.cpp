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

enum class kAxes {X, Y, Z, nX, nY, nZ};
enum class kDirection {POSITIVE, NEGATIVE};

std::ostream& operator<<(std::ostream& os, kAxes axis) {
  switch (axis) {
    case kAxes::X: std::cout << " X"; break;
    case kAxes::Y: std::cout << " Y"; break;
    case kAxes::Z: std::cout << " Z"; break;
    case kAxes::nX: std::cout << "-X"; break;
    case kAxes::nY: std::cout << "-Y"; break;
    case kAxes::nZ: std::cout << "-Z"; break;
  }
  return os;
}
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

  friend std::ostream& operator<<(std::ostream& os, Orientation orient) {
    os << '(' << orient.x << ',' << orient.y << ',' << orient.z << ')';
    return os;
  }

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
  bool operator<(const Position &other) const { 
    return (x < other.x)
           || (x == other.x && y < other.y)
           || (x == other.x && y == other.y && z < other.z);
  }

};

Position operator-(Position const& left, Position const& right) {

  return {
    left.x - right.x,
    left.y - right.y,
    left.z - right.z
  };
}

Position operator+(Position const& left, Position const& right) {

  return {
    left.x + right.x,
    left.y + right.y,
    left.z + right.z
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
  switch (axis) {
    case kAxes::X: return orig.x;
    case kAxes::Y: return orig.y;
    case kAxes::Z: return orig.z; 
    case kAxes::nX: return -orig.x;
    case kAxes::nY: return -orig.y;
    case kAxes::nZ: return -orig.z; 
    default: assert(false);
  }
}

Position rotate(Position const& orig, Orientation o) {
  return {rotateCoord(orig, o.x), rotateCoord(orig, o.y), rotateCoord(orig, o.z)};
}

kAxes rotateCoord(Orientation orig, kAxes axis) {
  switch (axis) {
    case kAxes::X: return orig.x;
    case kAxes::Y: return orig.y;
    case kAxes::Z: return orig.z;
    case kAxes::nX: return kNegate.at(orig.x);
    case kAxes::nY: return kNegate.at(orig.y);
    case kAxes::nZ: return kNegate.at(orig.z);
    default: assert(false);
  }
}

Orientation rotate(Orientation orig, Orientation o) {
  return {rotateCoord(orig, o.x), rotateCoord(orig, o.y), rotateCoord(orig, o.z)};
}

void printData(std::vector<std::vector<Position>> scanners) {
  for (int i = 0; i < scanners.size(); i++) {
    std::cout << "--- scanner " << i << " ---\n";
    for(auto const& beacon : scanners[i]) {
      std::cout << beacon << "\n";
    }
    std::cout << '\n';
  }
}

std::vector<std::vector<Position>> getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  
  std::vector<std::vector<Position>> scanners = {};
  
  std::string line;
  while (std::getline(in, line)) {
    if (line.substr(0, 3) == "---") {
      std::vector<Position> beacons = {};
      while (std::getline(in, line) && line.length() != 0) {
        std::istringstream csv(line);
        Position position; 
        csv >> position.x;
        csv.ignore(1);
        csv >> position.y;
        csv.ignore(1);
        csv >> position.z;
        csv.ignore(1);
        beacons.push_back(position);
      }
      scanners.push_back(beacons);
    }
  }
  in.close();
  return scanners;
}

struct Offset {
  Position relative_offset;
  Orientation orientation;
};

struct RelOffset {
  int scanner;
  Offset offset;
};

//gets offset of one scanner to another
bool compareScanners(std::vector<Position> const& scannerL, 
                     std::vector<Position> const& scannerR, 
                     Offset& offset) {
  
  std::unordered_map<Position, int> counts;
  for (auto const& orientation: kOrientations) {
    counts = {};
    for (auto const& beaconL : scannerL) {
      for (auto const& beaconR : scannerR) {
          Position rotated = rotate(beaconR, orientation);
          
          Position dist = beaconL - rotated;
          if (counts.find(dist) == counts.end()) {
            counts.insert({dist, 0});
          }
          counts[dist]++;

          if (counts[dist] >= 12) {
            offset = {dist, orientation};
            return true;
          }
      }
    }
  }

  return false;
}

void solve(std::vector<std::vector<Position>> const& scanners) {
  std::cout << "Comparing scanners\n";
  std::unordered_map<int, std::vector<RelOffset>> rel_offsets;
  for (int i = 0; i < scanners.size(); i++) {
    for (int j = 0; j < scanners.size(); j++) {
      Offset offset;
      if (i != j && compareScanners(scanners[i], scanners[j], offset)) {
        if (rel_offsets.find(i) == rel_offsets.end())
          rel_offsets.insert({i, {}});
        rel_offsets[i].push_back({j, offset});
      }
    }
    std::cout << "\t comparing against " << i << "\n";
  }

  std::unordered_map<int, Offset> abs_offsets;
  abs_offsets.insert({12, 
                      {.relative_offset = {0,0,0}, 
                       .orientation = {kAxes::X, kAxes::Y, kAxes::Z}}});

  std::vector<int> stack;
  int const BASE_SCANNER = 12;
  for (int i = 0; i < scanners.size(); i++)
    if (i != BASE_SCANNER) stack.push_back(i);
 
  std::cout << "Finding scanners\n";
  bool found;
  while (!stack.empty()) {
    found =false;
    for (int el : stack) {
      for (auto const& [source, abs_offset] : abs_offsets) { //clang-tidy NOLINT
        auto const& rel_offset = rel_offsets.at(source);
        auto it = std::find_if(rel_offset.begin(), rel_offset.end(), 
            [el](RelOffset const& ro) { return ro.scanner == el; });

        if (it != rel_offset.end()) {
          Orientation abs_orient = rotate(it->offset.orientation, abs_offset.orientation);
          Position abs_offset_to_source = rotate(it->offset.relative_offset, abs_offset.orientation);
          Position abs_offset_to_0 = abs_offset.relative_offset + abs_offset_to_source;
          abs_offsets.insert({el, {abs_offset_to_0, abs_orient}});
          stack.erase(std::remove(stack.begin(), stack.end(), el));
          found = true;
          break;
        }
      }
      if (found)
        break;
    }
  }

  std::cout << "Finding beacons\n";
  std::set<Position> beacons;
  for (int scanner = 0; scanner < scanners.size(); scanner++) {
    for (auto const& beacon : scanners[scanner]) {
      Position abs_position = beacon;
      abs_position = rotate(beacon, abs_offsets.at(scanner).orientation);
      abs_position = abs_position + abs_offsets.at(scanner).relative_offset;

      beacons.insert(abs_position);
    }
  } 

  std::cout << beacons.size() << " beacons. \n";
}

void test() {
  solve(getData("input.test"));

}

void solution() {
  solve(getData());
}

int main() {
  //test();
  solution();
  return 0;
}
