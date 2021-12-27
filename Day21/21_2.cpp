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

struct Player {
  long long position = 0, score = 0;
  bool operator==(const Player &other) const { 
    return (position == other.position
            && score == other.score);
  }
};

struct QuantumState {
  Player player1, player2;
  long long times = 0;
  bool turn = false;

  bool operator==(const QuantumState &other) const { 
    return (player1 == other.player1
            && player2 == other.player2
            && turn == other.turn);
  }
};

std::unordered_map<long long, long long> getPossibleRolls() {
  std::unordered_map<long long, long long> possible_rolls;

  for (long long i = 1; i < 4; i++) {
    for (long long j = 1; j < 4; j++) {
      for (long long k = 1; k < 4; k++) {
        if (possible_rolls.find(i + j + k) == possible_rolls.end())
          possible_rolls.insert({i + j + k, 0});
        possible_rolls[i + j + k]++;
      }
    }
  }
  return possible_rolls;
}

void solve(QuantumState starting) {
  std::pair<long long, long long> wins = {0, 0};
  auto rolls = getPossibleRolls();
  std::vector<QuantumState> states;
  states.push_back(starting);

  while (!states.empty()) {

    QuantumState current = states.front();
    states.erase(states.begin());
    Player current_player = current.turn ? current.player2 : current.player1;

    for (auto const& [roll, times] : rolls) { //clang-tidy NOLINT
      Player new_player;
      new_player.position = (current_player.position + roll - 1) % 10 + 1;
      new_player.score = current_player.score +  new_player.position;

      if (new_player.score >= 21) {
        if (current.turn) 
          wins.second += times * current.times;
        else
          wins.first += times * current.times;
      } else {

        QuantumState next = current;
        next.turn = !current.turn;
        next.times = times * current.times;
        if (current.turn) {
          next.player2 = new_player;
        } else {
          next.player1 = new_player;
        }

        auto it = std::find(states.begin(), states.end(), next);
        if (it != states.end()) {
          it->times += next.times;
        } else {
          states.push_back(next);
        }
      }
    }
  }

  std::cout << std::max(wins.first, wins.second) << "\n";
}

void test() {
  QuantumState initial {
    .player1 = {4, 0},
    .player2 = {8, 0},
    .times = 1,
    .turn = false
  };

  solve(initial);
}

void solution() {
  QuantumState initial {
    .player1 = {8, 0},
    .player2 = {7, 0},
    .times = 1,
    .turn = false
  };
  solve(initial);
}

int main() {
  test();
  solution();
  return 0;
}
