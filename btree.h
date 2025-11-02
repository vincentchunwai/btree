#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>

template<typename T, int ORDER>
class BTreeNode {
public:
    bool isLeaf;
    std::vector<T> keys;
    std::vector<BTreeNode*> children;
    int numKeys;

    BTreeNode(bool leaf);
    ~BTreeNode();
    
    // Helper function to traverse and print the tree
    void traverse();
    
    // Helper function to search for a key
    BTreeNode* search(T key);
    
    // Helper function to insert when node is not full
    void insertNonFull(T key);
    
    // Helper function to split a full child
    void splitChild(int index);
    
    // Helper function to check if the node is full
    bool isFull();

    // Helper function to check if the node has enough key
    bool needResizeIfDelete();
};


template<typename T, int ORDER>
class BTree {
private:
    BTreeNode<T, ORDER>* root;
    int minDegree; 

public:
    BTree();
    ~BTree();
    
    void insert(T key);
    void remove(T key);
    BTreeNode<T, ORDER>* search(T key);
    
    void traverse();
    void display();
    
    bool isEmpty();
    void clear();
};

#endif // BTREE_H
