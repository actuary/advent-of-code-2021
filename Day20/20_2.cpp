#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <assert.h>
#include <stack>
#include <set>

struct Image {
  std::vector<std::string> pixels;
  int width;
  int height;
};

struct Data { 
  std::string enhance;
  Image image;
};


Data getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  std::string enhance; 
  std::vector<std::string> img;
 
  std::getline(in, enhance);

  std::string line;
  std::getline(in, line);
  while (std::getline(in, line)) {
    img.push_back(line);
  }

  in.close();

  return {enhance, {img, (int) img[0].length(), (int) img.size()}};
}

void printImage(Image const& image) {
  for (auto const& line : image.pixels) {
    std::cout << line << "\n";
  }
  std::cout << "\n";
}

void printData(Data const& data) {
  std::cout << data.enhance << "\n";
  std::cout << "\n";
  printImage(data.image);
}

Image blankImage(int width, int height, bool fill) {
  std::vector<std::string> blank;
 
  char fill_char = fill ? '#' : '.';
  for (int i = 0; i < height; i++)
    blank.push_back(std::string(width, fill_char));

  return {blank, width, height};
}

bool outOfBounds(int row, int col, int width, int height) {
  return (row < 0 || row >= height || col < 0 || col >= width);
}


Image enlarge(Image const& image, int border = 1, bool fill = false) {
  Image output = blankImage(image.width + border*2, 
                            image.height + border*2, 
                            fill);

  for (int row = 0; row < image.height; row++) {
    for (int col = 0; col < image.width; col++) {
      output.pixels[row+border][col+border] = image.pixels[row][col];
    }
  }
  return output;
}

int getBinValue(Image const& image, int row, int col) {
  int value = 0;
  for (int r = row - 1; r <= row + 1; r++) {
    for(int c = col - 1; c <= col + 1; c++) {
      value *= 2;
      if (outOfBounds(r, c, image.width, image.height)) {
        value += 0;  
      } else if (image.pixels[r][c] == '#') {
        value += 1;
      }
    }
  }
  return value;
}

Image enhance(Image const& image, std::string const& enhance, bool fill = false) {
  Image output = blankImage(image.width, image.height, fill);

  int value;
  for (int row = 0; row < output.height; row++) {
    for (int col = 0; col < output.width; col++) {
      if (col == 0 || row == 0) {
        output.pixels[row][col] = fill ? '.' : '#';
        value = getBinValue(image, row, col);
        //output.pixels[row][col] = enhance[value];
      } else {
        value = getBinValue(image, row, col);
        output.pixels[row][col] = enhance[value];
      }
    }
  }

  return output;
}

void solve(Data const& data) {

  Image output_image = enhance(enlarge(data.image, 2), data.enhance);

  bool fill = false;
  int i = 1;
  while (i < 50) {
    fill = (i%2) == 1; 
    output_image = enhance(enlarge(output_image, 2, fill), data.enhance, fill);
    i++;
  }

  int count = 0;
  for (auto const& row : output_image.pixels) {
    for (auto const& pixel : row) {
      count += (pixel == '#');
    }
  }

  std::cout << count << " pixels are lit.\n";
}

void test() {
  std::vector<std::string> image = {
    "#..#.",
    "#....",
    "##..#",
    "..#..",
    "..###",
  };
  
  std::string enhance = 
    "..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..##"
    "#..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###"
    ".######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#."
    ".#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#....."
    ".#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.."
    "...####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#....."
    "..##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#";
  
  Data data = {enhance, image, 5, 5};

  solve(data);


}

void solution() {
  solve(getData());
}

int main() {
  //test(); NB have to set fill to false
  solution();
  return 0;
}
