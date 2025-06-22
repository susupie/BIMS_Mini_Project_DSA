#pragma once 



#include <iostream> 

using namespace std;



template <typename T>

struct Node {

    T data;

    Node* next;

    Node(const T& data) : data(data), next(nullptr) {}

};



template <typename T>

class LinkedList {

private:

    Node<T>* head;

public:

    LinkedList() : head(nullptr) {}

    ~LinkedList() {

        Node<T>* current = head;

        while (current) {

            Node<T>* toDelete = current;

            current = current->next;

            delete toDelete;

        }

    }

    void addFront(const T& data) {

        Node<T>* newNode = new Node<T>(data);

        newNode->next = head;

        head = newNode;

    }

    bool remove(const T& data) {

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

    Node<T>* find(const T& data) {

        Node<T>* current = head;

        while (current) {

            if (current->data == data)

                return current;

            current = current->next;

        }

        return nullptr;

    }

    void update(const T& oldData, const T& newData) {

        Node<T>* node = find(oldData);

        if (node)

            node->data = newData;

    }

    void displayAll() const {

        Node<T>* current = head;

        while (current) {

            cout << current->data << endl;

            current = current->next;

        }

    }

    Node<T>* getHead() const { return head; }

};