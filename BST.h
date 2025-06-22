#pragma once 
#include <iostream> 
using namespace std;

template <typename T, typename KeyType>
struct BSTNode {
    T data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const T& d) : data(d), left(nullptr), right(nullptr) {}
};

template <typename T, typename KeyType>
class BST {
private:
    BSTNode<T, KeyType>* root;
    KeyType(*getKey)(const T&);

    void insert(BSTNode<T, KeyType>*& node, const T& data);
    BSTNode<T, KeyType>* search(BSTNode<T, KeyType>* node, const KeyType& key) const;
    void inorder(BSTNode<T, KeyType>* node) const;
    void clear(BSTNode<T, KeyType>* node);
    void remove(BSTNode<T, KeyType>*& node, const T& data);

public:
    BST(KeyType(*getKeyFunc)(const T&));
    ~BST();
    void insert(const T& data);
    void remove(const T& data);
    T* search(const KeyType& key) const;
    void displayInOrder() const;
};


template <typename T, typename KeyType>
BST<T, KeyType>::BST(KeyType(*getKeyFunc)(const T&)) : root(nullptr), getKey(getKeyFunc) {}

template <typename T, typename KeyType>
BST<T, KeyType>::~BST() { clear(root); }

template <typename T, typename KeyType>
void BST<T, KeyType>::insert(const T& data) { insert(root, data); }

template <typename T, typename KeyType>
void BST<T, KeyType>::remove(const T& data) { remove(root, data); }

template <typename T, typename KeyType>
T* BST<T, KeyType>::search(const KeyType& key) const {
    BSTNode<T, KeyType>* node = search(root, key);
    return node ? &node->data : nullptr;
}

template <typename T, typename KeyType>
void BST<T, KeyType>::displayInOrder() const { inorder(root); }

template <typename T, typename KeyType>
void BST<T, KeyType>::insert(BSTNode<T, KeyType>*& node, const T& data) {
    if (!node) node = new BSTNode<T, KeyType>(data);
    else if (getKey(data) < getKey(node->data)) insert(node->left, data);
    else if (getKey(data) > getKey(node->data)) insert(node->right, data);
}

template <typename T, typename KeyType>
BSTNode<T, KeyType>* BST<T, KeyType>::search(BSTNode<T, KeyType>* node, const KeyType& key) const {
    if (!node) return nullptr;
    if (key == getKey(node->data)) return node;
    else if (key < getKey(node->data)) return search(node->left, key);
    else return search(node->right, key);
}

template <typename T, typename KeyType>
void BST<T, KeyType>::inorder(BSTNode<T, KeyType>* node) const {
    if (!node) return;
    inorder(node->left);
    cout << node->data << endl;
    inorder(node->right);
}

template <typename T, typename KeyType>
void BST<T, KeyType>::clear(BSTNode<T, KeyType>* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T, typename KeyType>
void BST<T, KeyType>::remove(BSTNode<T, KeyType>*& node, const T& data) {
    if (!node) return;
    if (getKey(data) < getKey(node->data)) {
        remove(node->left, data);
    }
    else if (getKey(data) > getKey(node->data)) {
        remove(node->right, data);
    }
    else {
        if (!node->left) {
            BSTNode<T, KeyType>* temp = node;
            node = node->right;
            delete temp;
        }
        else if (!node->right) {
            BSTNode<T, KeyType>* temp = node;
            node = node->left;
            delete temp;
        }
        else {
            BSTNode<T, KeyType>* temp = node->right;
            while (temp->left) temp = temp->left;
            node->data = temp->data;
            remove(node->right, temp->data);
        }
    }
}