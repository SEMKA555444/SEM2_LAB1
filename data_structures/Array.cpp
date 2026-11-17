#include "Array.hpp"

#include <iostream>

using namespace std;

void create(Array* arr, int initialCapacity) {
    if (!arr) return;
    arr->capacity = initialCapacity > 0 ? initialCapacity : 1;
    arr->size = 0;
    arr->data = new float[arr->capacity];
}

void destroy(Array* arr) {
    if (!arr) return;
    delete[] arr->data;
    arr->data = nullptr;
    arr->capacity = 0;
    arr->size = 0;
}

void reserve(Array* arr, int newCapacity) {
    if (!arr || newCapacity <= arr->capacity)
        return;
    arr->capacity = newCapacity;
    float* newData = new float[arr->capacity];
    for (int i = 0; i < arr->size; ++i)
        newData[i] = arr->data[i];
    delete[] arr->data;
    arr->data = newData;
}

void push_back(Array* arr, float value) {
    if (!arr) return;
    if (arr->size == arr->capacity)
        reserve(arr, arr->capacity * 2);
    arr->data[arr->size] = value;
    arr->size++;
}

void insert(Array* arr, int index, float value) {
    if (!arr || index < 0 || index > arr->size)
        return;
    if (arr->size == arr->capacity)
        reserve(arr, arr->capacity * 2);
    for (int i = arr->size; i > index; --i)
    {
        arr->data[i] = arr->data[i-1];
    }
    arr->data[index] = value;
    arr->size++;
}

float get(const Array* arr, int index) {
    if (!arr || index < 0 || index >= arr->size) {
        cerr << "Index out of range" << endl;
        return 0.0f;
    }
    cout << arr->data[index] << endl;
    return arr->data[index];
}

void pop_back(Array* arr) {
    if (!arr || arr->size == 0)
        return;
    arr->size--;
}

void erase(Array* arr, int index) {
    if (!arr || index < 0 || index >= arr->size)
        return;
    for (int i = index; i < arr->size - 1; ++i) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
}

void clear(Array* arr) {
    if (!arr) return;
    arr->size = 0;
}

void change(Array* arr, int index, float value) {
    if (!arr || index < 0 || index >= arr->size)
        return;
    arr->data[index] = value;
}

int length(const Array* arr) {
    return arr ? arr->size : 0;
}

void print(const Array* arr) {
    if (!arr) return;
    for (int i = 0; i < arr->size; ++i)
        cout << arr->data[i] << " ";
    cout << endl;
}
