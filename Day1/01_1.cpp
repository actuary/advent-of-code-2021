#include <iostream>
#include <fstream>

int main() {
  std::ifstream in("01_1.data");  
  int prev_num, curr_num; 
  
  int count = 0;
  in >> prev_num;
  while ((in >> curr_num)) {
    count += (curr_num > prev_num)*1;
    prev_num = curr_num;
  }
  
  std::cout << "There are " << count << " increases.\n";
  in.close();
  return 0;
}
