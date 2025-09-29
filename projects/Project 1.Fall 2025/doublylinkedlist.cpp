//Program Headers:
//University of Arkansas at Little Rock
//Department of Computer Science
//CPSI 28003-01 and 9S1L: Algorithms
//Fall 2025
//Project 1: Implementing a Doubly Linked List
//Due Date: September 30, 2025, 10:50 AM, Tuesday
//Name: Alicia Acosta
//Student ID:
//Discriptiom of the program:"
//Date written: September 20, 2025
//Date last revised: September 27, 2025



#ifndef DOUBLYLINKEDLIST_CPP
#define DOUBLYLINKEDLIST_CPP

#include "doublylinkedlist.h"

DoublyLinkedList::DoublyLinkedList(){
head = NULL;
tail = NULL;
}

DoublyLinkedList::~DoublyLinkedList(){
    Node* current = head;
    while (current != NULL) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = NULL;
    tail = NULL;
}

void DoublyLinkedList::addNewNodeToFront(Node * newNode) {
    if(head == NULL && tail == NULL) {
        head = tail = newNode;
    } else {
        head->prev = newNode;
        newNode->next = head;
        head = newNode;
    }
}

void DoublyLinkedList::addNewNodeToBack(Node * newNode) {
    if (head == NULL && tail == NULL) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

Node* DoublyLinkedList::removeNodeFromFront(){
    if (head == NULL) {
        return NULL; // List is empty
    }

    Node *tempNode = head;
    head = head->next;
    if (head != NULL) {
        head->prev = NULL;
    } else {
        tail = NULL; 
    }
    tempNode->next = NULL; 
    return tempNode;
}

void DoublyLinkedList::removeNodeFromBack(){
    if (tail == NULL) {
        return; // List is empty
    }

    Node *tempNode = tail;
    tail = tail->prev;
    if (tail != NULL) {
        tail->next = NULL;
    } else {
        head = NULL; 
    }
    delete tempNode; 
}

void DoublyLinkedList::displayDoublyLinkedList()
{
    Node *tempNode = head;
    while (tempNode != NULL)
    {
        cout << tempNode->value << " ";
        tempNode = tempNode->next;
    }
    cout << endl;
}

void DoublyLinkedList::drawDoublyLinkedList()
{
    Node *tempNode = head;
    while (tempNode != NULL) {
    cout << "[address:" << tempNode << ", prev: ";
    if (tempNode->prev == NULL) {
        cout << "0";
    } else {
        cout << tempNode->prev;
    }
    cout << ", value: " << tempNode->value;

    cout << ", next: ";
    if (tempNode->next == NULL) {
        cout << "0";
    } else {
        cout << tempNode->next;
    }
    cout << " ]" << endl;
    tempNode = tempNode->next;
    }
}

bool DoublyLinkedList::isPalindrome() {
    if (head == NULL) {
        return true; 
    }

    Node* front = head;
    Node* back = tail;

    while (front != back && front->prev != back) {
        if (front->value != back->value) {
            return false; 
        }
        front = front->next;
        back = back->prev;
    }

    return true; 
}

int DoublyLinkedList::countSize() {
    int count = 0;
    Node* tempNode = head;
    while (tempNode != NULL) {
        count++;
        tempNode = tempNode->next;
    }
    return count;
}

void DoublyLinkedList::split(int n) {
    int totalSize = countSize();

    if (n < 1 || totalSize < n || totalSize % n != 0) {
        cout << "cannot be processed." << endl; 
        return;
    }
    int partitionSize = totalSize / n;
    Node* current = head;

    for (int i = 0; i < n; i++) {
        DoublyLinkedList subList;
        subList.head = current;

        Node* subListTail = current;
        for (int j = 1; j < partitionSize; j++) {
            subListTail = subListTail->next;
        }

        Node* nextPartitionHead = subListTail->next;
        if (nextPartitionHead != NULL) {
            nextPartitionHead->prev = NULL;
        }
        subListTail->next = NULL;
        subList.tail = subListTail;

        cout << "Sub-list" << i + 1 << ": ";
        subList.displayDoublyLinkedList();

        current = nextPartitionHead;
    }

        head = NULL;
        tail = NULL;
}
#endif
