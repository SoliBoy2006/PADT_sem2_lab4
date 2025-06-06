#pragma once
#include "AVLTree.h"
#include "AVLTreeExtensions.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class TraversalOrder { InOrder, PreOrder, PostOrder };

template <class T>
void Traverse(const AVLTree<T>& tree, TraversalOrder order, std::function<void(const T&)> f) {
    switch (order) {
        case TraversalOrder::InOrder: tree.InOrder(f); break;
        case TraversalOrder::PreOrder: tree.PreOrder(f); break;
        case TraversalOrder::PostOrder: tree.PostOrder(f); break;
    }
}

template <class T>
std::string ToStringTemplate(const AVLTree<T>& tree, const std::string& pattern) {
    std::ostringstream out;
    if (pattern == "KLP")
        tree.PreOrder([&](const T& v){ out << v << " "; });
    else if (pattern == "LKP")
        tree.InOrder([&](const T& v){ out << v << " "; });
    else if (pattern == "LPK")
        tree.PostOrder([&](const T& v){ out << v << " "; });
    else
        throw std::invalid_argument("Unsupported pattern");
    return out.str();
}

template <class T>
AVLTree<T>* FromOrderTemplate(const std::vector<T>& values, const std::string& pattern) {
    AVLTree<T>* tree = new AVLTree<T>();
    if (pattern == "KLP") {
        for (const T& val : values) tree->Insert(val);
    } else if (pattern == "LKP") {
        std::function<void(int, int)> build = [&](int l, int r) {
            if (l > r) return;
            int m = (l + r) / 2;
            tree->Insert(values[m]);
            build(l, m - 1);
            build(m + 1, r);
        };
        build(0, values.size() - 1);
    } else if (pattern == "LPK") {
        for (auto it = values.rbegin(); it != values.rend(); ++it)
            tree->Insert(*it);
    } else {
        delete tree;
        throw std::invalid_argument("Unsupported pattern");
    }
    return tree;
}

template <class T>
std::vector<T> ParseValuesFromString(const std::string& str) {
    std::istringstream iss(str);
    std::vector<T> values;
    T val;
    while (iss >> val) values.push_back(val);
    return values;
}

template <class T>
bool IsSameTree(const AVLTree<T>& a, const AVLTree<T>& b) {
    std::vector<T> va, vb;
    a.PreOrder([&](const T& v) { va.push_back(v); });
    b.PreOrder([&](const T& v) { vb.push_back(v); });
    return va == vb;
}

template <class T>
bool HasSubtree(const AVLTree<T>& tree, const AVLTree<T>& sub) {
    std::vector<T> values;
    tree.PreOrder([&](const T& v) { values.push_back(v); });

    for (const T& rootCandidate : values) {
        auto candidate = ExtractSubtree(tree, rootCandidate);
        if (IsSameTree(*candidate, sub)) {
            delete candidate;
            return true;
        }
        delete candidate;
    }
    return false;
}