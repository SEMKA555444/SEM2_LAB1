#pragma once

struct StackNode {
    float data;
    StackNode* next;
};

struct Stack {
    StackNode* top;
    int size;
};

void push(Stack* stack, float value);
void pop(Stack* stack);
void print(const Stack* stack);
void init(Stack* stack);
bool isEmpty(const Stack* stack);
void clear(Stack* stack);
int size(const Stack* stack);
float top(const Stack* stack);