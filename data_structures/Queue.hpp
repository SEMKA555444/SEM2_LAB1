#pragma once

struct QueueNode {
    float data;
    QueueNode* next;
};

struct Queue {
    QueueNode* head;
    QueueNode* tail;
    int size;
};

void initQueue(Queue* queue);
bool isEmpty(const Queue* queue);
void push(Queue* queue, float value);
void pop(Queue* queue);
void print(const Queue* queue);
float front(const Queue* queue);
void clear(Queue* queue);
int size(const Queue* queue);