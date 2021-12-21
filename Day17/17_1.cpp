#include <iostream>
#include <fstream>
#include <assert.h>
#include <bits/stdc++.h>
#include <cmath>

struct Velocity {
  int dx, dy;
  Velocity(int x, int y) : dx(x), dy(y) {}
};

struct Point {
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
};

int T(int n, int after = 0) {
  assert(after >= 0 && n >= after);

  if (n == 0)
    return 0;

  if (after == 0)
    return (n * (n + 1)) /2;

  return (n * (n + 1)) / 2 - T(after, 0); 
}

struct Target {
  Point pmin, pmax;
  Target(Point const& pmin, Point const& pmax) : pmin(pmin), pmax(pmax) {}

  bool isInside(Point const& p) const {
    return p.x >= pmin.x && p.x <= pmax.x && p.y >= pmin.y && p.y <= pmax.y;
  }

  bool isAbove(Point const& p) const {
    return p.x >= pmin.x && p.x <= pmax.x && p.y >= pmin.y;
  }
};
int signum(int n) {
  return n == 0 ? 0 : n > 0 ? 1 : -1;
}

struct Probe {
  Point position;
  Velocity velocity;
  
  Probe(Point position, Velocity velocity) : 
    position(position), velocity(velocity) {}

  void step() {
    position.x += velocity.dx;
    position.y += velocity.dy;

    
    velocity.dx -= signum(velocity.dx);
    velocity.dy -= 1;
  }

  void step(int n) {
    for (int i = 0; i < n; i++)
      step();
  }

  bool hitsTarget(Target const& t) {
    while (velocity.dx != 0 || position.y >= t.pmin.y) {
      if (t.isInside(position)) 
        return true;
      step();
    }

    return false;
  }

  int highestPosition() {
    if (velocity.dy <= 0)
      return position.y;

    int max_y = position.y;
    while (velocity.dy != 0) {
      step();
      if (position.y > max_y)
        max_y = position.y;
    }

    return max_y;
  }
};


void solve(Target const& t) {
  //tired today, this is bad and i don't care
  int width = t.pmax.x - t.pmin.x, height = t.pmax.y - t.pmin.y;
  int max_dx = 1;
  int max_dy = t.pmin.y;
  int count = 0;
  for(int dx = 1; dx <= t.pmax.x; dx++) {
    for (int dy = t.pmin.y; dy < 10000; dy++) {
      if (Probe({0, 0}, {dx, dy}).hitsTarget(t)) {
        count++;
        if (max_dy < dy) {
          max_dy = dy;
          max_dx = dx;
        }
      }
    }
  }
  std::cout << "(" << max_dx << ", " << max_dy << ")\n";
  std::cout << Probe({0, 0}, {max_dx, max_dy}).highestPosition() << "\n";
  std::cout << count << "\n";
}

void solution() {
  Target t(Point(96, -144), Point(125, -98));
  solve(t);
}

void test() {
  Target t(Point(20, -10), Point(30, -5));

  assert(t.isInside({30,-6}));
  assert(!t.isInside({6,2}));

  assert(Probe({0, 0}, {7, 2}).hitsTarget(t));
  assert(Probe({0, 0}, {6, 3}).hitsTarget(t));
  assert(!Probe({0, 0}, {17, -4}).hitsTarget(t));
  assert(Probe({0, 0}, {6, 9}).hitsTarget(t));
  assert(Probe({0, 0}, {6, 9}).highestPosition() == 45);
  solve(t); 
}
//target area: x=96..125, y=-144..-98
int main() {
  test();
  solution();
  return 0;
}
