#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

class Node {
    friend class DoublyLinkedList;
public:
    Node();
    Node(int v);
    ~Node();
private:
    Node *prev;
    int value;
    Node *next;
};
#endif
