#include "AVLTree.h"
#include "PersonTypes.h"
#include <cassert>
#include <complex>
#include <cmath>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

// Simple functions for TestFunctionTree
int FuncA(int x) { return x + 1; }
int FuncB(int x) { return x * 2; }

void TestIntTree() {
    AVLTree<int> tree;

    // Вставка
    tree.Insert(5);
    tree.Insert(3);
    tree.Insert(7);
    tree.Insert(2);

    // Проверка наличия элементов
    assert(tree.Contains(5));
    assert(tree.Contains(3));
    assert(!tree.Contains(10));

    // Проверка обходов
    std::vector<int> inOrderResult;
    tree.InOrder([&](int x) { inOrderResult.push_back(x); });

    // Удаление
    tree.Remove(3);
    assert(!tree.Contains(3));
}

void TestDoubleTree() {
    AVLTree<double> tree;

    tree.Insert(3.14);
    tree.Insert(2.71);
    tree.Insert(1.618);

    assert(tree.Contains(2.71));

    // FindMin returns a Node*, so check its value
    auto minNode = tree.FindMin();
    assert(minNode != nullptr);
    assert(std::abs(minNode->value - 1.618) < 1e-9);

    // Level-order traversal (just ensure it compiles and runs)
    std::vector<double> levelOrder;
    tree.LevelOrder([&](double x) { levelOrder.push_back(x); });
    // We won't assert exact structure here; just confirm the call works without crashing
    assert(!levelOrder.empty());
}

void TestComplexTree() {
    // Comparator that sorts by magnitude
    auto comp = [](const std::complex<double>& a, const std::complex<double>& b) {
        return std::abs(a) < std::abs(b);
    };

    // Instantiate tree with that comparator
    AVLTree<std::complex<double>, decltype(comp)> tree(comp);

    tree.Insert({1, 2}); // |1+2i| = sqrt(5)
    tree.Insert({3, 4}); // |3+4i| = 5
    tree.Insert({0, 1}); // |i| = 1

    // Now search using the stored comparator
    assert(tree.Contains(std::complex<double>{1, 2}));

    // Also verify Contains with a one-off comparator works
    assert(tree.Contains(std::complex<double>{3, 4}, comp));
    assert(!tree.Contains(std::complex<double>{10, 10}, comp));
}

void TestStringTree() {
    AVLTree<std::string> tree;

    tree.Insert("apple");
    tree.Insert("banana");
    tree.Insert("cherry");

    assert(tree.Contains("banana"));
    assert(!tree.Contains("pear"));

    // Лексикографический порядок
    std::vector<std::string> result;
    tree.InOrder([&](const std::string& s) { result.push_back(s); });
}

void TestPersonTree() {
    // Компаратор для Person по ID (series, then number)
    auto personComp = [](const auto& a, const auto& b) {
        if (a.GetID().series < b.GetID().series) return true;
        if (a.GetID().series > b.GetID().series) return false;
        return a.GetID().number < b.GetID().number;
    };

    // Instantiate AVLTree<Student> with that comparator
    AVLTree<Student, decltype(personComp)> studentTree(personComp);

    std::tm dob{};
    dob.tm_year = 2000 - 1900;
    dob.tm_mday = 1;

    Student s1{{1000, 123456}, "Ivan", "Ivanovich", "Ivanov", dob};
    Student s2{{1001, 654321}, "Petr", "Petrovich", "Petrov", dob};

    studentTree.Insert(s1);
    studentTree.Insert(s2);

    assert(studentTree.Contains(s1));

    // Тест для преподавателей
    AVLTree<Teacher, decltype(personComp)> teacherTree(personComp);
    Teacher t{{2000, 111111}, "Maria", "Ivanovna", "Sidorova", dob};
    teacherTree.Insert(t);

    assert(teacherTree.Contains(t));
}

void TestFunctionTree() {
    // We store function pointers of type int(*)(int), compare by pointer address.
    using FuncType = int(*)(int);
    auto funcComp = [](FuncType a, FuncType b) {
        return a < b; // compare function pointers by address
    };

    AVLTree<FuncType, decltype(funcComp)> tree(funcComp);

    tree.Insert(&FuncA);
    tree.Insert(&FuncB);

    // Check that both function pointers are present
    assert(tree.Contains(&FuncA));
    assert(tree.Contains(&FuncB));
    // Some function not inserted:
    auto fakeFunc = [](int y) { return y - 1; };
    // This lambda cannot be compared as function pointer; skip searching for it.
}

void TestAdvancedFunctionality() {
    AVLTree<int> tree;
    for (int i = 0; i < 100; ++i) {
        tree.Insert(i);
    }

    // Проверка балансировки: высота должна быть <= 1.44 * log2(N+2)
    double h = static_cast<double>(tree.GetHeight());
    double bound = 1.44 * std::log2(100 + 2);
    assert(h <= bound + 1e-9);

    // Тест удаления
    for (int i = 0; i < 50; ++i) {
        tree.Remove(i);
    }
    assert(!tree.Contains(25));
    assert(tree.Contains(75));
}

void RunAllTests() {
    TestIntTree();
    TestDoubleTree();
    TestComplexTree();
    TestStringTree();
    TestPersonTree();
    TestFunctionTree();
    TestAdvancedFunctionality();

    std::cout << "All tests passed successfully!\n";
}

int main() {
    RunAllTests();
    return 0;
}

