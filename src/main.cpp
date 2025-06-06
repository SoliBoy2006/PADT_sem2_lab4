#include "AVLTree.h"
#include "AVLTreeExtensions.h"
#include "AVLTreeTraversalTemplates.h"

#include <iostream>
#include <string>

int main() {
    AVLTree<int> tree;
    int choice, value;

    while (true) {
        std::cout << "\n--- AVL Tree Menu ---\n";
        std::cout << "1. Insert\n2. Remove\n3. Search\n4. Print In-Order\n5. Print Pre-Order\n";
        std::cout << "6. Map (×2)\n7. Where (x > n)\n8. Extract Subtree\n9. Compare trees\n";
        std::cout << "10. Save to template string\n11. Build from string and template\n";
        std::cout << "12. Traverse with selected order\n13. Exit\n> ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Value to insert: "; std::cin >> value;
                tree.Insert(value); break;
            case 2:
                std::cout << "Value to remove: "; std::cin >> value;
                tree.Remove(value); break;
            case 3:
                std::cout << "Value to search: "; std::cin >> value;
                std::cout << (tree.Contains(value) ? "Found\n" : "Not found\n"); break;
            case 4:
                tree.InOrder([](int v){ std::cout << v << " "; }); std::cout << "\n"; break;
            case 5:
                tree.PreOrder([](int v){ std::cout << v << " "; }); std::cout << "\n"; break;
            case 6: {
                auto mapped = Map<int, int>(tree, [](int x){ return x * 2; });
                mapped->InOrder([](int v){ std::cout << v << " "; }); std::cout << "\n";
                delete mapped; break;
            }
            case 7: {
                std::cout << "Filter x > ? "; int n; std::cin >> n;
                auto filtered = Where<int>(tree, [n](int x){ return x > n; });
                filtered->InOrder([](int v){ std::cout << v << " "; }); std::cout << "\n";
                delete filtered; break;
            }
            case 8: {
                std::cout << "Subtree key: "; int k; std::cin >> k;
                auto sub = ExtractSubtree(tree, k);
                sub->InOrder([](int v){ std::cout << v << " "; }); std::cout << "\n";
                delete sub; break;
            }
            case 9: {
                AVLTree<int> other;
                std::cin.ignore();
                std::string line;
                std::cout << "Enter values for other tree (space-separated): ";
                std::getline(std::cin, line);

                auto values = ParseValuesFromString<int>(line);
                for (int x : values)
                    other.Insert(x);

                std::cout << (Equals(tree, other) ? "Trees are equal.\n" : "Trees are NOT equal.\n");
            }
            case 10: {
                std::string pattern;
                std::cout << "Pattern (KLP, LKP, LPK): ";
                std::cin >> pattern;
                try {
                    std::string result = ToStringTemplate(tree, pattern);
                    std::cout << "Serialized: " << result << "\n";
                } catch (std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 11: {
                std::cin.ignore();
                std::string str, pattern;
                std::cout << "Values: "; std::getline(std::cin, str);
                std::cout << "Pattern (KLP, LKP, LPK): "; std::getline(std::cin, pattern);
                try {
                    auto values = ParseValuesFromString<int>(str);
                    auto built = FromOrderTemplate<int>(values, pattern);
                    built->InOrder([](int v){ std::cout << v << " "; }); std::cout << "\n";
                    delete built;
                } catch (std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 13: {
                std::string order;
                std::cout << "Traversal (KLP, LKP, LPK): ";
                std::cin >> order;
                TraversalOrder ord;
                if (order == "KLP") ord = TraversalOrder::PreOrder;
                else if (order == "LKP") ord = TraversalOrder::InOrder;
                else if (order == "LPK") ord = TraversalOrder::PostOrder;
                else { std::cout << "Unknown\n"; break; }
                Traverse<int>(tree, ord, [](int x){ std::cout << x << " "; });
                std::cout << "\n"; break;
            }
            case 14: return 0;
        }
    }
}
