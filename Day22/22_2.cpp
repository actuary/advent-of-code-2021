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
#include <list>

struct Range {
  int min, max;

  long long length() const {
    return max - min + 1;
  }

  friend bool operator==(Range const& left, Range const& other) {
    return left.min == other.min && left.max == other.max;
  }
};

struct RebootStep {
  Range x, y, z;
  bool on = false;

  friend std::ostream& operator<<(std::ostream& os, RebootStep step) {
    os << (step.on ? "on " : "off ") 
       << "x=" << step.x.min << ".." << step.x.max << ","
       << "y=" << step.y.min << ".." << step.y.max << ","
       << "z=" << step.z.min << ".." << step.z.max 
       << " : VOLUME : " << step.volume();

    return os;
  }

  long long volume() const {
    return x.length() * y.length() * z.length();
  }

  friend bool operator==(RebootStep const& left, RebootStep const& other) {
    return (left.x == other.x && left.y == other.y && left.z == other.z);
  }

  friend bool operator!=(RebootStep const& left, RebootStep const& other) {
      return !(left == other);
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

bool isOverlapping(RebootStep const& stepL, RebootStep const& stepR) {
  
  return (stepL.x.max >= stepR.x.min && stepR.x.max >= stepL.x.min) 
    &&   (stepL.x.max > stepR.x.min || stepR.x.max > stepL.x.min) //so not only touching on edge
    &&   (stepL.y.max >= stepR.y.min && stepR.y.max >= stepL.y.min) 
    &&   (stepL.y.max > stepR.y.min || stepR.y.max > stepL.y.min) 
    &&   (stepL.z.max >= stepR.z.min && stepR.z.max >= stepL.z.min)
    &&   (stepL.z.max > stepR.z.min || stepR.z.max > stepL.z.min);
}

//returns up to 6 cuboids contained in stepL not present in stepR
std::vector<RebootStep> diffCube(RebootStep const& stepL, RebootStep const& stepR) {
  if (!isOverlapping(stepL, stepR) && !isOverlapping(stepR, stepL)) {
    return {stepL};
  }

  std::vector<RebootStep> result;

  //chopping up non overlapping. We'll priority top+bottom slices, then 
  //left + right, (so front+back slices are most constrained)

  //top slice
  if (stepL.z.max > stepR.z.max) 
    result.push_back({ stepL.x, stepL.y, {stepR.z.max+1, stepL.z.max}, stepL.on});

  //bottom slice
  if (stepL.z.min < stepR.z.min) 
    result.push_back({ stepL.x, stepL.y, {stepL.z.min, stepR.z.min-1}, stepL.on});

  //left slice
  if (stepL.x.min < stepR.x.min) 
    result.push_back({ {stepL.x.min, stepR.x.min-1},
                        stepL.y, 
                        {std::max(stepL.z.min, stepR.z.min), 
                         std::min(stepL.z.max, stepR.z.max)}, stepL.on});

  //right slice
  if (stepL.x.max > stepR.x.max) 
    result.push_back({ {stepR.x.max+1, stepL.x.max},
                        stepL.y, 
                        {std::max(stepL.z.min, stepR.z.min), 
                         std::min(stepL.z.max, stepR.z.max)}, stepL.on});

  //front slice
  if (stepL.y.min < stepR.y.min) 
    result.push_back({ {std::max(stepL.x.min, stepR.x.min), 
                         std::min(stepL.x.max, stepR.x.max)},
                         {stepL.y.min, stepR.y.min-1}, 
                        {std::max(stepL.z.min, stepR.z.min), 
                         std::min(stepL.z.max, stepR.z.max)}, stepL.on});

  //back slice
  if (stepL.y.max > stepR.y.max) 
    result.push_back({ {std::max(stepL.x.min, stepR.x.min), 
                         std::min(stepL.x.max, stepR.x.max)},
                         {stepR.y.max + 1, stepL.y.max}, 
                        {std::max(stepL.z.min, stepR.z.min), 
                         std::min(stepL.z.max, stepR.z.max)}, stepL.on});
 
  return result;
}

std::vector<RebootStep> intersectCube(RebootStep const& stepL, RebootStep const& stepR) {
  if (!isOverlapping(stepL, stepR) && !isOverlapping(stepR, stepL)) {
    return {};
  }
  std::vector<RebootStep> result;
  result.push_back({ {std::max(stepL.x.min, stepR.x.min), 
                       std::min(stepL.x.max, stepR.x.max)},
                       {std::max(stepL.y.min, stepR.y.min), 
                       std::min(stepL.y.max, stepR.y.max)}, 
                      {std::max(stepL.z.min, stepR.z.min), 
                       std::min(stepL.z.max, stepR.z.max)}, stepL.on});
  return result;
}

std::vector<RebootStep> allDiffs(RebootStep const& cubeL, std::vector<RebootStep> const& cubesR) {
  std::vector<RebootStep> diffs = {cubeL};
  for (auto const& cubeR : cubesR) {
    std::vector<RebootStep> diffs_tmp = {};
    for (auto const& cube : diffs) {
      auto diff_curr = diffCube(cube, cubeR);
      diffs_tmp.insert(diffs_tmp.end(), diff_curr.begin(), diff_curr.end());
    }
    diffs = diffs_tmp;
  }
  return diffs;
}

long long totalVolume(std::vector<RebootStep> const& cubes) {
  long long volume = 0;
  for (auto const& cube : cubes) {
    volume += cube.volume();
  }
  return volume;
}

void solve(std::vector<RebootStep>& steps) {
  //strategy is: 
  //  work backwards through the list, if "current cube" is on
  //  then get the diff* between this cube and the previously seen cubes
  //  and switch only these on.
  //  Reason for this as previously seen cubes have either:
  //    already been added as "on"
  //    switched off after "current cube"
  //
  //
  //  * The diff is obtained by by slicing up the original cuboid, after
  //    removing the intersecting piece, into further cuboids
  //    These cuboids will all be _disjoint_
  //
  //    Strategy for diffing is to first slice top and bottom, 
  //    then left and right, and then remaining is front and back.
  //    so front/back is usually the "smallest"
  //    
  long long total_on = 0; 

  std::vector<RebootStep> seen_cubes = {};
  for (int i = steps.size() - 1; i >= 0; i--) {

    //either its on and we've already seen it, or set off later
    if (steps[i].on) { 
      auto diffs = allDiffs(steps[i], seen_cubes);
      total_on += totalVolume(diffs); 
    }

    seen_cubes.insert(seen_cubes.end(), steps[i]);
  }

  std::cout << total_on << "\n";
}

void test() {

  auto data = getData("input2.test");
  solve(data);
}

void solution() {
  auto data = getData();
  solve(data);
}

int main() {
  //test();
  solution();
  return 0;
}
