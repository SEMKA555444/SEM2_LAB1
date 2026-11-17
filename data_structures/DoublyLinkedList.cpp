#include "DoublyLinkedList.hpp"

#include <iostream>
#include <cmath>

using namespace std;

using Node = DNode;

void createList(DoublyLinkedList* list) {
    if (!list) return;
    list->head = nullptr;
    list->tail = nullptr;
}

void deleteList(DoublyLinkedList* list) {
    if (!list) return;
    Node* current= list->head;
    while (current != nullptr) {
        Node* NextNode = current->next;
        delete current;
        current = NextNode;
    }
    list->head = nullptr;
    list->tail = nullptr;
}

void push_front(DoublyLinkedList* list, float value) {
    if (!list) return;
    Node* newNode = new Node{value, list->head, nullptr};
    if (list-> head != nullptr) {
        list->head->prev = newNode;
        list->head = newNode;
    }
    else {
        list->head = newNode;
        list->tail = newNode;
    }
}

void push_back(DoublyLinkedList* list, float value) {
    if (!list) return;
    Node *newNode = new Node{value, nullptr, list->tail};
    if (list->tail != nullptr) {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    else {
        list->head = newNode;
        list->tail = newNode;
    }
}

Node* getAt(const DoublyLinkedList* list, int index) {
    if (!list || index < 0) return nullptr;
    Node* current = list->head;
    for (int i = 0; i < index && current != nullptr; ++i) {
        current = current->next;
    }
    return current;
}

void insertForward(DoublyLinkedList* list, int index, float value) {
    if (!list || index < 0) return;
    if (index == 0) {
        push_front(list, value);
        return;
    }
    
    Node* current = getAt(list, index);

    if (!current) return;
    Node* newNode = new Node{value, current, current->prev};

    current->prev->next = newNode;
    current->prev = newNode;
}

void insertBackward(DoublyLinkedList* list, int index, float value) {
    if (!list || index < 0) return;

    Node* current = getAt(list, index);

    if (!current) return;

    if (current == list->tail) {
        push_back(list, value);
        return;
    }

    Node* newNode = new Node{value, current->next, current};


    current->next->prev = newNode;
    current->next = newNode;
}

void pop_front (DoublyLinkedList* list) {
    if (!list || !list->head) return;
    Node* temp = list->head;
    list->head = list->head->next;
    if (list->head) {
        list->head->prev = nullptr;
    } else {
        list->tail = nullptr;
    }
    delete temp;
}

void pop_back (DoublyLinkedList* list) {
    if (!list || !list->tail) return;
    Node* temp = list->tail;
    list->tail = list->tail->prev;
    if (list->tail) {
        list->tail->next = nullptr;
    } else {
        list->head = nullptr;
    }
    delete temp;
}

void removeForward(DoublyLinkedList* list, int index) {
    if (!list || index < 0) return;
    Node* current = getAt(list, index-1);
    if (!current) return;

    if (current == list->head) {
        pop_front(list);
        return;
    }
    if (current == list->tail) {
        pop_back(list);
        return;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
}

void removeBackward(DoublyLinkedList* list, int index) {
    if (!list || index < 0) return;
    Node* current = getAt(list, index+1);
    if (!current) return;

    if (current == list->tail) {
        pop_back(list);
        return;
    }
    if (current == list->head) {
        pop_front(list);
        return;
    }
    current->next->prev = current->prev;
    current->prev->next = current->next;
    delete current;
}

int length(const DoublyLinkedList* list) {
    if (!list) return 0;
    int count = 0;
    Node* current = list->head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;   
}

void printList(const DoublyLinkedList* list) {
    if (!list) return;
    Node* current = list->head;
    int index = 0;
    while (current != nullptr) {
        cout << index << " : " << current->data << endl;
        current = current->next;
        index++;
    }
}

void printListReverse(const DoublyLinkedList* list) {
    if (!list) return;
    Node* current = list->tail;
    int index = length(list) - 1;
    while (current != nullptr) {
        cout << index << " : " << current->data << endl;
        current = current->prev;
        index--;
    }
}

void insertSorted(DoublyLinkedList* list, float value) {
    if (!list) return;

    if (list->head == nullptr || list->head->data >= value) {
        push_front(list, value);
        return;
    }

    if (list->tail->data <= value) {
        push_back(list, value);
        return;
    }

    Node* current = list->head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }

    Node* newNode = new Node{value, current->next, current};
    if (current->next != nullptr) {
        current->next->prev = newNode;
    }
    current->next = newNode;
}

DoublyLinkedList* ListAscending(const DoublyLinkedList* list) {
    if (!list) return nullptr;

    DoublyLinkedList* newList = new DoublyLinkedList;
    createList(newList);

    Node* current = list->head;
    while (current != nullptr) {
        insertSorted(newList, current->data);
        current = current->next;
    }

    return newList;
}

void printListAscending(const DoublyLinkedList* list) {
    DoublyLinkedList* sortedList = ListAscending(list);
    if (!sortedList) return;
    printList(sortedList);
    deleteList(sortedList);
    delete sortedList;
}

void insertSortedDescending(DoublyLinkedList* list, float value) {
    if (!list) return;

    if (list->head == nullptr || list->head->data <= value) {
        push_front(list, value);
        return;
    }

    if (list->tail->data >= value) {
        push_back(list, value);
        return;
    }

    Node* current = list->head;

    while (current->next != nullptr && current->next->data > value) {
            current = current->next;
        }

    Node* newNode = new Node{value, current->next, current};
    current->next->prev = newNode;
    current->next = newNode;
}

DoublyLinkedList* ListDescending(const DoublyLinkedList* list) {
    if (!list) return nullptr;

    DoublyLinkedList* newList = new DoublyLinkedList;
    createList(newList);

    Node* current = list->head;
    while (current != nullptr) {
        insertSortedDescending(newList, current->data);
        current = current->next;
    }

    return newList;
}

void printListDescending(const DoublyLinkedList* list) {
    DoublyLinkedList* sortedList = ListDescending(list);
    if (!sortedList) return;
    printList(sortedList);
    deleteList(sortedList);
    delete sortedList;
}

void deletingByValue(DoublyLinkedList* list, float value) {
    if (!list || list->head == nullptr) return;

    const float EPS = 1e-6f;
    Node* current = list->head;

    while (current != nullptr) {
        if (fabs(current->data - value) < EPS) {
            Node* toDelete = current;
            current = current->next;

            if (toDelete == list->head) {
                pop_front(list);
            } else if (toDelete == list->tail) {
                pop_back(list);
            } else {
                toDelete->prev->next = toDelete->next;
                toDelete->next->prev = toDelete->prev;
                delete toDelete;
            }
        } else {
            current = current->next;
        }
    }
}

void searchByValue(DoublyLinkedList* list, float value) {
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