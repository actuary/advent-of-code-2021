#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <assert.h>
#include <tuple>
#include <vector>
#include <algorithm>

int readNum(std::string const& data, int& pos) {
  assert(std::isdigit(data[pos]));

  int result = 0;
  while (std::isdigit(data[pos])) {
    result *= 10;
    result += data[pos++] - '0';
  }
  pos--;
  return result;
}

namespace Snailfish {

struct Number;
using NumberPtr = std::shared_ptr<Number>;

enum class Type {REGULAR, PAIR};
enum class Side {ONLY, LEFT, RIGHT};

struct Number {
  NumberPtr parent = nullptr;
  Type const type;
  Side side;

  Number(NumberPtr parent, Type type, Side side) : 
    parent(parent), type(type), side(side) {}

  virtual int getValue() = 0;
  virtual void setValue(int value) = 0;
  virtual void print() = 0;
  virtual ~Number() {}
};

struct Regular : public Number {
  Side side = Side::ONLY;
  int value;

  Regular(NumberPtr parent, Side side, int value) :
    Number(parent, Type::REGULAR, side), value(value) {}
    
  int getValue() {return value; }
  void setValue(int value) { value = value; }

  void print() {
    std::cout << value;
  }
};

struct Pair : public Number {
  NumberPtr first;
  NumberPtr second;

  Pair(NumberPtr parent, Side side, NumberPtr first, NumberPtr second) :
    Number(parent, Type::PAIR, side), first(first), second(second) {}

  int getValue() { return -1; }
  void setValue(int value) { value = value; }

  void print() {
    std::cout << '[';
    first->print();
    std::cout << ',';
    second->print();
    std::cout << ']';
  }
};

using RegularPtr = std::shared_ptr<Regular>;
using PairPtr = std::shared_ptr<Pair>;

void setLeftNumber(NumberPtr& element, int value) {
  if (element == nullptr) 
    return;

  PairPtr parent = std::dynamic_pointer_cast<Pair>(element->parent);
  if (element->side == Side::RIGHT) {
    if (parent->first->type == Type::REGULAR) {
      parent->first->setValue(value);
    } else {
      parent = std::dynamic_pointer_cast<Pair>(parent->first);
      while (parent->second->type != Type::REGULAR) {
        parent = std::dynamic_pointer_cast<Pair>(parent->second);
      }
      parent->second->setValue(value);
    }
  } else {
    setLeftNumber(element->parent, value);
  }
}

void setRightNumber(NumberPtr& element, int value) {
  if (element == nullptr) 
    return;

  PairPtr parent = std::dynamic_pointer_cast<Pair>(element->parent);
  if (element->side == Side::LEFT) {
    if (parent->second->type == Type::REGULAR) {
      parent->second->setValue(value);
    } else {
      parent = std::dynamic_pointer_cast<Pair>(parent->second);
      while (parent->first->type != Type::REGULAR) {
        parent = std::dynamic_pointer_cast<Pair>(parent->first);
      }
      parent->first->setValue(value);
    }
  } else {
    setRightNumber(element->parent, value);
  }
}

void explode(NumberPtr& element) {
  assert(element->type == Type::PAIR);

  PairPtr pair = std::dynamic_pointer_cast<Pair>(element);
  assert(pair->first->type == Type::REGULAR);
  assert(pair->second->type == Type::REGULAR);
  
  int first = pair->first->getValue();
  int second = pair->second->getValue();

  element = std::make_shared<Regular>(element->parent, element->side, 0);

  setLeftNumber(element, first);
  setRightNumber(element, second);
}

}
void test() {
  //[[[[[9,8],1],2],3],4] 
  auto r1 = std::make_shared<Snailfish::Regular>(nullptr, Snailfish::Side::RIGHT, 9);
  auto r2 = std::make_shared<Snailfish::Regular>(nullptr, Snailfish::Side::RIGHT, 8);
  auto r3 = std::make_shared<Snailfish::Regular>(nullptr, Snailfish::Side::RIGHT, 1);
  auto r4 = std::make_shared<Snailfish::Regular>(nullptr, Snailfish::Side::RIGHT, 2);
  auto r5 = std::make_shared<Snailfish::Regular>(nullptr, Snailfish::Side::RIGHT, 3);
  auto r6 = std::make_shared<Snailfish::Regular>(nullptr, Snailfish::Side::RIGHT, 4);
}

int main() {
  return 0;
}
