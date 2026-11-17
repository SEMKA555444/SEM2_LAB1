#pragma once

struct DNode {
    float data;
    DNode* next;
    DNode* prev;
};

struct DoublyLinkedList {
    DNode* head;
    DNode* tail;
};

void createList(DoublyLinkedList* list);
void deleteList(DoublyLinkedList* list);
void push_front(DoublyLinkedList* list, float value);
void push_back(DoublyLinkedList* list, float value);
DNode* getAt(const DoublyLinkedList* list, int index);
void insertForward(DoublyLinkedList* list, int index, float value);
void insertBackward(DoublyLinkedList* list, int index, float value);
void pop_front (DoublyLinkedList* list);
void pop_back (DoublyLinkedList* list);
void removeForward(DoublyLinkedList* list, int index);
void removeBackward(DoublyLinkedList* list, int index);
int length(const DoublyLinkedList* list);
void printList(const DoublyLinkedList* list);
void printListReverse(const DoublyLinkedList* list);
void printListAscending(const DoublyLinkedList* list);
void printListDescending(const DoublyLinkedList* list);
void deletingByValue(DoublyLinkedList* list, float value);
void searchByValue(DoublyLinkedList* list, float value);
DoublyLinkedList* ListAscending(const DoublyLinkedList* list);
DoublyLinkedList* ListDescending(const DoublyLinkedList* list);
void insertSorted(DoublyLinkedList* list, float value);
void insertSortedDescending(DoublyLinkedList* list, float value);