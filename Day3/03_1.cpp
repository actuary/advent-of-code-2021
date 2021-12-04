#include <cstddef>
#include <iostream>
#include <fstream>


int main() {
  std::ifstream in("03_1.data");
  
  int const NUM_BITS = 12;
 
  int most_common_bits[12] = {0};
  std::string bit_string; 
  int line_count = 0;
  while ((in >>  bit_string)) {
    for (int i = 0; i < NUM_BITS; i++) 
      if (bit_string[i] == '1') most_common_bits[i]++;
    line_count++;
  }

  int mid = line_count / 2;
  int gamma = (most_common_bits[0] > mid);
  int epsilon = (most_common_bits[0] < mid);

  for (int i = 1; i < NUM_BITS; i++) {
    gamma *= 2;
    gamma += (most_common_bits[i] > mid);
    epsilon *= 2;
    epsilon += (most_common_bits[i] < mid);
  }

  std::cout << "Gamma: " << gamma << "\n"; 
  std::cout << "Epsilon: " << epsilon << "\n"; 
  std::cout << "Power consumption: " << gamma * epsilon << "\n";
  in.close();

  return 0;
}
