#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <utility>

template <typename T>
struct TreeNode {
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;
};

template <typename T>
struct CompleteBinaryTree {
    TreeNode<T>* root;
    int size;
};

template <typename T>
void destroySubtree(TreeNode<T>* node) {
    if (!node) return;
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

template <typename T>
void preOrderHelper(TreeNode<T>* node) {
    if (!node) return;
    std::cout << node->data << " ";
    preOrderHelper(node->left);
    preOrderHelper(node->right);
}

template <typename T>
void inOrderHelper(TreeNode<T>* node, std::vector<T>& elements) {
    if (!node) return;
    inOrderHelper(node->left, elements);
    elements.push_back(node->data);
    inOrderHelper(node->right, elements);
}

template <typename T>
void postOrderHelper(TreeNode<T>* node) {
    if (!node) return;
    postOrderHelper(node->left);
    postOrderHelper(node->right);
    std::cout << node->data << " ";
}

template <typename T>
void printTreeHelper(TreeNode<T>* node, const std::string& prefix, bool isLeft) {
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLeft ? "├── " : "└── ");
    std::cout << node->data << std::endl;

    std::string nextPrefix = prefix + (isLeft ? "│   " : "    ");
    if (node->left && node->right) {
        printTreeHelper(node->left, nextPrefix, true);
        printTreeHelper(node->right, nextPrefix, false);
    } else if (node->left) {
        printTreeHelper(node->left, nextPrefix, false);
    } else if (node->right) {
        printTreeHelper(node->right, nextPrefix, false);
    }
}

template <typename T>
void initTree(CompleteBinaryTree<T>* tree) {
    if (!tree) return;
    tree->root = nullptr;
    tree->size = 0;
}

template <typename T>
void destroyTree(CompleteBinaryTree<T>* tree) {
    if (!tree) return;
    destroySubtree(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}

template <typename T>
void clear(CompleteBinaryTree<T>* tree) {
    destroyTree(tree);
}

template <typename T>
int size(const CompleteBinaryTree<T>* tree) {
    return tree ? tree->size : 0;
}

template <typename T>
bool isEmpty(const CompleteBinaryTree<T>* tree) {
    return !tree || tree->root == nullptr;
}

template <typename T>
void insert(CompleteBinaryTree<T>* tree, const T& value) {
    if (!tree) return;

    TreeNode<T>* newNode = new TreeNode<T>{value, nullptr, nullptr};
    if (tree->root == nullptr) {
        tree->root = newNode;
        tree->size = 1;
        return;
    }

    std::queue<TreeNode<T>*> q;
    q.push(tree->root);

    while (!q.empty()) {
        TreeNode<T>* current = q.front();
        q.pop();

        if (current->left == nullptr) {
            current->left = newNode;
            tree->size++;
            return;
        } else {
            q.push(current->left);
        }

        if (current->right == nullptr) {
            current->right = newNode;
            tree->size++;
            return;
        } else {
            q.push(current->right);
        }
    }
}

template <typename T>
bool search(const CompleteBinaryTree<T>* tree, const T& value) {
    if (!tree || tree->root == nullptr) return false;

    std::queue<TreeNode<T>*> q;
    q.push(tree->root);

    while (!q.empty()) {
        TreeNode<T>* current = q.front();
        q.pop();

        if (current->data == value) {
            return true;
        }
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
    return false;
}

template <typename T>
bool isComplete(const CompleteBinaryTree<T>* tree) {
    if (!tree || tree->root == nullptr) return true;

    std::queue<TreeNode<T>*> q;
    q.push(tree->root);
    bool mustBeLeaf = false;

    while (!q.empty()) {
        TreeNode<T>* current = q.front();
        q.pop();

        if (current->left != nullptr) {
            if (mustBeLeaf) return false;
            q.push(current->left);
        } else {
            mustBeLeaf = true;
        }

        if (current->right != nullptr) {
            if (mustBeLeaf) return false;
            q.push(current->right);
        } else {
            mustBeLeaf = true;
        }
    }

    return true;
}

template <typename T>
bool remove(CompleteBinaryTree<T>* tree, const T& value) {
    if (!tree || tree->root == nullptr) return false;

    if (tree->root->left == nullptr && tree->root->right == nullptr) {
        if (tree->root->data == value) {
            delete tree->root;
            tree->root = nullptr;
            tree->size = 0;
            return true;
        }
        return false;
    }

    TreeNode<T>* targetNode = nullptr;
    TreeNode<T>* lastNode = nullptr;
    TreeNode<T>* lastParent = nullptr;

    std::queue<std::pair<TreeNode<T>*, TreeNode<T>*>> parentQueue;
    parentQueue.push({tree->root, nullptr});

    while (!parentQueue.empty()) {
        auto [current, parent] = parentQueue.front();
        parentQueue.pop();

        if (current->data == value && targetNode == nullptr) {
            targetNode = current;
        }

        lastNode = current;
        lastParent = parent;

        if (current->left != nullptr) {
            parentQueue.push({current->left, current});
        }
        if (current->right != nullptr) {
            parentQueue.push({current->right, current});
        }
    }

    if (targetNode == nullptr) {
        return false;
    }

    targetNode->data = lastNode->data;

    if (lastParent != nullptr) {
        if (lastParent->right == lastNode) {
            lastParent->right = nullptr;
        } else if (lastParent->left == lastNode) {
            lastParent->left = nullptr;
        }
    }

    delete lastNode;
    tree->size--;
    return true;
}

template <typename T>
void printPreOrder(const CompleteBinaryTree<T>* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    preOrderHelper(tree->root);
    std::cout << std::endl;
}

template <typename T>
void printInOrder(const CompleteBinaryTree<T>* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    std::vector<T> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i] << (i + 1 < elements.size() ? " " : "");
    }
    std::cout << std::endl;
}

template <typename T>
void printPostOrder(const CompleteBinaryTree<T>* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    postOrderHelper(tree->root);
    std::cout << std::endl;
}

template <typename T>
void printBFS(const CompleteBinaryTree<T>* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    std::queue<TreeNode<T>*> q;
    q.push(tree->root);
    bool first = true;
    while (!q.empty()) {
        TreeNode<T>* current = q.front();
        q.pop();
        if (!first) std::cout << " ";
        std::cout << current->data;
        first = false;
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    std::cout << std::endl;
}

template <typename T>
void printTree(const CompleteBinaryTree<T>* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    std::cout << tree->root->data << std::endl;
    if (tree->root->left && tree->root->right) {
        printTreeHelper(tree->root->left, "", true);
        printTreeHelper(tree->root->right, "", false);
    } else if (tree->root->left) {
        printTreeHelper(tree->root->left, "", false);
    } else if (tree->root->right) {
        printTreeHelper(tree->root->right, "", false);
    }
}

template <typename T>
bool findSuccessor(const CompleteBinaryTree<T>* tree, const T& value, T& successor) {
    if (!tree || !tree->root) return false;
    std::vector<T> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        if (elements[i] == value) {
            if (i + 1 < elements.size()) {
                successor = elements[i + 1];
                return true;
            }
            return false;
        }
    }
    return false;
}

template <typename T>
bool findPredecessor(const CompleteBinaryTree<T>* tree, const T& value, T& predecessor) {
    if (!tree || !tree->root) return false;
    std::vector<T> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        if (elements[i] == value) {
            if (i > 0) {
                predecessor = elements[i - 1];
                return true;
            }
            return false;
        }
    }
    return false;
}
