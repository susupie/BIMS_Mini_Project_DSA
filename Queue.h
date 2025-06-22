#pragma once

#include <iostream>
using namespace std;

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d);
    };
    Node* frontNode;
    Node* rearNode;
public:
    Queue();
    Queue(const Queue& other);
    ~Queue();
    void enqueue(const T& data);
    bool dequeue(T& data);
    bool isEmpty() const;
};

template <typename T>
Queue<T>::Node::Node(const T& d) : data(d), next(nullptr) {}

template <typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr) {}

template <typename T>
Queue<T>::Queue(const Queue& other) : frontNode(nullptr), rearNode(nullptr) {
    Node* curr = other.frontNode;
    while (curr) {
        enqueue(curr->data);
        curr = curr->next;
    }
}

template <typename T>
Queue<T>::~Queue() {
    while (frontNode) {
        Node* toDelete = frontNode;
        frontNode = frontNode->next;
        delete toDelete;
    }
}

template <typename T>
void Queue<T>::enqueue(const T& data) {
    Node* node = new Node(data);
    if (rearNode) rearNode->next = node;
    else frontNode = node;
    rearNode = node;
}

template <typename T>
bool Queue<T>::dequeue(T& data) {
    if (!frontNode) return false;
    data = frontNode->data;
    Node* toDelete = frontNode;
    frontNode = frontNode->next;
    if (!frontNode) rearNode = nullptr;
    delete toDelete;
    return true;
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return !frontNode;
}