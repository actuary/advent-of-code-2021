#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>


struct PolymerFormula {
  std::string polymer_template;
  std::map<std::string, char> insertion_rules;
};

PolymerFormula getData(std::string file_path = "input.data") {
  PolymerFormula pf;

  std::ifstream in(file_path);
  
  std::getline(in, pf.polymer_template);
  
  std::string line;
  std::getline(in, line); //empty line;
  while (std::getline(in, line)) {
    pf.insertion_rules.insert({line.substr(0, line.find(" -> ")), 
                               line[line.length()-1]});
  }

  in.close(); 

  return pf;
}

void countChars(std::string const& polymer_template, std::map<char, int>& counts) {
  for(auto const& ch : polymer_template) {
    if (counts.find(ch) != counts.end())
      counts[ch]++;
    else
      counts.insert({ch, 1});
  }
}
struct Node {
  std::string data;
  Node* left = nullptr;
  Node* right = nullptr;

  Node(std::string data) : data(data) {}
};

void printPF(PolymerFormula const& pf) {
  std::cout << pf.polymer_template << "\n";

  for (auto const& rule : pf.insertion_rules) {
    std::cout << rule.first << " -> " << rule.second << "\n";
  }

  std::cout << "\n";
}

void doStep(PolymerFormula& pf) {
  std::string result = std::string() + pf.polymer_template[0];
  for(int idx = 0; idx < pf.polymer_template.length() - 1; idx++) {
    std::string lookup = std::string() + pf.polymer_template[idx];
    lookup += pf.polymer_template[idx + 1];
    result += pf.insertion_rules.at(lookup);
    result += pf.polymer_template[idx+1]; 
  }
  
  pf.polymer_template = result;
}

void printCounts(std::map<char, int> const& counts) {
  for (auto const& key : counts) {
    std::cout << key.first << " -> " << key.second << "\n";
  }
}

int calcMaxLessMin(std::map<char, int> const& counts) {
  std::vector<int> counts_vec;
  
  for (auto const& key : counts) {
    counts_vec.push_back(key.second);
  }
  return *std::max_element(counts_vec.begin(), counts_vec.end()) 
       - *std::min_element(counts_vec.begin(), counts_vec.end());
}

void solve(PolymerFormula& pf) {
  int step = 0;
  std::string result;
  while (step++ < 10) {
    doStep(pf);
  }
  
  std::map<char, int> counts = {};
  countChars(pf.polymer_template, counts);
  printCounts(counts);
  std::cout << "Result: " << calcMaxLessMin(counts) << "\n";  
}

void test() {
  PolymerFormula pf {
    .polymer_template = "NNCB",
    .insertion_rules = {
      {"CH", 'B'},
      {"HH", 'N'},
      {"CB", 'H'},
      {"NH", 'C'},
      {"HB", 'C'},
      {"HC", 'B'},
      {"HN", 'C'},
      {"NN", 'C'},
      {"BH", 'H'},
      {"NC", 'B'},
      {"NB", 'B'},
      {"BN", 'B'},
      {"BB", 'N'},
      {"BC", 'B'},
      {"CC", 'N'},
      {"CN", 'C'},
    }
  };
  
  solve(pf);
}

void solution() {
  PolymerFormula pf = getData();

  solve(pf);
}
int main() {
  test();
  solution();
  return 0;
}
