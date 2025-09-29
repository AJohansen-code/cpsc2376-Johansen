#include <iostream>
#include "doublylinkedlist.h"

using namespace std;

void testDraw() {
    cout << "---Test Draw Functionality---" << endl;
    DoublyLinkedList Dll_1;
    Node n11(10);
    Node n12(20);
    Node n13(30);
    Node n14(40);
    Node n15(50);
    Dll_1.addNewNodeToFront(&n11);
    Dll_1.addNewNodeToBack(&n12);
    Dll_1.addNewNodeToFront(&n13);
    Dll_1.addNewNodeToBack(&n14);
    Dll_1.addNewNodeToFront(&n15);
    Dll_1.drawDoublyLinkedList();
    cout << endl;
}

void testPalindrome() {
    cout << "---Test Palindrome Functionality---" << endl;
    DoublyLinkedList Dll_1;
    Node n11(10);
    Node n12(20);
    Node n13(30);
    Node n14(40);
    Node n15(50);
    Dll_1.addNewNodeToFront(&n11);
    Dll_1.addNewNodeToFront(&n12);
    Dll_1.addNewNodeToFront(&n13);
    Dll_1.addNewNodeToFront(&n14);
    Dll_1.addNewNodeToFront(&n15);
    cout << "Is 50-40-30-20-10 a palindrome? " << (Dll_1.isPalindrome() ? "true" : "false") << endl;

    DoublyLinkedList Dll_2;
    Node n21(10);
    Node n22(20);
    Node n23(20);
    Node n24(10);
    Dll_2.addNewNodeToFront(&n21);
    Dll_2.addNewNodeToFront(&n22);
    Dll_2.addNewNodeToFront(&n23);
    Dll_2.addNewNodeToFront(&n24);
    cout << "Is 10-20-20-10 a palindrome? " << (Dll_2.isPalindrome() ? "true" : "false") << endl;
    cout << endl;
}

void testSplit() {
    cout << "---Test Split Functionality---" << endl;
    DoublyLinkedList*Dll_1 = new DoublyLinkedList();
    Node n11(1);
    Node n12(2);
    Node n13(3);
    Node n14(4);
    Node n15(5);
    Node n16(6);
    Dll_1->addNewNodeToBack(&n11);
    Dll_1->addNewNodeToBack(&n12);
    Dll_1->addNewNodeToBack(&n13);
    Dll_1->addNewNodeToBack(&n14);
    Dll_1->addNewNodeToBack(&n15);
    Dll_1->addNewNodeToBack(&n16); 
    Dll_1->split(3);
    delete Dll_1;
    cout << endl;
}

int main() {
    testDraw();
    testPalindrome();
    testSplit();
    system("pause");
    return 0;
}