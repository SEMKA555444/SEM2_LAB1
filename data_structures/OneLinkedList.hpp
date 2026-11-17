#pragma once

#include <iostream>

struct SNode {
    float data;
    SNode* next;
};

struct OneLinkedList {
    SNode* head;
    SNode* tail;
};

void createList(OneLinkedList* list);
void destroyList(OneLinkedList* list);
void push_front(OneLinkedList* list, float value);
void push_back(OneLinkedList* list, float value);
SNode* getAt(const OneLinkedList* list, int index);
void insertForward(OneLinkedList* list, int index, float value);
void insertBackward(OneLinkedList* list, int index, float value);
void pop_front(OneLinkedList* list);
void pop_back(OneLinkedList* list);
void removeForward(OneLinkedList* list, int index);
void removeBackward(OneLinkedList* list, int index);
void printList(const OneLinkedList* list);
void printListReverse(const OneLinkedList* list);
void printListAscending(const OneLinkedList* list);
void printListDescending(const OneLinkedList* list);
void deletingByValue(OneLinkedList* list, float value);
void searchByValue(OneLinkedList* list, float value);
int length(const OneLinkedList* list);
OneLinkedList* ListAscending(const OneLinkedList* list);
OneLinkedList* ListDescending(const OneLinkedList* list);
void insertSorted(OneLinkedList* list, float value);
void insertSortedDescending(OneLinkedList* list, float value);