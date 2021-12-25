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

struct Number;
using NumberPtr = std::shared_ptr<Number>;

enum class Type {REGULAR, PAIR};
enum class Side {ONLY, LEFT, RIGHT};

struct Number : std::enable_shared_from_this<Number> {
  NumberPtr parent = nullptr;
  Type const type;
  Side side;
  int level = 0; //how many pairs its inside

  Number(NumberPtr parent, Type type, Side side) : 
    parent(parent), type(type), side(side) {}

  virtual void setLevel(int l) = 0;
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
  void setValue(int v) { value = v; }
  void setLevel(int l) { level = l; }

  void print() {
    std::cout << value;
  }
};

struct Pair : public Number {
  NumberPtr first;
  NumberPtr second;

  Pair(NumberPtr parent, Side side, NumberPtr first, NumberPtr second) :
    Number(parent, Type::PAIR, side), first(first), second(second) {
      first->side = Side::LEFT;
      second->side = Side::RIGHT;
    }

  void setParents() {
    first->parent = shared_from_this(); 
    second->parent = shared_from_this(); 
    setLevel(level);
  }

  void setLevel(int l) {
    level = l;
    first->setLevel(level+1);
    second->setLevel(level+1);
  }

  int getValue() {
    return 3*first->getValue() + 2*second->getValue();
  }
  void setValue(int value) {}

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
      parent->first->setValue(value + parent->first->getValue());
    } else {
      parent = std::dynamic_pointer_cast<Pair>(parent->first);
      while (parent->second->type != Type::REGULAR) {
        parent = std::dynamic_pointer_cast<Pair>(parent->second);
      }
      parent->second->setValue(value + parent->second->getValue());
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
      parent->second->setValue(value + parent->second->getValue());
    } else {
      parent = std::dynamic_pointer_cast<Pair>(parent->second);
      while (parent->first->type != Type::REGULAR) {
        parent = std::dynamic_pointer_cast<Pair>(parent->first);
      }
      parent->first->setValue(value + parent->first->getValue());
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
  
  setLeftNumber(element, first);
  setRightNumber(element, second);

  element = std::make_shared<Regular>(element->parent, element->side, 0);
  element->level--;
  if (element->parent != nullptr) {
    PairPtr parent = std::dynamic_pointer_cast<Pair>(element->parent);
    if (element->side == Side::LEFT) 
      parent->first = element;
    else if (element->side == Side::RIGHT)
      parent->second = element;

    parent->setParents();
  }
}

bool findAndExplode(NumberPtr& head) {
  if (head->type == Type::REGULAR)
    return false;

  PairPtr pair = std::dynamic_pointer_cast<Pair>(head);

  if (pair->level == 4) {
    explode(head);
    return true;
  } else {
    bool first = findAndExplode(pair->first);
    
    if (first)
      return true;
    else
      return findAndExplode(pair->second);
  }
  return false;
}

void split(NumberPtr& element) {
  assert(element->type == Type::REGULAR);
 
  NumberPtr first = std::make_shared<Regular>(element->parent, 
                                              Side::LEFT, element->getValue()/2);
  NumberPtr second = std::make_shared<Regular>(element->parent, 
                                              Side::RIGHT, (element->getValue()+1)/2);

  element = std::make_shared<Pair>(element->parent, element->side, first, second);
  PairPtr pair = std::dynamic_pointer_cast<Pair>(element);
  pair->setParents();
   
  if (element->parent != nullptr) {
    PairPtr parent = std::dynamic_pointer_cast<Pair>(element->parent);
    if (element->side == Side::LEFT) 
      parent->first = element;
    else if (element->side == Side::RIGHT)
      parent->second = element;

    parent->setParents();
  }
}

bool findAndSplit(NumberPtr& head) {
  assert(head != nullptr);
  if (head->type == Type::REGULAR) {
    if (head->getValue() >= 10) {
      split(head);
      return true;
    }
    return false;
  }

  PairPtr pair = std::dynamic_pointer_cast<Pair>(head);
  
  bool first = findAndSplit(pair->first);
  if (first)
    return true;
  else
    return findAndSplit(pair->second);
}

void reduce(NumberPtr& head) {
  bool reducing = true;
  bool exploded = false, splitted = false; 

  while (reducing) {
    splitted = false;
    exploded = findAndExplode(head);

    if (!exploded) 
      splitted = findAndSplit(head);
    
    reducing = exploded || splitted; 
  }
}
int findCommaPos(std::string const& expr) {
  int bracket_count = 0;

  for (int idx = 1; idx < expr.length(); idx++) {
    if (expr[idx] == '[')
      bracket_count++;
    else if (expr[idx] == ']')
      bracket_count--;
    else if (expr[idx] == ',' && bracket_count == 0)
      return idx;
  }
  assert(0); //shouldn't happen
}

NumberPtr parseNumberPtr(std::string const& expr) {
  if (std::isdigit(expr[0])) {
    return std::make_shared<Regular>(nullptr, Side::ONLY, stoi(expr));
  }

  int comma_pos = findCommaPos(expr);
  
  auto first = parseNumberPtr(expr.substr(1, comma_pos-1));
  auto second = parseNumberPtr(expr.substr(comma_pos+1, expr.length() - comma_pos -1));
  auto pair = std::make_shared<Pair>(nullptr, Side::ONLY, first, second);
  pair->setParents();

  return pair;
}

void printData(std::vector<NumberPtr> const& data) {
  for (auto const& ptr: data) {
    if (ptr != nullptr)
      ptr->print();
    std::cout << "\n";
  }
}

std::vector<NumberPtr> getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "File failure.\n";
    return {};
  }
    
  std::vector<NumberPtr> result;
  std::string line; 
  while (std::getline(in, line)) {
    result.push_back(parseNumberPtr(line));
  }

  return result;
}

NumberPtr add(NumberPtr left, NumberPtr right) {
  NumberPtr result = std::make_shared<Pair>(nullptr, Side::ONLY, 
      left, right);
  auto pair = std::dynamic_pointer_cast<Pair>(result);
  pair->setParents();
  return result;
}

void test_explode1() {
  //[[[[[9,8],1],2],3],4] 
  auto r1 = std::make_shared<Regular>(nullptr, Side::ONLY, 9);
  auto r2 = std::make_shared<Regular>(nullptr, Side::ONLY, 8);
  auto r3 = std::make_shared<Regular>(nullptr, Side::ONLY, 1);
  auto r4 = std::make_shared<Regular>(nullptr, Side::ONLY, 2);
  auto r5 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r6 = std::make_shared<Regular>(nullptr, Side::ONLY, 4);

  auto p1 = std::make_shared<Pair>(nullptr, Side::ONLY, r1, r2);
  p1->setParents();
  auto p3 = std::make_shared<Pair>(nullptr, Side::ONLY, p1, r3);
  p3->setParents();
  auto p4 = std::make_shared<Pair>(nullptr, Side::ONLY, p3, r4);
  p4->setParents();
  auto p5 = std::make_shared<Pair>(nullptr, Side::ONLY, p4, r5);
  p5->setParents();
  auto p6 = std::make_shared<Pair>(nullptr, Side::ONLY, p5, r6);
  p6->setParents();

  p6->print();
  std::cout << "\n";
  NumberPtr n = p6;
  assert(findAndExplode(n));
  p6->print();
  std::cout << "\n";

}

void test_explode2() {
  //[7,[6,[5,[4,[3,2]]]]]
  auto r1 = std::make_shared<Regular>(nullptr, Side::ONLY, 7);
  auto r2 = std::make_shared<Regular>(nullptr, Side::ONLY, 6);
  auto r3 = std::make_shared<Regular>(nullptr, Side::ONLY, 5);
  auto r4 = std::make_shared<Regular>(nullptr, Side::ONLY, 4);
  auto r5 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r6 = std::make_shared<Regular>(nullptr, Side::ONLY, 2);

  auto p1 = std::make_shared<Pair>(nullptr, Side::ONLY, r5, r6);
  p1->setParents();
  auto p3 = std::make_shared<Pair>(nullptr, Side::ONLY, r4, p1);
  p3->setParents();
  auto p4 = std::make_shared<Pair>(nullptr, Side::ONLY, r3, p3);
  p4->setParents();
  auto p5 = std::make_shared<Pair>(nullptr, Side::ONLY, r2, p4);
  p5->setParents();
  auto p6 = std::make_shared<Pair>(nullptr, Side::ONLY, r1, p5);
  p6->setParents();

  p6->print();
  std::cout << "\n";
  NumberPtr n = p6;
  assert(findAndExplode(n));
  p6->print();
  std::cout << "\n";

};

void test_explode3() {
  //[[6,[5,[4,[3,2]]]],1]
  auto r1 = std::make_shared<Regular>(nullptr, Side::ONLY, 6);
  auto r2 = std::make_shared<Regular>(nullptr, Side::ONLY, 5);
  auto r3 = std::make_shared<Regular>(nullptr, Side::ONLY, 4);
  auto r4 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r5 = std::make_shared<Regular>(nullptr, Side::ONLY, 2);
  auto r6 = std::make_shared<Regular>(nullptr, Side::ONLY, 1);

  auto p1 = std::make_shared<Pair>(nullptr, Side::ONLY, r4, r5);
  p1->setParents();
  auto p3 = std::make_shared<Pair>(nullptr, Side::ONLY, r3, p1);
  p3->setParents();
  auto p4 = std::make_shared<Pair>(nullptr, Side::ONLY, r2, p3);
  p4->setParents();
  auto p5 = std::make_shared<Pair>(nullptr, Side::ONLY, r1, p4);
  p5->setParents();
  auto p6 = std::make_shared<Pair>(nullptr, Side::ONLY, p5, r6);
  p6->setParents();

  p6->print();
  std::cout << "\n";
  NumberPtr n = p6;
  assert(findAndExplode(n));
  p6->print();
  std::cout << "\n";
}

void test_explode4() {
  //[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]
  auto r1 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r2 = std::make_shared<Regular>(nullptr, Side::ONLY, 2);
  auto r3 = std::make_shared<Regular>(nullptr, Side::ONLY, 1);
  auto r4 = std::make_shared<Regular>(nullptr, Side::ONLY, 7);
  auto r5 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r6 = std::make_shared<Regular>(nullptr, Side::ONLY, 6);
  auto r7 = std::make_shared<Regular>(nullptr, Side::ONLY, 5);
  auto r8 = std::make_shared<Regular>(nullptr, Side::ONLY, 4);
  auto r9 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r10 = std::make_shared<Regular>(nullptr, Side::ONLY, 2);

  auto p1 = std::make_shared<Pair>(nullptr, Side::ONLY, r4, r5);
  p1->setParents();
  auto p3 = std::make_shared<Pair>(nullptr, Side::ONLY, r3, p1);
  p3->setParents();
  auto p4 = std::make_shared<Pair>(nullptr, Side::ONLY, r2, p3);
  p4->setParents();
  auto p5 = std::make_shared<Pair>(nullptr, Side::ONLY, r1, p4);
  p5->setParents();
  auto p6 = std::make_shared<Pair>(nullptr, Side::ONLY, r9, r10);
  p6->setParents();
  auto p7 = std::make_shared<Pair>(nullptr, Side::ONLY, r8, p6);
  p7->setParents();
  auto p8 = std::make_shared<Pair>(nullptr, Side::ONLY, r7, p7);
  p8->setParents();
  auto p9 = std::make_shared<Pair>(nullptr, Side::ONLY, r6, p8);
  p9->setParents();
  auto p10 = std::make_shared<Pair>(nullptr, Side::ONLY, p5, p9);
  p10->setParents();

  p10->print();
  std::cout << "\n";
  NumberPtr n = p10;
  assert(findAndExplode(n));
  p10->print();
  std::cout << "\n";
  assert(findAndExplode(n));
  p10->print();
  std::cout << "\n";
}

void test_explode() {
  test_explode1();
  test_explode2();
  test_explode3();
  test_explode4();
}

void test_split() {
  //[[[[[9,8],11],2],3],4] 
  auto r1 = std::make_shared<Regular>(nullptr, Side::ONLY, 9);
  auto r2 = std::make_shared<Regular>(nullptr, Side::ONLY, 8);
  auto r3 = std::make_shared<Regular>(nullptr, Side::ONLY, 11);
  auto r4 = std::make_shared<Regular>(nullptr, Side::ONLY, 2);
  auto r5 = std::make_shared<Regular>(nullptr, Side::ONLY, 3);
  auto r6 = std::make_shared<Regular>(nullptr, Side::ONLY, 4);

  auto p1 = std::make_shared<Pair>(nullptr, Side::ONLY, r1, r2);
  p1->setParents();
  auto p3 = std::make_shared<Pair>(nullptr, Side::ONLY, p1, r3);
  p3->setParents();
  auto p4 = std::make_shared<Pair>(nullptr, Side::ONLY, p3, r4);
  p4->setParents();
  auto p5 = std::make_shared<Pair>(nullptr, Side::ONLY, p4, r5);
  p5->setParents();
  auto p6 = std::make_shared<Pair>(nullptr, Side::ONLY, p5, r6);
  p6->setParents();

  p6->print();
  std::cout << "\n";
  NumberPtr n = p6;
  findAndSplit(n);
  p6->print();
  std::cout << "\n";
}

void solve(std::vector<NumberPtr> data) {
  NumberPtr accum = data[0];
  for (int idx = 1; idx < data.size(); idx++) {
    accum = add(accum, data[idx]);
    reduce(accum);
  }

  accum->print();
  std::cout << "\n";
  std::cout << accum->getValue() << "\n";
}

void test() {
  std::vector<NumberPtr> data = {};
  data.push_back(parseNumberPtr("[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]"));
  data.push_back(parseNumberPtr("[[[5,[2,8]],4],[5,[[9,9],0]]]"));
  data.push_back(parseNumberPtr("[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]"));
  data.push_back(parseNumberPtr("[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]"));
  data.push_back(parseNumberPtr("[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]"));
  data.push_back(parseNumberPtr("[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]"));
  data.push_back(parseNumberPtr("[[[[5,4],[7,7]],8],[[8,3],8]]"));
  data.push_back(parseNumberPtr("[[9,3],[[9,9],[6,[4,9]]]]"));
  data.push_back(parseNumberPtr("[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]"));
  data.push_back(parseNumberPtr("[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]"));
  //printData(data);
  std::cout << "Solving---------\n";
  solve(data);
}

void test1() {
  std::vector<NumberPtr> data = {};
  data.push_back(parseNumberPtr("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]"));
  data.push_back(parseNumberPtr("[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]"));
  data.push_back(parseNumberPtr("[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]"));
  data.push_back(parseNumberPtr("[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]"));
  data.push_back(parseNumberPtr("[7,[5,[[3,8],[1,4]]]]"));
  data.push_back(parseNumberPtr("[[2,[2,2]],[8,[8,1]]]"));
  data.push_back(parseNumberPtr("[2,9]"));
  data.push_back(parseNumberPtr("[1,[[[9,3],9],[[9,0],[0,7]]]]"));
  data.push_back(parseNumberPtr("[[[5,[7,4]],7],1]"));
  data.push_back(parseNumberPtr("[[[[4,2],2],6],[8,7]]"));
  //printData(data);
  std::cout << "Solving---------\n";
  solve(data);
}

void solution() {
  auto data = getData();
  solve(data);
}

int main() {
  //test_explode();
  //test_split();
  //test();
  solution();
  return 0;
}
