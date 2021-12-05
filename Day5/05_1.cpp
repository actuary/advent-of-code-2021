#include <iostream>
#include <fstream>
#include <vector>

struct Point {
  int x, y;
};

struct Line {
  Point a, b;
};

void readNonNumeric(std::ifstream& in) {
  char ch;
  while (in.get(ch) && !std::isdigit(ch));
  if (std::isdigit(ch))
    in.putback(ch);
}

void readNextInt(std::ifstream& in, int& num) {
  readNonNumeric(in);
  in >> num;
}

bool readLine(std::ifstream& in, Line& line) {
  readNextInt(in, line.a.x);
  readNextInt(in, line.a.y);
  readNextInt(in, line.b.x);
  readNextInt(in, line.b.y);

  return in.good();
}

bool isHorizontal(Line const& line) {
  return line.a.y == line.b.y;
}

bool isVertical(Line const& line) {
  return line.a.x == line.b.x;
}

void fillSeabed(Line const& line, int seabed[][1000], int& count) {
  
  if (isHorizontal(line)) {
    int sign = line.b.x - line.a.x < 0 ? -1 : 1;
    for (int x = line.a.x; x != line.b.x + sign; x+= sign) {
      if (++seabed[x][line.a.y] == 2) {
        count++;
      }
    }

  } else if (isVertical(line)) {
    int sign = line.b.y - line.a.y < 0 ? -1 : 1;
    for (int y = line.a.y; y != line.b.y + sign; y+= sign) {
      if (++seabed[line.a.x][y] == 2) {
        count++;
      }
    }
  }
}

int main() {
  std::ifstream in("input.data");

  std::vector<Line> lines;

  Line line;
  int seabed[1000][1000] = {0};
  int count = 0;
  while (readLine(in, line)) {
    if (isHorizontal(line) || isVertical(line))
      fillSeabed(line, seabed, count);
  }
  
  std::cout << count << std::endl;


  
  in.close();
  return 0;
}
