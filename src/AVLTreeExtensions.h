#pragma once
#include "AVLTree.h"
#include <functional>
#include <vector>

template <class T, class R>
AVLTree<R>* Map(const AVLTree<T>& tree, std::function<R(const T&)> func) {
    AVLTree<R>* result = new AVLTree<R>();
    tree.InOrder([&](const T& value) {
        result->Insert(func(value));
    });
    return result;
}

template <class T>
AVLTree<T>* Where(const AVLTree<T>& tree, std::function<bool(const T&)> predicate) {
    AVLTree<T>* result = new AVLTree<T>();
    tree.InOrder([&](const T& value) {
        if (predicate(value)) result->Insert(value);
    });
    return result;
}

template <class T, class R>
R Reduce(const AVLTree<T>& tree, std::function<R(const R&, const T&)> func, R initial) {
    tree.InOrder([&](const T& val) {
        initial = func(initial, val);
    });
    return initial;
}

template <class T>
AVLTree<T>* ExtractSubtree(const AVLTree<T>& tree, const T& key) {
    AVLTree<T>* result = new AVLTree<T>();
    bool found = false;
    tree.PreOrder([&](const T& value) {
        if (found) result->Insert(value);
        if (value == key) {
            result->Insert(value);
            found = true;
        }
    });
    return result;
}

template <class T>
bool Equals(const AVLTree<T>& a, const AVLTree<T>& b) {
    std::vector<T> va, vb;
    a.InOrder([&](const T& v) { va.push_back(v); });
    b.InOrder([&](const T& v) { vb.push_back(v); });
    return va == vb;
}