#pragma once

#include <iostream>

template <typename T>
struct Array {
    T* data;
    int size;
    int capacity;
};

template <typename T>
void create(Array<T>* arr, int initialCapacity = 4) {
    if (!arr) return;
    arr->capacity = initialCapacity > 0 ? initialCapacity : 1;
    arr->size = 0;
    arr->data = new T[arr->capacity];
}

template <typename T>
void destroy(Array<T>* arr) {
    if (!arr) return;
    delete[] arr->data;
    arr->data = nullptr;
    arr->capacity = 0;
    arr->size = 0;
}

template <typename T>
void reserve(Array<T>* arr, int newCapacity) {
    if (!arr || newCapacity <= arr->capacity)
        return;
    arr->capacity = newCapacity;
    T* newData = new T[arr->capacity];
    for (int i = 0; i < arr->size; ++i)
        newData[i] = arr->data[i];
    delete[] arr->data;
    arr->data = newData;
}

template <typename T>
void push_back(Array<T>* arr, const T& value) {
    if (!arr) return;
    if (arr->size == arr->capacity)
        reserve(arr, arr->capacity * 2);
    arr->data[arr->size] = value;
    arr->size++;
}

template <typename T>
void insert(Array<T>* arr, int index, const T& value) {
    if (!arr || index < 0 || index > arr->size)
        return;
    if (arr->size == arr->capacity)
        reserve(arr, arr->capacity * 2);
    for (int i = arr->size; i > index; --i)
    {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[index] = value;
    arr->size++;
}

template <typename T>
T get(const Array<T>* arr, int index) {
    if (!arr || index < 0 || index >= arr->size) {
        std::cerr << "Index out of range" << std::endl;
        return T{};
    }
    std::cout << arr->data[index] << std::endl;
    return arr->data[index];
}

template <typename T>
void pop_back(Array<T>* arr) {
    if (!arr || arr->size == 0)
        return;
    arr->size--;
    arr->data[arr->size] = T{};
}

template <typename T>
void erase(Array<T>* arr, int index) {
    if (!arr || index < 0 || index >= arr->size)
        return;
    for (int i = index; i < arr->size - 1; ++i) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
    arr->data[arr->size] = T{};
}

template <typename T>
void clear(Array<T>* arr) {
    if (!arr) return;
    for (int i = 0; i < arr->size; ++i) {
        arr->data[i] = T{};
    }
    arr->size = 0;
}

template <typename T>
void change(Array<T>* arr, int index, const T& value) {
    if (!arr || index < 0 || index >= arr->size)
        return;
    arr->data[index] = value;
}

template <typename T>
int length(const Array<T>* arr) {
    return arr ? arr->size : 0;
}

template <typename T>
void print(const Array<T>* arr) {
    if (!arr) return;
    for (int i = 0; i < arr->size; ++i)
        std::cout << arr->data[i] << " ";
    std::cout << std::endl;
}
