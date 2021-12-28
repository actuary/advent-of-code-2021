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

void getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  in.close();
}


void solve() {
}

void test() {
}

//#############
//#...........#
//###C#B#A#D###
//  #C#D#A#B#
//  #########
//
// Hand solved. Got lucky as mine was easy since A/C is 
// independent from B/D.
//
// First Moves for A:
//   The optimal moves to go in would be 14, if the Cs weren't
//   occupying their room, small detour adds to 4xcost(A) (see below)
//   Obv better to detour A as they cost less
//
//#############
//#.A.....A...#
//###C#B#.#D###
//  #C#D#.#B#
//  #########
//
// First Moves for C:
//   Clearly optimal as Cs go straight in, leading to 14xcost(C)
//#############
//#.A.....A...#
//###.#B#C#D###
//  #.#D#C#B#
//  #########
//
// Next Moves for A:
//   move them directly inot their room for another 14xcost(A),
//   total is now 14 x cost(C) + 18 x cost(A)
//#############
//#...........#
//###A#B#C#D###
//  #A#D#C#B#
//  #########
//
// First Moves for B/D:
//   Optimal cost (if we relaxed some rules), would be 8 x cost(B) + 8 x cost(D)
//   D HAS to get out of the way for B in room 4, so that adds a cost of 4 x cost(D)
//   B must stay out of Ds way as they're so expensive, cost of 5 x cost(B)

//#############
//#...B.D.D.B.#
//###A#.#C#.###
//  #A#.#C#.#
//  #########
//
// Next moves for D:
//   move straight into their room, for an addition cost of 8 x cost(D).
//   total is now 14 x cost(C) + 18 x cost(A) + 12 x cost(D)
//  
//#############
//#...B.....B.#
//###A#.#C#D###
//  #A#.#C#D#
//  #########
// Next moves for B:
//   move straight into their room, for an addition cost of 9 x cost(B).
//   total is now 14 x cost(C) + 18 x cost(A) + 12 x cost(D) + 14 x cost(B)
//  
//#############
//#...B.D.D.B.#
//###A#.#C#.###
//  #A#.#C#.#
//  #########
//
void solution() {
  //hand solved 
  std::cout << 14 * 100 + 18 * 1 + 12 * 1000 + 14 * 10 << "\n";
}

int main() {
  solution();
  return 0;
}
