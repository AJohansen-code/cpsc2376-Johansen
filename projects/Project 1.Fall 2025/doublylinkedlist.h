#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
#include "Node.h"

class DoublyLinkedList{
public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    void addNewNodeToFront(Node* newNode);
    void addNewNodeToBack(Node* newnNde);
    Node* removeNodeFromFront();
    void removeNodeFromBack();
    void displayDoublyLinkedList();
    bool isPalindrome();
    void split(int n);
    void drawDoublyLinkedList();
    
private:
    Node* head;
    Node* tail;
    int countSize();
};
#endif