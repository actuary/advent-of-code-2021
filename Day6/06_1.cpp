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

int numChildren(int days, int time_to_first_fish) {
  if (time_to_first_fish > days)
    return 0;

  int x = 1 + (days - time_to_first_fish)/7;

  //then go through children and add them
  for (int y = 0; y < 1 + (days - time_to_first_fish)/7; y++) {
    x += numChildren(days - time_to_first_fish - 7*y, 9);
  }
  
  return x;
}

int main() {
  std::ifstream in("input.data");
  std::vector<int> data = split(in);

  std::vector<int> data_test = {3,4,3,1,2};

  int count_test = 0;
  for (int num : data_test) {
    count_test += 1 + numChildren(80, num + 1);
  }
 
  std::cout << count_test << " fish.\n";

  int count = 0;
  for (int num : data) {
    count += 1 + numChildren(80, num + 1);
  }

  std::cout << count << " fish.\n";

  in.close();
  return 0;
}
