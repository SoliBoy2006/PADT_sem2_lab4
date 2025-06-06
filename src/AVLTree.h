#pragma once
#include <queue>
#include <iostream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

/*
 * AVLTree now supports a custom comparator type Compare (default = std::less<T>).
 * Internally, all comparisons (insert/remove/search) use comp(a, b).
 * We also expose:
 *   - public: Node* FindMin() const
 *   - public: int GetHeight() const
 *   - public: void LevelOrder(std::function<void(const T&)> f) const
 */

template <class T, class Compare = std::less<T>>
class AVLTree {
public:
    struct Node {
        T value;
        Node* left;
        Node* right;
        int height;
        Node(const T& val) : value(val), left(nullptr), right(nullptr), height(1) {}
    };

private:
    Node* root;
    Compare comp;

    int getHeight(Node* node) const {
        return node ? node->height : 0;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    int getBalance(Node* node) const {
        if (!node) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        if (!node) return nullptr;
        updateHeight(node);
        int balanceFactor = getBalance(node);

        // Left-heavy
        if (balanceFactor > 1) {
            if (getBalance(node->left) >= 0) {
                return rotateRight(node);
            } else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        // Right-heavy
        if (balanceFactor < -1) {
            if (getBalance(node->right) <= 0) {
                return rotateLeft(node);
            } else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    // Insert using comp to decide direction. If equal (neither comp(a,b) nor comp(b,a)), do nothing.
    Node* insert(Node* node, const T& value) {
        if (!node) return new Node(value);

        if (comp(value, node->value)) {
            node->left = insert(node->left, value);
        } else if (comp(node->value, value)) {
            node->right = insert(node->right, value);
        } else {
            // value == node->value (no duplicates)
            return node;
        }

        return balance(node);
    }

    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const T& value) {
        if (!node) return nullptr;

        if (comp(value, node->value)) {
            node->left = remove(node->left, value);
        } else if (comp(node->value, value)) {
            node->right = remove(node->right, value);
        } else {
            // found node to remove
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                Node* minRight = findMin(node->right);
                node->value = minRight->value;
                node->right = remove(node->right, minRight->value);
            }
        }

        return balance(node);
    }

    void destroy(Node* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    void inorder(Node* node, std::function<void(const T&)> f) const {
        if (!node) return;
        inorder(node->left, f);
        f(node->value);
        inorder(node->right, f);
    }

    void preorder(Node* node, std::function<void(const T&)> f) const {
        if (!node) return;
        f(node->value);
        preorder(node->left, f);
        preorder(node->right, f);
    }

    void postorder(Node* node, std::function<void(const T&)> f) const {
        if (!node) return;
        postorder(node->left, f);
        postorder(node->right, f);
        f(node->value);
    }

public:
    AVLTree() : root(nullptr), comp(Compare()) {}
    explicit AVLTree(const Compare& comparator) : root(nullptr), comp(comparator) {}
    ~AVLTree() { destroy(root); }

    // Insert a value using the stored comparator
    void Insert(const T& value) {
        root = insert(root, value);
    }

    // Remove a value using the stored comparator
    void Remove(const T& value) {
        root = remove(root, value);
    }

    // Search/Contains: uses the stored comparator
    bool Contains(const T& value) const {
        Node* current = root;
        while (current) {
            if (!comp(value, current->value) && !comp(current->value, value)) {
                return true; // equal
            } else if (comp(value, current->value)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false;
    }

    // Overload: allow a one-off comparator for Contains (does not modify tree, just searches by that comparator)
    bool Contains(const T& value, std::function<bool(const T&, const T&)> customComp) const {
        Node* current = root;
        while (current) {
            if (!customComp(value, current->value) && !customComp(current->value, value)) {
                return true;
            } else if (customComp(value, current->value)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false;
    }

    // Public InOrder, PreOrder, PostOrder
    void InOrder(std::function<void(const T&)> f) const { inorder(root, f); }
    void PreOrder(std::function<void(const T&)> f) const { preorder(root, f); }
    void PostOrder(std::function<void(const T&)> f) const { postorder(root, f); }

    // Public LevelOrder
    void LevelOrder(std::function<void(const T&)> f) const {
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            f(current->value);
            if (current->left)  q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    // Public ReverseInOrder
    void ReverseInOrder(std::function<void(const T&)> f) const {
        // In-order but right subtree first
        std::function<void(Node*)> rev = [&](Node* node) {
            if (!node) return;
            rev(node->right);
            f(node->value);
            rev(node->left);
        };
        rev(root);
    }

    // Morris InOrder (still private-like; can expose if needed)
    void MorrisInOrder(std::function<void(const T&)> f) const {
        Node* current = root;
        while (current) {
            if (!current->left) {
                f(current->value);
                current = current->right;
            } else {
                Node* predecessor = current->left;
                while (predecessor->right && predecessor->right != current) {
                    predecessor = predecessor->right;
                }
                if (!predecessor->right) {
                    predecessor->right = current;
                    current = current->left;
                } else {
                    predecessor->right = nullptr;
                    f(current->value);
                    current = current->right;
                }
            }
        }
    }

    // Public accessor for the minimal node pointer (or nullptr if empty)
    Node* FindMin() const {
        return findMin(root);
    }

    // Public accessor for the tree's height (height of root)
    int GetHeight() const {
        return getHeight(root);
    }

    bool IsEmpty() const {
        return root == nullptr;
    }
};

