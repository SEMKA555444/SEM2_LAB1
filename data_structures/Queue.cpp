#include "Queue.hpp"
#include <iostream>

using namespace std;

using Node = QueueNode;

void initQueue(Queue* queue) {
    if (!queue) return;
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0;
}

bool isEmpty(const Queue* queue) {
    return (queue == nullptr || queue->head == nullptr);
}

void push(Queue* queue, float value) {
    if (!queue) return;
    Node* newNode = new Node{value, nullptr};
    if (queue->head == nullptr) {
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size += 1;
}

void pop(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Queue is empty" << endl;
        return;
    }
    Node* temp = queue->head;
    queue->head = queue->head->next;
    queue->size -= 1;

    if (queue->head == nullptr) {
        queue->tail = nullptr;
        queue->size = 0;
    }
    delete temp;
}

float front(const Queue* queue) {
    if (isEmpty(queue)) {
        cerr << "Queue is empty" << endl;
        return 0.0f;
    }
    return queue->head->data;
}

void print(const Queue* queue) {
    if (isEmpty(queue)) {
        cerr << "Queue is empty" << endl;
        return;
    }
    Node* current = queue->head;
    int index = 0;
    while (current != nullptr) {
        cout << index << " : " << current->data << endl;
        current = current->next;
        index++;
    }
}

void clear(Queue* queue) {
    while (!isEmpty(queue)) {
        pop(queue);
    }
}

int size(const Queue* queue) {
    return queue ? queue->size : 0;
}