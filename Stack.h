#pragma once

#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d);
    };
    Node* topNode;
public:
    Stack();
    ~Stack();
    void push(const T& data);
    bool pop(T& data);
    bool isEmpty() const;
};

template <typename T>
Stack<T>::Node::Node(const T& d) : data(d), next(nullptr) {}

template <typename T>
Stack<T>::Stack() : topNode(nullptr) {}

template <typename T>
Stack<T>::~Stack() {
    while (topNode) {
        Node* toDelete = topNode;
        topNode = topNode->next;
        delete toDelete;
    }
}

template <typename T>
void Stack<T>::push(const T& data) {
    Node* node = new Node(data);
    node->next = topNode;
    topNode = node;
}

template <typename T>
bool Stack<T>::pop(T& data) {
    if (!topNode) return false;
    data = topNode->data;
    Node* toDelete = topNode;
    topNode = topNode->next;
    delete toDelete;
    return true;
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return !topNode;
}