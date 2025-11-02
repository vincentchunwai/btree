#include "btree.h"

template<typename T, int ORDER>
BTreeNode<T, ORDER>::BTreeNode(bool leaf) : isLeaf(leaf), numKeys(0) {
    keys.reserve(ORDER - 1);
    children.reserve(ORDER);
}

template<typename T, int ORDER>
BTreeNode<T, ORDER>::~BTreeNode() {
    if (!isLeaf) {
        for (int i = 0; i < numKeys + 1; i++) {
            delete children[i];
        }
    }
}

template<typename T, int ORDER>
void BTreeNode<T, ORDER>::traverse() {
    int i;
    for (i = 0; i < numKeys; i++) {
        if (!isLeaf) {
            children[i]->traverse();
        }
        std::cout << keys[i] << " ";
    }
    if (!isLeaf) {
        children[i]->traverse();
    }
}

template<typename T, int ORDER>
BTreeNode<T, ORDER>* BTreeNode<T, ORDER>::search(T key) {
    int i = 0;
    while (i < numKeys && key > keys[i]) {
        i++;
    }
    if (i < numKeys && keys[i] == key) {
        return this;
    }
    if (isLeaf) {
        return nullptr;
    }
    return children[i]->search(key);
}

template<typename T, int ORDER>
void BTreeNode<T, ORDER>::insertNonFull(T key) {
    int i = numKeys - 1;

    if (isLeaf) {
        while (i >= 0 && key < keys[i]) {
            i--;
        }
        
        keys.insert(keys.begin() + i + 1, key);
        numKeys++;
    
    } else {
        while (i >= 0 && key < keys[i]) {
            i--;
        }
        i++;

        if (children[i]->isFull()) {
            splitChild(i);
            if (key > keys[i]) {
                i++;
            }
        }

        children[i]->insertNonFull(key);
    }

}

template<typename T, int ORDER>
void BTreeNode<T, ORDER>::splitChild(int index) {
    BTreeNode<T, ORDER>* fullChild = children[index];
    
    BTreeNode<T, ORDER>* newNode = new BTreeNode<T, ORDER>(fullChild->isLeaf);

    for (int i = 0; i < ORDER / 2 - 1; i++) {
        newNode->keys.push_back(fullChild->keys[i + ORDER / 2 + 1]);
    }
    newNode->numKeys = ORDER / 2 - 1;

    if (!fullChild->isLeaf) {
        for (int i = 0; i < ORDER / 2; i++) {
            newNode->children.push_back(fullChild->children[i + ORDER / 2 + 1]);
        }
    }

    T middleKey = fullChild->keys[ORDER / 2];
    
    fullChild->numKeys = ORDER / 2;
    fullChild->keys.erase(fullChild->keys.begin() + ORDER / 2, fullChild->keys.end());
    if (!fullChild->isLeaf) {
        fullChild->children.erase(fullChild->children.begin() + ORDER / 2 + 1, fullChild->children.end());
    }
    
    keys.insert(keys.begin() + index, middleKey);
    children.insert(children.begin() + index + 1, newNode);
    numKeys++;
}

template<typename T, int ORDER>
bool BTreeNode<T, ORDER>::isFull() {
    return numKeys == ORDER - 1;
}

template<typename T, int ORDER>
bool BTreeNode<T, ORDER>::needResizeIfDelete() {
    return numKeys < ORDER / 2;
}

// BTree implementation
template<typename T, int ORDER>
BTree<T, ORDER>::BTree() : root(nullptr), minDegree(ORDER / 2) {
}

template<typename T, int ORDER>
BTree<T, ORDER>::~BTree() {
    clear();
}

template<typename T, int ORDER>
void BTree<T, ORDER>::insert(T key) {
    if (root == nullptr) {
        root = new BTreeNode<T, ORDER>(true);
        root->keys.push_back(key);
        root->numKeys = 1;
        return;
    }

    if (root->isFull()) {
        BTreeNode<T, ORDER>* newRoot = new BTreeNode<T, ORDER>(false);
        newRoot->children.push_back(root);
        newRoot->numKeys = 0;
        
        newRoot->splitChild(0);
        
        int i = (key < newRoot->keys[0]) ? 0 : 1;
        newRoot->children[i]->insertNonFull(key);
        
        root = newRoot;
    } else {
        root->insertNonFull(key);
    }
}

template<typename T, int ORDER>
BTreeNode<T, ORDER>* BTree<T, ORDER>::search(T key) {
    return (root == nullptr) ? nullptr : root->search(key);
}

template<typename T, int ORDER>
void BTree<T, ORDER>::traverse() {
    if (root != nullptr) {
        root->traverse();
    }
}

template<typename T, int ORDER>
void BTree<T, ORDER>::remove(T key) {
    if (root == nullptr) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }
    
    BTreeNode<T, ORDER>* node = root->search(key);

    if (node == nullptr) {
        std::cout << "Key not found" << std::endl;
        return;
    }

    if (node->isLeaf) {
        if (!node->needResizeIfDelete()) {
            for (auto it = node->keys.begin(); it != node->keys.end(); ++it) {
                if (*it == key) {
                    node->keys.erase(it);
                    node->numKeys--;
                    break;
                }
            }
            return;
        } else {
            for (auto it = node->keys.begin(); it != node->keys.end(); ++it) {
                if (*it == key) {
                    node->keys.erase(it);
                    node->numKeys--;
                    break;
                }
            }
            
        }
        return;
    }
}

template<typename T, int ORDER>
bool BTree<T, ORDER>::isEmpty() {
    return root == nullptr;
}

template<typename T, int ORDER>
void BTree<T, ORDER>::clear() {
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}

template<typename T, int ORDER>
void BTree<T, ORDER>::display() {
    if (root == nullptr) {
        std::cout << "Tree is empty" << std::endl;
    } else {
        std::cout << "B-Tree contents (in-order): ";
        traverse();
        std::cout << std::endl;
    }
}

// Explicit template instantiations
template class BTree<int, 3>;
