#pragma once

#include <iostream>

template <typename T>
struct QueueNode {
    T data;
    QueueNode<T>* next;
};

template <typename T>
struct Queue {
    QueueNode<T>* head;
    QueueNode<T>* tail;
    int size;
};

template <typename T>
void initQueue(Queue<T>* queue) {
    if (!queue) return;
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0;
}

template <typename T>
bool isEmpty(const Queue<T>* queue) {
    return (queue == nullptr || queue->head == nullptr);
}

template <typename T>
void push(Queue<T>* queue, const T& value) {
    if (!queue) return;
    QueueNode<T>* newNode = new QueueNode<T>{value, nullptr};
    if (queue->head == nullptr) {
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size += 1;
}

template <typename T>
void pop(Queue<T>* queue) {
    if (isEmpty(queue)) {
        std::cout << "Queue is empty" << std::endl;
        return;
    }
    QueueNode<T>* temp = queue->head;
    queue->head = queue->head->next;
    queue->size -= 1;

    if (queue->head == nullptr) {
        queue->tail = nullptr;
        queue->size = 0;
    }
    delete temp;
}

template <typename T>
T front(const Queue<T>* queue) {
    if (isEmpty(queue)) {
        std::cerr << "Queue is empty" << std::endl;
        return T{};
    }
    return queue->head->data;
}

template <typename T>
void print(const Queue<T>* queue) {
    if (isEmpty(queue)) {
        std::cerr << "Queue is empty" << std::endl;
        return;
    }
    QueueNode<T>* current = queue->head;
    int index = 0;
    while (current != nullptr) {
        std::cout << index << " : " << current->data << std::endl;
        current = current->next;
        index++;
    }
}

template <typename T>
void clear(Queue<T>* queue) {
    while (!isEmpty(queue)) {
        pop(queue);
    }
}

template <typename T>
int size(const Queue<T>* queue) {
    return queue ? queue->size : 0;
}
