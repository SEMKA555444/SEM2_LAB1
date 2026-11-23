#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <utility>

struct TreeNode {
    double data;
    TreeNode* left;
    TreeNode* right;
};

struct CompleteBinaryTree {
    TreeNode* root;
    int size;
};

inline void destroySubtree(TreeNode* node) {
    if (!node) return;
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

inline void preOrderHelper(TreeNode* node) {
    if (!node) return;
    std::cout << node->data << " ";
    preOrderHelper(node->left);
    preOrderHelper(node->right);
}

inline void inOrderHelper(TreeNode* node, std::vector<double>& elements) {
    if (!node) return;
    inOrderHelper(node->left, elements);
    elements.push_back(node->data);
    inOrderHelper(node->right, elements);
}

inline void postOrderHelper(TreeNode* node) {
    if (!node) return;
    postOrderHelper(node->left);
    postOrderHelper(node->right);
    std::cout << node->data << " ";
}

inline void printTreeHelper(TreeNode* node, const std::string& prefix, bool isLeft) {
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

inline void initTree(CompleteBinaryTree* tree) {
    if (!tree) return;
    tree->root = nullptr;
    tree->size = 0;
}

inline void destroyTree(CompleteBinaryTree* tree) {
    if (!tree) return;
    destroySubtree(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}

inline void clear(CompleteBinaryTree* tree) {
    destroyTree(tree);
}

inline int size(const CompleteBinaryTree* tree) {
    return tree ? tree->size : 0;
}

inline bool isEmpty(const CompleteBinaryTree* tree) {
    return !tree || tree->root == nullptr;
}

inline void insert(CompleteBinaryTree* tree, double value) {
    if (!tree) return;

    TreeNode* newNode = new TreeNode{value, nullptr, nullptr};
    if (tree->root == nullptr) {
        tree->root = newNode;
        tree->size = 1;
        return;
    }

    std::queue<TreeNode*> q;
    q.push(tree->root);

    while (!q.empty()) {
        TreeNode* current = q.front();
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

inline bool search(const CompleteBinaryTree* tree, double value) {
    if (!tree || tree->root == nullptr) return false;

    std::queue<TreeNode*> q;
    q.push(tree->root);

    while (!q.empty()) {
        TreeNode* current = q.front();
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

inline bool isComplete(const CompleteBinaryTree* tree) {
    if (!tree || tree->root == nullptr) return true;

    std::queue<TreeNode*> q;
    q.push(tree->root);
    bool mustBeLeaf = false;

    while (!q.empty()) {
        TreeNode* current = q.front();
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

inline bool remove(CompleteBinaryTree* tree, double value) {
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

    TreeNode* targetNode = nullptr;
    TreeNode* lastNode = nullptr;
    TreeNode* lastParent = nullptr;

    std::queue<std::pair<TreeNode*, TreeNode*>> parentQueue;
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

inline void printPreOrder(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    preOrderHelper(tree->root);
    std::cout << std::endl;
}

inline void printInOrder(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    std::vector<double> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i] << (i + 1 < elements.size() ? " " : "");
    }
    std::cout << std::endl;
}

inline void printPostOrder(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    postOrderHelper(tree->root);
    std::cout << std::endl;
}

inline void printBFS(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        std::cout << "(empty tree)" << std::endl;
        return;
    }
    std::queue<TreeNode*> q;
    q.push(tree->root);
    bool first = true;
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        if (!first) std::cout << " ";
        std::cout << current->data;
        first = false;
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    std::cout << std::endl;
}

inline void printTree(const CompleteBinaryTree* tree) {
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

inline bool findSuccessor(const CompleteBinaryTree* tree, double value, double& successor) {
    if (!tree || !tree->root) return false;
    std::vector<double> elements;
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

inline bool findPredecessor(const CompleteBinaryTree* tree, double value, double& predecessor) {
    if (!tree || !tree->root) return false;
    std::vector<double> elements;
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