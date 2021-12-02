#include <iostream>
#include <fstream>


int main() {
  std::ifstream in("02_01.data");
  
  enum Command {FORWARD, DOWN, HORIZONTAL};
  int depth = 0, horiz = 0;

  struct {
    char cmd[11] = {0};
    int amount;
  } sub_move;

  while ((in >> sub_move.cmd) && (in >> sub_move.amount)) {
    switch (sub_move.cmd[0]) {
      case 'f'     : horiz += sub_move.amount; break;
      case 'u'     : depth -= sub_move.amount; break;
      case 'd'     : depth += sub_move.amount; break; 
    }
  }

  std::cout << "Horizontal = " << horiz << ", Depth = " << depth << ". "
            << "Product = " << horiz * depth << "\n";

  in.close();
}
