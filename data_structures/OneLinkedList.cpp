#include "OneLinkedList.hpp"

#include <iostream>
#include <cmath>

using namespace std;

using Node = SNode;

void createList(OneLinkedList* list) {
    if (!list) return;
    list->head = nullptr;
    list->tail = nullptr;
}

void destroyList(OneLinkedList* list) {
    if (!list) return;
    Node* current = list->head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    list->head = list->tail = nullptr;
}

void push_front(OneLinkedList* list, float value) {
    if (!list) return;
    Node* newNode = new Node{value, list->head};
    list->head = newNode;
    if (list->tail == nullptr) {
        list->tail = newNode;
    }
}

void push_back(OneLinkedList* list, float value) {
    if (!list) return;
    Node* newNode = new Node{value, nullptr};
    if (list->tail != nullptr) {
        list->tail->next = newNode;
    }
    list->tail = newNode;
    if (list->head == nullptr) {
        list->head = newNode;
    }
}

Node* getAt(const OneLinkedList* list, int index) {
    if (!list || index < 0) return nullptr;
    Node* current = list->head;
    for (int i = 0; i < index && current != nullptr; ++i) {
        current = current->next;
    }
    return current;
}

void insertForward(OneLinkedList* list, int index, float value) {
    if (!list || index < 0) return;

    if (index == 0) {
        push_front(list, value);
        return;
    }

    Node* prev = getAt(list, index - 1);
    if (!prev) return; 

    if (prev == list->tail) {
        push_back(list, value);
        return;
    }

    Node* newNode = new Node{value, prev->next};
    prev->next = newNode;
}

void insertBackward(OneLinkedList* list, int index, float value) {
    if (!list || index < 0) return;

    Node* target = getAt(list, index);
    if (!target) return; 

    if (target == list->tail) {
        push_back(list, value);
        return;
    }

    Node* newNode = new Node{value, target->next};
    target->next = newNode;
}

void pop_front(OneLinkedList* list) {
    if (!list || list->head == nullptr) return;
    Node* temp = list->head;
    list->head = list->head->next;
    delete temp;
    if (list->head == nullptr) {
        list->tail = nullptr;
    }
}
        
void pop_back(OneLinkedList* list) {
    if (!list || list->head == nullptr) {
        return;
    }
    if (list->head == list->tail) {
        delete list->head;
        list->head = nullptr;
        list->tail = nullptr;
        return;
    }
    Node* current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }
    delete list->tail;
    current->next = nullptr;
    list->tail = current;
}

void removeForward(OneLinkedList* list, int index) {
    if (!list || index <= 0 || list->head == nullptr) return;

    if (index == 1) {
        pop_front(list);
        return;
    }

    Node* prev = getAt(list, index - 2);
    if (!prev || prev->next == nullptr) return;

    Node* target = prev->next;
    prev->next = target->next;

    if (target == list->tail) {
        list->tail = prev;
    }

    delete target;
}

void removeBackward(OneLinkedList* list, int index) {
    if (!list || index < 0 || list->head == nullptr) return;

    Node* current = getAt(list, index);
    if (!current || current->next == nullptr) return; 

    if (current->next == list->tail) {
        pop_back(list);
        return;
    }

    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
}

void printList(const OneLinkedList* list) {
    if (!list) return;
    Node* current = list->head;
    int elementIndex = 0;
    while (current != nullptr) {
        cout << elementIndex << " : " << current->data << endl;
        current = current->next;
        elementIndex++;
    }
}

int length(const OneLinkedList* list) {
    if (!list) return 0;
    int count = 0;
    Node* current = list->head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void printListReverse(const OneLinkedList* list) {
    if (!list) return;
    int len = length(list);
    for (int i = len - 1; i >= 0; --i) {
        Node* current = getAt(list, i);
        if (current != nullptr) {
            cout << i << " : " << current->data << endl;
        }
    }
}

void insertSorted(OneLinkedList* list, float value) {
    if (!list) return;

    if (list->head == nullptr || list->head->data >= value) {
        push_front(list, value);
        return;
    }

    Node* current = list->head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }

    Node* newNode = new Node{value, current->next};
    current->next = newNode;
    if (current == list->tail) {
        list->tail = newNode;
    }
}

OneLinkedList* ListAscending(const OneLinkedList* list) {
    if (!list) return nullptr;

    OneLinkedList* newList = new OneLinkedList;
    createList(newList);

    Node* current = list->head;
    while (current != nullptr) {
        insertSorted(newList, current->data);
        current = current->next;
    }

    return newList;
}

void printListAscending(const OneLinkedList* list) {
    OneLinkedList* sortedList = ListAscending(list);
    if (!sortedList) return;
    printList(sortedList);
    destroyList(sortedList);
    delete sortedList;
}

void insertSortedDescending(OneLinkedList* list, float value) {
    if (!list) return;

    if (list->head == nullptr || list->head->data <= value) {
        push_front(list, value);
        return;
    }

    Node* current = list->head;

    while (current->next != nullptr && current->next->data > value) {
            current = current->next;
        }

    Node* newNode = new Node{value, current->next};
    current->next = newNode;

    if (current == list->tail) {
        list->tail = newNode;
    }
}

OneLinkedList* ListDescending(const OneLinkedList* list) {
    if (!list) return nullptr;

    OneLinkedList* newList = new OneLinkedList;
    createList(newList);

    Node* current = list->head;
    while (current != nullptr) {
        insertSortedDescending(newList, current->data);
        current = current->next;
    }

    return newList;
}

void printListDescending(const OneLinkedList* list) {
    OneLinkedList* sortedList = ListDescending(list);
    if (!sortedList) return;
    printList(sortedList);
    destroyList(sortedList);
    delete sortedList;
}

void deletingByValue(OneLinkedList* list, float value) {
    if (!list || list->head == nullptr) return;

    const float EPS = 1e-6f;
    Node* current = list->head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (fabs(current->data - value) < EPS) {
            if (previous == nullptr) {
                pop_front(list);
                current = list->head;
            } else {
                previous->next = current->next;
                if (current == list->tail) {
                    list->tail = previous;
                }
                delete current;
                current = previous->next;
            }
        } else {
            previous = current;
            current = current->next;
        }
    }
}

void searchByValue(OneLinkedList* list, float value) {
    if (!list) return;

    const float EPS = 1e-6f;
    Node* current = list->head;
    int index = 0;

    while (current != nullptr) {
        if (fabs(current->data - value) < EPS) {
            cout << "Value " << value << " found at index: " << index << endl;
            return;
        }
        current = current->next;
        index++;
    }
    cout << "Value " << value << " not found in the list." << endl;
}