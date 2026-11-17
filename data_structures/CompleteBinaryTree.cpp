#include "CompleteBinaryTree.hpp"

#include <iostream>
#include <cmath>
#include <queue>
#include <vector>
#include <string>

using namespace std;

namespace {
    const float EPS = 1e-6f;

    void destroySubtree(TreeNode* node) {
        if (!node) return;
        destroySubtree(node->left);
        destroySubtree(node->right);
        delete node;
    }

    void preOrderHelper(TreeNode* node) {
        if (!node) return;
        cout << node->data << " ";
        preOrderHelper(node->left);
        preOrderHelper(node->right);
    }

    void inOrderHelper(TreeNode* node, vector<float>& elements) {
        if (!node) return;
        inOrderHelper(node->left, elements);
        elements.push_back(node->data);
        inOrderHelper(node->right, elements);
    }

    void postOrderHelper(TreeNode* node) {
        if (!node) return;
        postOrderHelper(node->left);
        postOrderHelper(node->right);
        cout << node->data << " ";
    }

    void printTreeHelper(TreeNode* node, const string& prefix, bool isLeft) {
        if (!node) return;

        cout << prefix;
        cout << (isLeft ? "├── " : "└── ");
        cout << node->data << endl;

        string nextPrefix = prefix + (isLeft ? "│   " : "    ");
        if (node->left && node->right) {
            printTreeHelper(node->left, nextPrefix, true);
            printTreeHelper(node->right, nextPrefix, false);
        } else if (node->left) {
            printTreeHelper(node->left, nextPrefix, false);
        } else if (node->right) {
            printTreeHelper(node->right, nextPrefix, false);
        }
    }
}

void initTree(CompleteBinaryTree* tree) {
    if (!tree) return;
    tree->root = nullptr;
    tree->size = 0;
}

void destroyTree(CompleteBinaryTree* tree) {
    if (!tree) return;
    destroySubtree(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}

void clear(CompleteBinaryTree* tree) {
    destroyTree(tree);
}

int size(const CompleteBinaryTree* tree) {
    return tree ? tree->size : 0;
}

bool isEmpty(const CompleteBinaryTree* tree) {
    return !tree || tree->root == nullptr;
}

void insert(CompleteBinaryTree* tree, float value) {
    if (!tree) return;

    TreeNode* newNode = new TreeNode{value, nullptr, nullptr};
    if (tree->root == nullptr) {
        tree->root = newNode;
        tree->size = 1;
        return;
    }

    queue<TreeNode*> q;
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

bool search(const CompleteBinaryTree* tree, float value) {
    if (!tree || tree->root == nullptr) return false;

    queue<TreeNode*> q;
    q.push(tree->root);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        if (fabs(current->data - value) < EPS) {
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

bool isComplete(const CompleteBinaryTree* tree) {
    if (!tree || tree->root == nullptr) return true;

    queue<TreeNode*> q;
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

bool remove(CompleteBinaryTree* tree, float value) {
    if (!tree || tree->root == nullptr) return false;

    if (tree->root->left == nullptr && tree->root->right == nullptr) {
        if (fabs(tree->root->data - value) < EPS) {
            delete tree->root;
            tree->root = nullptr;
            tree->size = 0;
            return true;
        }
        return false;
    }

    queue<TreeNode*> q;
    q.push(tree->root);

    TreeNode* targetNode = nullptr;
    TreeNode* lastNode = nullptr;
    TreeNode* lastParent = nullptr;

    queue<pair<TreeNode*, TreeNode*>> parentQueue;
    parentQueue.push({tree->root, nullptr});

    while (!parentQueue.empty()) {
        auto [current, parent] = parentQueue.front();
        parentQueue.pop();

        if (fabs(current->data - value) < EPS && targetNode == nullptr) {
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

void printPreOrder(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        cout << "(empty tree)" << endl;
        return;
    }
    preOrderHelper(tree->root);
    cout << endl;
}

void printInOrder(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        cout << "(empty tree)" << endl;
        return;
    }
    vector<float> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        cout << elements[i] << (i + 1 < elements.size() ? " " : "");
    }
    cout << endl;
}

void printPostOrder(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        cout << "(empty tree)" << endl;
        return;
    }
    postOrderHelper(tree->root);
    cout << endl;
}

void printBFS(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        cout << "(empty tree)" << endl;
        return;
    }
    queue<TreeNode*> q;
    q.push(tree->root);
    bool first = true;
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        if (!first) cout << " ";
        cout << current->data;
        first = false;
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    cout << endl;
}

void printTree(const CompleteBinaryTree* tree) {
    if (!tree || !tree->root) {
        cout << "(empty tree)" << endl;
        return;
    }
    cout << tree->root->data << endl;
    if (tree->root->left && tree->root->right) {
        printTreeHelper(tree->root->left, "", true);
        printTreeHelper(tree->root->right, "", false);
    } else if (tree->root->left) {
        printTreeHelper(tree->root->left, "", false);
    } else if (tree->root->right) {
        printTreeHelper(tree->root->right, "", false);
    }
}

bool findSuccessor(const CompleteBinaryTree* tree, float value, float& successor) {
    if (!tree || !tree->root) return false;
    vector<float> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        if (fabs(elements[i] - value) < EPS) {
            if (i + 1 < elements.size()) {
                successor = elements[i + 1];
                return true;
            }
            return false;
        }
    }
    return false;
}

bool findPredecessor(const CompleteBinaryTree* tree, float value, float& predecessor) {
    if (!tree || !tree->root) return false;
    vector<float> elements;
    inOrderHelper(tree->root, elements);
    for (size_t i = 0; i < elements.size(); ++i) {
        if (fabs(elements[i] - value) < EPS) {
            if (i > 0) {
                predecessor = elements[i - 1];
                return true;
            }
            return false;
        }
    }
    return false;
}
