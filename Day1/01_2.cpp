#include <iostream>
#include <fstream>

int main() {
  std::ifstream in("01_1.data");  
  int const kSize = 3;

  int window[kSize];
  for (int i = 0; i < kSize; i++) 
    in >> window[i];

  int new_num, count = 0, front = 0;
  while ((in >> new_num)) {
    count += (new_num > window[front]); //last element
    window[front] = new_num;
    front = (front + 1)% kSize;
  }
  
  std::cout << "There are " << count << " increases.\n";
  in.close();
}
