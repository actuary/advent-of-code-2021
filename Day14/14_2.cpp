#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>


struct PolymerFormula {
  std::string polymer_template;
  std::map<std::string, std::string> insertion_rules;
};

PolymerFormula getData(std::string file_path = "input.data") {
  PolymerFormula pf;

  std::ifstream in(file_path);
  
  std::getline(in, pf.polymer_template);
  
  std::string line;
  std::getline(in, line); //empty line;
  while (std::getline(in, line)) {
    pf.insertion_rules.insert({line.substr(0, line.find(" -> ")), 
                               std::string() + line[line.length()-1]});
  }

  in.close(); 

  return pf;
}

void countChars(std::string const& polymer_template, std::map<std::string, long>& counts) {
  for(auto const& ch : polymer_template) {
    if (counts.find(std::string() + ch) != counts.end())
      counts[std::string() + ch]++;
    else
      counts.insert({std::string() + ch, 1});
  }
}

void printCounts(std::map<std::string, long> const& counts) {
  for (auto const& key : counts) {
    std::cout << key.first << " -> " << key.second << "\n";
  }
}

long calcMaxLessMin(std::map<std::string, long> const& counts) {
  std::vector<long> counts_vec;
  
  for (auto const& key : counts) {
    counts_vec.push_back(key.second);
  }
  return *std::max_element(counts_vec.begin(), counts_vec.end()) 
       - *std::min_element(counts_vec.begin(), counts_vec.end());
}

static int const STEPS = 40;
using CharCounts = std::map<std::string, long>;

//kind of like a 3D
static std::array<std::array<std::array<CharCounts, 26>, 26>, STEPS + 1> results;

void initialiseBottomLayer() {
  for (int i = 0; i < 26; i++) {
    for(int j = 0; j < 26; j++) {
      if (i == j) {
        results[0][i][j].insert({std::string() + (char)('A' + i), 2});
      } else {
        results[0][i][j].insert({std::string() + (char)('A' + i), 1});
        results[0][i][j].insert({std::string() + (char)('A' + j), 1});
      }
    }
  }
}

CharCounts combineMaps(CharCounts const& left, CharCounts const& right) {
  CharCounts cc = left;
  
  for (auto elem : right) {
    if (cc.find(elem.first) != cc.end()) {
      cc[elem.first] += elem.second;
    } else {
      cc.insert(elem);
    }
  }

  return cc;
}

void fillResults(PolymerFormula const& pf) {
  for (int step = 1; step <= STEPS; step++) {
    for (int i = 0; i < 26; i++) {
      for(int j = 0; j < 26; j++) {
        std::string val = (char)('A' + i) + std::string() + (char)('A' + j);

        if(pf.insertion_rules.find(val) == pf.insertion_rules.end()) {
          continue;
        }
      
        std::string new_char = pf.insertion_rules.at(val);

        std::string left = val[0] + new_char;
        std::string right = new_char + val[1];

        CharCounts const& left_map = results[step-1][left[0] - 'A'][left[1] - 'A'];
        CharCounts const& right_map = results[step-1][right[0] - 'A'][right[1] - 'A'];
        CharCounts new_map = combineMaps(left_map, right_map);

        new_map[new_char]--;

        results[step][i][j] = new_map;
      }
    }
  }
}

void solve(PolymerFormula const& pf) {
  fillResults(pf);
 

  CharCounts cc = results[STEPS][pf.polymer_template[0] - 'A'][pf.polymer_template[1] - 'A'];

  for (int i = 1; i < pf.polymer_template.length() - 1; i++) {
    cc = combineMaps(cc, results[STEPS][pf.polymer_template[i] - 'A'][pf.polymer_template[i+1] - 'A']);
    cc[std::string() + pf.polymer_template[i]]--;
  }
  std::cout << "Result: " << calcMaxLessMin(cc) << "\n";
}

void test() {
  PolymerFormula pf {
    .polymer_template = "NNCB",
    .insertion_rules = {
      {"CH", "B"},
      {"HH", "N"},
      {"CB", "H"},
      {"NH", "C"},
      {"HB", "C"},
      {"HC", "B"},
      {"HN", "C"},
      {"NN", "C"},
      {"BH", "H"},
      {"NC", "B"},
      {"NB", "B"},
      {"BN", "B"},
      {"BB", "N"},
      {"BC", "B"},
      {"CC", "N"},
      {"CN", "C"},
    }
  };

  initialiseBottomLayer(); 
  solve(pf);
}

void solution() {
  PolymerFormula pf = getData();

  solve(pf);
}
int main() {
  //test();
  solution();
  return 0;
}
