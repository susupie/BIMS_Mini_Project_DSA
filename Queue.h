#pragma once 



#include <iostream> 

using namespace std;



template <typename T>

class Queue {

private:

    struct Node {

        T data;

        Node* next;

        Node(const T& d) : data(d), next(nullptr) {}

    };

    Node* frontNode;

    Node* rearNode;

public:

    Queue() : frontNode(nullptr), rearNode(nullptr) {}

    ~Queue() {

        while (frontNode) {

            Node* toDelete = frontNode;

            frontNode = frontNode->next;

            delete toDelete;

        }

    }

    void enqueue(const T& data) {

        Node* node = new Node(data);

        if (rearNode) rearNode->next = node;

        else frontNode = node;

        rearNode = node;

    }

    bool dequeue(T& data) {

        if (!frontNode) return false;

        data = frontNode->data;

        Node* toDelete = frontNode;

        frontNode = frontNode->next;

        if (!frontNode) rearNode = nullptr;

        delete toDelete;

        return true;

    }

    bool isEmpty() const { return !frontNode; }

};