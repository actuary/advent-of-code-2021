#include <iostream>
#include <fstream>
#include <vector>

void readNonNumeric(std::ifstream& in) {
  char ch;
  while (in.get(ch) && !std::isdigit(ch));
  if (std::isdigit(ch))
    in.putback(ch);
}

std::vector<int> split(std::ifstream& in) {
  int num;
  std::vector<int> result;

  readNonNumeric(in);
  while (in.good()) {
    in >> num;
    result.push_back(num);
    readNonNumeric(in);
  }

  return result;
}

int const NO_FISH = 256;
long results[256];

long numChildren(int days_from_1st_fish) {
  if (days_from_1st_fish < 0)
    return 0;

  if (results[days_from_1st_fish] != -1) 
    return results[days_from_1st_fish];

  long x = 1 + (days_from_1st_fish)/7;

  //then go through children and add them
  for (int y = 0; y < 1 + days_from_1st_fish/7; y++) {
    x += numChildren(days_from_1st_fish - 7*y - 9);
  }
  
  results[days_from_1st_fish] = x;
  return x;
}

void init_results() {
  for(int i = 0; i < 256; i++) 
    results[i] = -1;
}

void test() {
  std::vector<int> data_test = {3,4,3,1,2};

  init_results();
  long count_test = 0;
  for (int num : data_test) {
    count_test += 1 + numChildren(NO_FISH - num - 1);
  }
 
  std::cout << count_test << " fish.\n";
}

int main() {
  test(); 
  std::ifstream in("input.data");
  std::vector<int> data = split(in);

  init_results();
  long count = 0;
  for (int num : data) {
    count += 1 + numChildren(NO_FISH - num - 1);
  }

  std::cout << count << " fish.\n";

  in.close();
  return 0;
}
