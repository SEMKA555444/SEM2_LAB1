#pragma once

struct Array {
    float* data;
    int size;
    int capacity;
};

void create(Array* arr, int initialCapacity = 4);
void destroy(Array* arr);
void reserve(Array* arr, int newCapacity);
void push_back(Array* arr, float value);
void insert(Array* arr, int index, float value);
float get(const Array* arr, int index);
void pop_back(Array* arr);
void erase(Array* arr, int index);
void clear(Array* arr);
void change(Array* arr, int index, float value);
int length(const Array* arr);
void print(const Array* arr);
