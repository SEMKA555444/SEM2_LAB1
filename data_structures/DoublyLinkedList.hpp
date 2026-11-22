#pragma once

#include <iostream>

template <typename T>
struct DNode {
    T data;
    DNode<T>* next;
    DNode<T>* prev;
};

template <typename T>
struct DoublyLinkedList {
    DNode<T>* head;
    DNode<T>* tail;
};

template <typename T>
void createList(DoublyLinkedList<T>* list) {
    if (!list) return;
    list->head = nullptr;
    list->tail = nullptr;
}

template <typename T>
void deleteList(DoublyLinkedList<T>* list) {
    if (!list) return;
    DNode<T>* current = list->head;
    while (current != nullptr) {
        DNode<T>* NextNode = current->next;
        delete current;
        current = NextNode;
    }
    list->head = nullptr;
    list->tail = nullptr;
}

template <typename T>
void push_front(DoublyLinkedList<T>* list, const T& value) {
    if (!list) return;
    DNode<T>* newNode = new DNode<T>{value, list->head, nullptr};
    if (list->head != nullptr) {
        list->head->prev = newNode;
        list->head = newNode;
    }
    else {
        list->head = newNode;
        list->tail = newNode;
    }
}

template <typename T>
void push_back(DoublyLinkedList<T>* list, const T& value) {
    if (!list) return;
    DNode<T>* newNode = new DNode<T>{value, nullptr, list->tail};
    if (list->tail != nullptr) {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    else {
        list->head = newNode;
        list->tail = newNode;
    }
}

template <typename T>
DNode<T>* getAt(const DoublyLinkedList<T>* list, int index) {
    if (!list || index < 0) return nullptr;
    DNode<T>* current = list->head;
    for (int i = 0; i < index && current != nullptr; ++i) {
        current = current->next;
    }
    return current;
}

template <typename T>
void insertForward(DoublyLinkedList<T>* list, int index, const T& value) {
    if (!list || index < 0) return;
    if (index == 0) {
        push_front(list, value);
        return;
    }

    DNode<T>* current = getAt(list, index);

    if (!current) return;
    DNode<T>* newNode = new DNode<T>{value, current, current->prev};

    current->prev->next = newNode;
    current->prev = newNode;
}

template <typename T>
void insertBackward(DoublyLinkedList<T>* list, int index, const T& value) {
    if (!list || index < 0) return;

    DNode<T>* current = getAt(list, index);

    if (!current) return;

    if (current == list->tail) {
        push_back(list, value);
        return;
    }

    DNode<T>* newNode = new DNode<T>{value, current->next, current};

    current->next->prev = newNode;
    current->next = newNode;
}

template <typename T>
void pop_front(DoublyLinkedList<T>* list) {
    if (!list || !list->head) return;
    DNode<T>* temp = list->head;
    list->head = list->head->next;
    if (list->head) {
        list->head->prev = nullptr;
    } else {
        list->tail = nullptr;
    }
    delete temp;
}

template <typename T>
void pop_back(DoublyLinkedList<T>* list) {
    if (!list || !list->tail) return;
    DNode<T>* temp = list->tail;
    list->tail = list->tail->prev;
    if (list->tail) {
        list->tail->next = nullptr;
    } else {
        list->head = nullptr;
    }
    delete temp;
}

template <typename T>
void removeForward(DoublyLinkedList<T>* list, int index) {
    if (!list || index < 0) return;
    DNode<T>* current = getAt(list, index - 1);
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

template <typename T>
void removeBackward(DoublyLinkedList<T>* list, int index) {
    if (!list || index < 0) return;
    DNode<T>* current = getAt(list, index + 1);
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

template <typename T>
int length(const DoublyLinkedList<T>* list) {
    if (!list) return 0;
    int count = 0;
    DNode<T>* current = list->head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

template <typename T>
void printList(const DoublyLinkedList<T>* list) {
    if (!list) return;
    DNode<T>* current = list->head;
    int index = 0;
    while (current != nullptr) {
        std::cout << index << " : " << current->data << std::endl;
        current = current->next;
        index++;
    }
}

template <typename T>
void printListReverse(const DoublyLinkedList<T>* list) {
    if (!list) return;
    DNode<T>* current = list->tail;
    int index = length(list) - 1;
    while (current != nullptr) {
        std::cout << index << " : " << current->data << std::endl;
        current = current->prev;
        index--;
    }
}

template <typename T>
void insertSorted(DoublyLinkedList<T>* list, const T& value) {
    if (!list) return;

    if (list->head == nullptr || list->head->data >= value) {
        push_front(list, value);
        return;
    }

    if (list->tail->data <= value) {
        push_back(list, value);
        return;
    }

    DNode<T>* current = list->head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }

    DNode<T>* newNode = new DNode<T>{value, current->next, current};
    if (current->next != nullptr) {
        current->next->prev = newNode;
    }
    current->next = newNode;
}

template <typename T>
DoublyLinkedList<T>* ListAscending(const DoublyLinkedList<T>* list) {
    if (!list) return nullptr;

    DoublyLinkedList<T>* newList = new DoublyLinkedList<T>;
    createList(newList);

    DNode<T>* current = list->head;
    while (current != nullptr) {
        insertSorted(newList, current->data);
        current = current->next;
    }

    return newList;
}

template <typename T>
void printListAscending(const DoublyLinkedList<T>* list) {
    DoublyLinkedList<T>* sortedList = ListAscending(list);
    if (!sortedList) return;
    printList(sortedList);
    deleteList(sortedList);
    delete sortedList;
}

template <typename T>
void insertSortedDescending(DoublyLinkedList<T>* list, const T& value) {
    if (!list) return;

    if (list->head == nullptr || list->head->data <= value) {
        push_front(list, value);
        return;
    }

    if (list->tail->data >= value) {
        push_back(list, value);
        return;
    }

    DNode<T>* current = list->head;

    while (current->next != nullptr && current->next->data > value) {
        current = current->next;
    }

    DNode<T>* newNode = new DNode<T>{value, current->next, current};
    current->next->prev = newNode;
    current->next = newNode;
}

template <typename T>
DoublyLinkedList<T>* ListDescending(const DoublyLinkedList<T>* list) {
    if (!list) return nullptr;

    DoublyLinkedList<T>* newList = new DoublyLinkedList<T>;
    createList(newList);

    DNode<T>* current = list->head;
    while (current != nullptr) {
        insertSortedDescending(newList, current->data);
        current = current->next;
    }

    return newList;
}

template <typename T>
void printListDescending(const DoublyLinkedList<T>* list) {
    DoublyLinkedList<T>* sortedList = ListDescending(list);
    if (!sortedList) return;
    printList(sortedList);
    deleteList(sortedList);
    delete sortedList;
}

template <typename T>
void deletingByValue(DoublyLinkedList<T>* list, const T& value) {
    if (!list || list->head == nullptr) return;

    DNode<T>* current = list->head;

    while (current != nullptr) {
        if (current->data == value) {
            DNode<T>* toDelete = current;
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

template <typename T>
void searchByValue(DoublyLinkedList<T>* list, const T& value) {
    if (!list) return;

    DNode<T>* current = list->head;
    int index = 0;

    while (current != nullptr) {
        if (current->data == value) {
            std::cout << "Value " << value << " found at index: " << index << std::endl;
            return;
        }
        current = current->next;
        index++;
    }
    std::cout << "Value " << value << " not found in the list." << std::endl;
}
