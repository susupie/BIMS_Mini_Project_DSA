#pragma once

#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node(const T& data);
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
public:
    LinkedList();
    ~LinkedList();
    void addFront(const T& data);
    bool remove(const T& data);
    Node<T>* find(const T& data);
    void update(const T& oldData, const T& newData);
    void displayAll() const;
    Node<T>* getHead() const;
};

// --- Node constructor ---
template <typename T>
Node<T>::Node(const T& data) : data(data), next(nullptr) {}

// --- LinkedList methods ---
template <typename T>
LinkedList<T>::LinkedList() : head(nullptr) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node<T>* current = head;
    while (current) {
        Node<T>* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

template <typename T>
void LinkedList<T>::addFront(const T& data) {
    Node<T>* newNode = new Node<T>(data);
    newNode->next = head;
    head = newNode;
}

template <typename T>
bool LinkedList<T>::remove(const T& data) {
    Node<T>* current = head;
    Node<T>* prev = nullptr;
    while (current) {
        if (current->data == data) {
            if (prev) prev->next = current->next;
            else head = current->next;
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

template <typename T>
Node<T>* LinkedList<T>::find(const T& data) {
    Node<T>* current = head;
    while (current) {
        if (current->data == data)
            return current;
        current = current->next;
    }
    return nullptr;
}

template <typename T>
void LinkedList<T>::update(const T& oldData, const T& newData) {
    Node<T>* node = find(oldData);
    if (node)
        node->data = newData;
}

template <typename T>
void LinkedList<T>::displayAll() const {
    Node<T>* current = head;
    while (current) {
        cout << current->data << endl;
        current = current->next;
    }
}

template <typename T>
Node<T>* LinkedList<T>::getHead() const {
    return head;
}