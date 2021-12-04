#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

static int const BINGO_SIZE = 25;
static int const BINGO_ROW_SIZE = 5;
static int const BINGO_COL_SIZE = 5;

void getBingoNumbers(std::ifstream& in, std::vector<int>& bingo_nums) {
  //parse bingo nums
  std::string line;
  std::getline(in, line);
  std::stringstream ss(line);
  for(int i; ss >> i;) {
    bingo_nums.push_back(i);
    if (ss.peek() == ',')
      ss.ignore();
  }
}

void getBingoCards(std::ifstream& in, std::vector<int*>& bingo_cards) {
  while (in.good()) {
    int* bingo_card = new int[BINGO_SIZE];
    int i = 0;
    while (i < BINGO_SIZE) {
      in >> bingo_card[i++];
    }
    bingo_cards.push_back(bingo_card);
  }

  bingo_cards.pop_back(); //get rid of empty line
}

int getNumToBingo(std::vector<int> const& bingo_nums, std::vector<int*> const& bingo_cards) {
  for (auto const* bc : bingo_cards) {
    for(int i = 0; i < BINGO_SIZE; i++) {
      std::cout << bc[i];
    }
    std::cout << std::endl;
  }

  return 0;
}
struct Binfo { 
  int num_to_win = 0;
  int winning_number = 0;
  int sum_unmarked = 0;
};

bool checkCard(int const bingo_card[BINGO_SIZE], 
               std::vector<int> const& bingo_nums, Binfo& bi) {

  int bingo_row[BINGO_ROW_SIZE] = {0}; //once one of these reaches 5, bingo!
  int bingo_col[BINGO_COL_SIZE] = {0}; //once one of these reaches 5, bingo!

  int marked_sum = 0;
  int all_sum = 0;
  int num_to_win = 0;
  for (auto num : bingo_nums) {
    num_to_win++;
    for (int i = 0; i < BINGO_SIZE; i++) {
      //keep track of the sum of the bingo card first time you go through
      if (num_to_win == 1) {
        all_sum += bingo_card[i];
      }

      if (bingo_card[i] == num) {
        marked_sum += num;
        if (++bingo_row[i/BINGO_COL_SIZE] == BINGO_ROW_SIZE) {
          bi = {num_to_win, num, all_sum - marked_sum};
          return true;
        } else if (++bingo_col[i%BINGO_ROW_SIZE] == BINGO_COL_SIZE) {
          bi = {num_to_win, num, all_sum - marked_sum};
          return true;
        }
      }
    }
  }
  return false;
}

int main() {
  std::ifstream in("04_1.data");

  std::vector<int> bingo_nums;
  getBingoNumbers(in, bingo_nums);
  
  std::vector<int*> bingo_cards;
  getBingoCards(in, bingo_cards);

  Binfo winning_binfo;
  Binfo bi;

  for (auto* bc : bingo_cards) {
    if (checkCard(bc, bingo_nums, bi) && bi.num_to_win > winning_binfo.num_to_win) {
      winning_binfo = bi;
    }
  }

  std::cout << winning_binfo.sum_unmarked * winning_binfo.winning_number << std::endl; 

  for (auto* ptr : bingo_cards) 
    delete [] ptr;

  in.close();
  return 0;
}
