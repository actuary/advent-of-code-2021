#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> getData(std::ifstream& in) {
  std::vector<std::string> result;
  std::string line;

  while (std::getline(in, line)) {
    result.push_back(line);
  }

  return result;
}

struct Cavern {
  std::vector<std::string>& data;
  std::vector<std::vector<bool>>& flashed; 
  size_t len_x = 0;
  size_t len_y = 0;
  int flashes = 0;
};

struct Position {
  int x, y;
};

void printCavern(Cavern const& cavern, int step) {
  std::cout << "_ Step " << step << "__\n";
  for (int x = 0; x < cavern.len_x; x++) {
    for (int y = 0; y < cavern.len_y; y++) {
      std::cout << cavern.data[x][y];
    }
    std::cout << "\n";
  }
  std::cout << "Flashes: " << cavern.flashes << "\n\n";
}

//when thing is 9
void doFlash(Cavern& cavern, Position const& pos) { 
  if (pos.x < 0 || pos.y < 0 || pos.x >= cavern.len_x || pos.y >= cavern.len_y) {
    return;
  }

  if (!cavern.flashed[pos.x][pos.y]) 
    cavern.data[pos.x][pos.y]++;

  if (cavern.data[pos.x][pos.y] != '9' + 1)
    return;

  if (cavern.flashed[pos.x][pos.y]) {
    cavern.data[pos.x][pos.y] = '9';
    return;
  }

  cavern.flashed[pos.x][pos.y] = true;
  cavern.flashes++;
  cavern.data[pos.x][pos.y] = '0';

  for (int x = pos.x - 1; x <= pos.x + 1; x++)
    for (int y = pos.y - 1; y <= pos.y + 1; y++)
      if (!(x == pos.x && y == pos.y))
        doFlash(cavern, {x, y});

}

void doStep(Cavern& cavern) {
  for (int x = 0; x < cavern.len_x; x++) {
    for (int y = 0; y < cavern.len_y; y++) {
      doFlash(cavern, {x, y});
    }
  }
}

void resetFlashed(Cavern& cavern) {
  for (int x = 0; x < cavern.len_x; x++) {
    for (int y = 0; y < cavern.len_y; y++) {
      cavern.flashed[x][y] = false;
    }
  }
}
void solve(std::vector<std::string>& data) {
  int const STEPS = 100;
  size_t len_x = data[0].length();
  size_t len_y = data.size();

  std::vector<std::vector<bool>> flashed = {{0}}; 

  flashed.resize(len_y);
  for (auto& line: flashed) 
    line.resize(len_x);

  Cavern cavern = {data, flashed, len_x, len_y, 0};

  //printCavern(cavern, 0);
  for (int step = 0; step < STEPS; step++) {
    doStep(cavern);
    resetFlashed(cavern);
    //printCavern(cavern, step + 1);
  }

  std::cout << "After " << STEPS << " steps, there were " << cavern.flashes << " flashes.\n";
}

void test() {
  std::vector<std::string> data = {
    "5483143223",
    "2745854711",
    "5264556173",
    "6141336146",
    "6357385478",
    "4167524645",
    "2176841721",
    "6882881134",
    "4846848554",
    "5283751526"
  };

  solve(data);
}

void solution() {
  std::ifstream in("input.data");
  std::vector<std::string> data = getData(in);
  in.close();

  solve(data);
}
int main() {
  test();
  solution();
  return 0;
}
