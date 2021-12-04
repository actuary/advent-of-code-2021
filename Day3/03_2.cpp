#include <cstddef>
#include <iostream>
#include <fstream>

struct TreeNode {
  int count = 0;
  TreeNode* left = nullptr;;
  TreeNode* right = nullptr;
};

void consumeTree(TreeNode* tree, int& result, bool (*comp) (int, int)) {
  if (tree->left == nullptr && tree->right == nullptr)
    return;

  if (tree->left == nullptr) {
    consumeTree(tree->right, (result *= 2), comp); 
  } else if (tree->right == nullptr) {
    consumeTree(tree->left, (result *= 2) += 1, comp);
  } else if (comp(tree->left->count, tree->right->count))  {
    consumeTree(tree->left, (result *= 2) += 1, comp); 
  } else {
    consumeTree(tree->right, (result *= 2), comp); 
  }
}

void freeTree(TreeNode* tree) {
  if (tree->left == nullptr && tree->right == nullptr)
    return;

  if (tree->left != nullptr) {
    freeTree(tree->left);
    delete tree->left;
  }
  if (tree->right != nullptr) {
    freeTree(tree->right);
    delete tree->right;
  }
}

int main() {

  TreeNode *top = new TreeNode;
  TreeNode *curr = nullptr;
 
  std::ifstream in("03_1.data");
  std::string bit_string; 
  int const NUM_BITS = 12;
  while ((in >> bit_string)) {
    curr = top;
    for (int i = 0; i < NUM_BITS; i++) {
      if (bit_string[i] == '1') {
        if (!curr->left) 
          curr->left = new TreeNode;
        curr = curr->left;
        
      } else {
        if (!curr->right) 
          curr->right = new TreeNode;
        curr = curr->right;
      }
      curr->count++;
    }
  }

  int oxygen = 0, co2_scrubber = 0;
  consumeTree(top, oxygen, [](int l, int r) { return l >= r;});
  consumeTree(top, co2_scrubber, [](int l, int r) { return l < r;});
  std::cout << "Oxygen: " << oxygen << "\n";
  std::cout << "CO2 Scrubber: " <<  co2_scrubber << "\n";
  std::cout << "Life support rating: " << (co2_scrubber * oxygen) << "\n";

  in.close();

  freeTree(top);
  delete top;

  return 0;
}
