#include <iostream>
#include "btree.h"

int main() {
    BTree<int, 3> tree;
    
    std::cout << "Testing B-Tree insertion and traversal:\n";
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);
    tree.insert(30);
    tree.insert(7);
    tree.insert(17);
    
    tree.display();
    
    std::cout << "\nSearching for key 7: ";
    if (tree.search(7) != nullptr) {
        std::cout << "Found!\n";
    } else {
        std::cout << "Not found!\n";
    }
    
    std::cout << "Searching for key 15: ";
    if (tree.search(15) != nullptr) {
        std::cout << "Found!\n";
    } else {
        std::cout << "Not found!\n";
    }
    
    return 0;
}