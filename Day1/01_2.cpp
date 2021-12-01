#include <iostream>
#include <fstream>

int main() {
  std::ifstream in("01_1.data");  
  int const kSize = 3;

  int window[kSize];
  for (int i = 0; i < kSize; i++) 
    in >> window[i];

  int new_num, count = 0;
  while ((in >> new_num)) {
    count += (new_num > window[0])*1;
    for (int i = 0; i < kSize - 1; i++) {
      window[i] = window[i+1];
    }
    window[kSize-1] = new_num;
  }
  
  std::cout << "There are " << count << " increases.\n";
  in.close();
  return 0;
}
