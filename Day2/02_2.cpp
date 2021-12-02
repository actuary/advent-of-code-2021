#include <iostream>
#include <fstream>


int main() {
  std::ifstream in("02_01.data");
  
  int depth = 0, horiz = 0, aim = 0;
  struct {
    char cmd[11] = {0};
    int amount;
  } move;

  while ((in >> move.cmd) && (in >> move.amount)) {
    switch (move.cmd[0]) {
      case 'f'     : horiz += move.amount; depth += aim * move.amount; break;
      case 'u'     : aim -= move.amount; break;
      case 'd'     : aim += move.amount; break; 
    }
  }

  std::cout << "horizontal = " << horiz << ", depth = " << depth << ". "
            << "product = " << horiz * depth << "\n";

  in.close();
}
