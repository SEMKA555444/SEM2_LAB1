#pragma once

#include <iostream>

template <typename T> struct SNode {
  T data;
  SNode<T> *next;
};

template <typename T> struct OneLinkedList {
  SNode<T> *head;
  SNode<T> *tail;
};

template <typename T> void createList(OneLinkedList<T> *list) {
  if (!list)
    return;
  list->head = nullptr;
  list->tail = nullptr;
}

template <typename T> void destroyList(OneLinkedList<T> *list) {
  if (!list)
    return;
  SNode<T> *current = list->head;
  while (current != nullptr) {
    SNode<T> *next = current->next;
    delete current;
    current = next;
  }
  list->head = list->tail = nullptr;
}

template <typename T> void push_front(OneLinkedList<T> *list, const T &value) {
  if (!list)
    return;
  SNode<T> *newNode = new SNode<T>{value, list->head};
  list->head = newNode;
  if (list->tail == nullptr) {
    list->tail = newNode;
  }
}

template <typename T> void push_back(OneLinkedList<T> *list, const T &value) {
  if (!list)
    return;
  SNode<T> *newNode = new SNode<T>{value, nullptr};
  if (list->tail != nullptr) {
    list->tail->next = newNode;
  }
  list->tail = newNode;
  if (list->head == nullptr) {
    list->head = newNode;
  }
}

template <typename T> SNode<T> *getAt(const OneLinkedList<T> *list, int index) {
  if (!list || index < 0)
    return nullptr;
  SNode<T> *current = list->head;
  for (int i = 0; i < index && current != nullptr; ++i) {
    current = current->next;
  }
  return current;
}

template <typename T>
void insertForward(OneLinkedList<T> *list, const T &targetValue,
                   const T &newValue) {
  if (!list || list->head == nullptr)
    return;

  if (list->head->data == targetValue) {
    push_front(list, newValue);
    return;
  }

  SNode<T> *current = list->head;
  while (current->next != nullptr && current->next->data != targetValue) {
    current = current->next;
  }

  if (current->next == nullptr)
    return;

  SNode<T> *newNode = new SNode<T>{newValue, current->next};
  current->next = newNode;
}

template <typename T>
void insertBackward(OneLinkedList<T> *list, const T &targetValue,
                    const T &newValue) {
  if (!list || list->head == nullptr)
    return;

  SNode<T> *current = list->head;
  while (current != nullptr && current->data != targetValue) {
    current = current->next;
  }

  if (current == nullptr)
    return;

  if (current == list->tail) {
    push_back(list, newValue);
    return;
  }

  SNode<T> *newNode = new SNode<T>{newValue, current->next};
  current->next = newNode;
}

template <typename T> void pop_front(OneLinkedList<T> *list) {
  if (!list || list->head == nullptr)
    return;
  SNode<T> *temp = list->head;
  list->head = list->head->next;
  delete temp;
  if (list->head == nullptr) {
    list->tail = nullptr;
  }
}

template <typename T> void pop_back(OneLinkedList<T> *list) {
  if (!list || list->head == nullptr) {
    return;
  }
  if (list->head == list->tail) {
    delete list->head;
    list->head = nullptr;
    list->tail = nullptr;
    return;
  }
  SNode<T> *current = list->head;
  while (current->next != list->tail) {
    current = current->next;
  }
  delete list->tail;
  current->next = nullptr;
  list->tail = current;
}

template <typename T>
void removeForward(OneLinkedList<T> *list, const T &targetValue) {
  if (!list || list->head == nullptr || list->head->next == nullptr)
    return;

  if (list->head->data == targetValue)
    return;

  if (list->head->next->data == targetValue) {
    pop_front(list);
    return;
  }

  SNode<T> *prev = list->head;
  while (prev->next->next != nullptr && prev->next->next->data != targetValue) {
    prev = prev->next;
  }

  if (prev->next->next == nullptr)
    return;

  SNode<T> *toDelete = prev->next;
  prev->next = toDelete->next;
  delete toDelete;
}

template <typename T>
void removeBackward(OneLinkedList<T> *list, const T &targetValue) {
  if (!list || list->head == nullptr)
    return;

  SNode<T> *current = list->head;
  while (current != nullptr && current->data != targetValue) {
    current = current->next;
  }

  if (current == nullptr || current->next == nullptr)
    return;

  if (current->next == list->tail) {
    pop_back(list);
    return;
  }

  SNode<T> *toDelete = current->next;
  current->next = toDelete->next;
  delete toDelete;
}

template <typename T> void printList(const OneLinkedList<T> *list) {
  if (!list)
    return;
  SNode<T> *current = list->head;
  int elementIndex = 0;
  while (current != nullptr) {
    std::cout << elementIndex << " : " << current->data << std::endl;
    current = current->next;
    elementIndex++;
  }
}

template <typename T> int length(const OneLinkedList<T> *list) {
  if (!list)
    return 0;
  int count = 0;
  SNode<T> *current = list->head;
  while (current != nullptr) {
    count++;
    current = current->next;
  }
  return count;
}

template <typename T> void printListReverse(const OneLinkedList<T> *list) {
  if (!list)
    return;
  int len = length(list);
  for (int i = len - 1; i >= 0; --i) {
    SNode<T> *current = getAt(list, i);
    if (current != nullptr) {
      std::cout << i << " : " << current->data << std::endl;
    }
  }
}

template <typename T>
void insertSorted(OneLinkedList<T> *list, const T &value) {
  if (!list)
    return;

  if (list->head == nullptr || list->head->data >= value) {
    push_front(list, value);
    return;
  }

  SNode<T> *current = list->head;
  while (current->next != nullptr && current->next->data < value) {
    current = current->next;
  }

  SNode<T> *newNode = new SNode<T>{value, current->next};
  current->next = newNode;
  if (current == list->tail) {
    list->tail = newNode;
  }
}

template <typename T>
OneLinkedList<T> *ListAscending(const OneLinkedList<T> *list) {
  if (!list)
    return nullptr;

  OneLinkedList<T> *newList = new OneLinkedList<T>;
  createList(newList);

  SNode<T> *current = list->head;
  while (current != nullptr) {
    insertSorted(newList, current->data);
    current = current->next;
  }

  return newList;
}

template <typename T> void printListAscending(const OneLinkedList<T> *list) {
  OneLinkedList<T> *sortedList = ListAscending(list);
  if (!sortedList)
    return;
  printList(sortedList);
  destroyList(sortedList);
  delete sortedList;
}

template <typename T>
void insertSortedDescending(OneLinkedList<T> *list, const T &value) {
  if (!list)
    return;

  if (list->head == nullptr || list->head->data <= value) {
    push_front(list, value);
    return;
  }

  SNode<T> *current = list->head;

  while (current->next != nullptr && current->next->data > value) {
    current = current->next;
  }

  SNode<T> *newNode = new SNode<T>{value, current->next};
  current->next = newNode;

  if (current == list->tail) {
    list->tail = newNode;
  }
}

template <typename T>
OneLinkedList<T> *ListDescending(const OneLinkedList<T> *list) {
  if (!list)
    return nullptr;

  OneLinkedList<T> *newList = new OneLinkedList<T>;
  createList(newList);

  SNode<T> *current = list->head;
  while (current != nullptr) {
    insertSortedDescending(newList, current->data);
    current = current->next;
  }

  return newList;
}

template <typename T> void printListDescending(const OneLinkedList<T> *list) {
  OneLinkedList<T> *sortedList = ListDescending(list);
  if (!sortedList)
    return;
  printList(sortedList);
  destroyList(sortedList);
  delete sortedList;
}

template <typename T>
void deletingByValue(OneLinkedList<T> *list, const T &value) {
  if (!list || list->head == nullptr)
    return;

  SNode<T> *current = list->head;
  SNode<T> *previous = nullptr;

  while (current != nullptr) {
    if (current->data == value) {
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

template <typename T>
void searchByValue(OneLinkedList<T> *list, const T &value) {
  if (!list)
    return;

  SNode<T> *current = list->head;
  int index = 0;

  while (current != nullptr) {
    if (current->data == value) {
      std::cout << "Value " << value << " found at index: " << index
                << std::endl;
      return;
    }
    current = current->next;
    index++;
  }
  std::cout << "Value " << value << " not found in the list." << std::endl;
}