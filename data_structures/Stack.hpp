#pragma once

#include <iostream>

template <typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
};

template <typename T>
struct Stack {
    StackNode<T>* top;
    int size;
};

template <typename T>
void init(Stack<T>* stack) {
    if (!stack) return;
    stack->size = 0;
    stack->top = nullptr;
}

template <typename T>
void push(Stack<T>* stack, const T& value) {
    if (!stack) return;

    StackNode<T>* NewNode = new StackNode<T>{value, stack->top};
    stack->top = NewNode;
    stack->size++;
}

template <typename T>
bool isEmpty(const Stack<T>* stack) {
    return (!stack || stack->size == 0);
}

template <typename T>
void pop(Stack<T>* stack) {
    if (isEmpty(stack)) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }

    StackNode<T>* temp = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    delete temp;
}

template <typename T>
void clear(Stack<T>* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}

template <typename T>
void print(const Stack<T>* stack) {
    if (isEmpty(stack)) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }

    StackNode<T>* current = stack->top;
    int index = 0;
    while (current != nullptr) {
        std::cout << index << " : " << current->data << std::endl;
        current = current->next;
        index++;
    }
}

template <typename T>
int size(const Stack<T>* stack) {
    return stack ? stack->size : 0;
}

template <typename T>
T top(const Stack<T>* stack) {
    if (isEmpty(stack)) {
        std::cerr << "Stack is empty" << std::endl;
        return T{};
    }
    return stack->top->data;
}
