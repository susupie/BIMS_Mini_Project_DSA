#pragma once 



#include <iostream> 

using namespace std;



template <typename T>

class Stack {

private:

    struct Node {

        T data;

        Node* next;

        Node(const T& d) : data(d), next(nullptr) {}

    };

    Node* topNode;

public:

    Stack() : topNode(nullptr) {}

    ~Stack() {

        while (topNode) {

            Node* toDelete = topNode;

            topNode = topNode->next;

            delete toDelete;

        }

    }

    void push(const T& data) {

        Node* node = new Node(data);

        node->next = topNode;

        topNode = node;

    }

    bool pop(T& data) {

        if (!topNode) return false;

        data = topNode->data;

        Node* toDelete = topNode;

        topNode = topNode->next;

        delete toDelete;

        return true;

    }

    bool isEmpty() const { return !topNode; }

};

