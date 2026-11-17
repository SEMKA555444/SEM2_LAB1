#include "Stack.hpp"

#include <iostream>

using namespace std;

using Node = StackNode;

void init(Stack* stack) {
    if (!stack) return;
    stack->size = 0;
    stack->top = nullptr;
}

void push(Stack* stack, float value) {
    if (!stack) return;

    Node* NewNode = new Node{value, stack->top};
    stack->top = NewNode;
    stack->size++;
}

bool isEmpty(const Stack* stack) {
    return (!stack || stack->size == 0);
}

void pop(Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return;
    }

    Node* temp = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    delete temp;
}

void clear(Stack* stack) {
    while(!isEmpty(stack)) {
        pop(stack);
    }
}

void print(const Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return;
    }

    Node* current = stack->top;
    int index = 0;
    while (current != nullptr) {
        cout << index << " : " << current->data << endl;
        current = current->next;
        index++;
    }
}

int size(const Stack* stack) {
    return stack ? stack->size : 0;
}

float top(const Stack* stack) {
    if (isEmpty(stack)) {
        cerr << "Stack is empty" << endl;
        return 0.0f;
    }
    return stack->top->data;
}