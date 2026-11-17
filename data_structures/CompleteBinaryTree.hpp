#pragma once

#include <iostream>

struct TreeNode {
    float data;
    TreeNode* left;
    TreeNode* right;
};

struct CompleteBinaryTree {
    TreeNode* root;
    int size;
};

void initTree(CompleteBinaryTree* tree);
void destroyTree(CompleteBinaryTree* tree);
void clear(CompleteBinaryTree* tree);

void insert(CompleteBinaryTree* tree, float value);
bool search(const CompleteBinaryTree* tree, float value);
bool isComplete(const CompleteBinaryTree* tree);
bool remove(CompleteBinaryTree* tree, float value);

int size(const CompleteBinaryTree* tree);
bool isEmpty(const CompleteBinaryTree* tree);

void printPreOrder(const CompleteBinaryTree* tree);
void printInOrder(const CompleteBinaryTree* tree);
void printPostOrder(const CompleteBinaryTree* tree);
void printBFS(const CompleteBinaryTree* tree);
void printTree(const CompleteBinaryTree* tree);

bool findSuccessor(const CompleteBinaryTree* tree, float value, float& successor);
bool findPredecessor(const CompleteBinaryTree* tree, float value, float& predecessor);
