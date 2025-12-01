#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    double value;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(double val)
        : value(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

Color getColor(Node* node) {
    if (node == nullptr) return BLACK;
    return node->color;
}

void setColor(Node* node, Color c) {
    if (node != nullptr) node->color = c;
}

Node* leftRotate(Node* root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
    return root;
}

Node* rightRotate(Node* root, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;
    x->parent = y->parent;

    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
    return root;
}

Node* fixInsert(Node* root, Node* z) {
    while (z != root && getColor(z->parent) == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* uncle = z->parent->parent->right;
            if (getColor(uncle) == RED) {
                setColor(z->parent, BLACK);
                setColor(uncle, BLACK);
                setColor(z->parent->parent, RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    root = leftRotate(root, z);
                }
                setColor(z->parent, BLACK);
                setColor(z->parent->parent, RED);
                root = rightRotate(root, z->parent->parent);
            }
        } else {
            Node* uncle = z->parent->parent->left;
            if (getColor(uncle) == RED) {
                setColor(z->parent, BLACK);
                setColor(uncle, BLACK);
                setColor(z->parent->parent, RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    root = rightRotate(root, z);
                }
                setColor(z->parent, BLACK);
                setColor(z->parent->parent, RED);
                root = leftRotate(root, z->parent->parent);
            }
        }
    }
    setColor(root, BLACK);
    return root;
}

Node* insert(Node* root, double value) {
    Node* newNode = new Node(value);
    Node* parent = nullptr;
    Node* current = root;

    while (current != nullptr) {
        parent = current;
        if (newNode->value < current->value)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;
    if (parent == nullptr) {
        root = newNode;
    } else if (newNode->value < parent->value) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    root = fixInsert(root, newNode);
    return root;
}

void inorder(Node* node) {
    if (node != nullptr) {
        inorder(node->left);
        cout << node->value << "(" << (node->color == RED ? "R" : "B") << ") ";
        inorder(node->right);
    }
}

void preorder(Node* node) {
    if (node != nullptr) {
        cout << node->value << "(" << (node->color == RED ? "R" : "B") << ") ";
        preorder(node->left);
        preorder(node->right);
    }
}

double sumOfLeaves(Node* node) {
    if (node == nullptr) return 0.0;
    if (node->left == nullptr && node->right == nullptr)
        return node->value;
    return sumOfLeaves(node->left) + sumOfLeaves(node->right);
}

pair<int, double> countAndSum(Node* node) {
    if (node == nullptr)
        return {0, 0.0};

    auto left = countAndSum(node->left);
    auto right = countAndSum(node->right);

    int totalNodes = 1 + left.first + right.first;
    double totalSum = node->value + left.second + right.second;
    return {totalNodes, totalSum};
}

double average(Node* root) {
    auto [count, sum] = countAndSum(root);
    return (count > 0) ? sum / count : 0.0;
}

void freeTree(Node* node) {
    if (node != nullptr) {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Node* root = nullptr;
    int choice;
    double value;

    while (true) {
        cout << "\n=== Красно-чёрное дерево ===\n";
        cout << "1. Вставить элемент\n";
        cout << "2. Прямой обход\n";
        cout << "3. Симметричный обход\n";
        cout << "4. Сумма значений листьев\n";
        cout << "5. Среднее арифметическое всех узлов\n";
        cout << "0. Выход\n";
        cout << "Выберите режим: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Введите вещественное число: ";
                cin >> value;
                root = insert(root, value);
                cout << "Элемент " << value << " вставлен.\n";
                break;

            case 2:
                cout << "Прямой обход: ";
                preorder(root);
                cout << "\n";
                break;

            case 3:
                cout << "Симметричный обход: ";
                inorder(root);
                cout << "\n";
                break;

            case 4: {
                double sum = sumOfLeaves(root);
                cout << "Сумма листьев: " << sum << "\n";
                break;
            }

            case 5: {
                double avg = average(root);
                cout << fixed << setprecision(4);
                cout << "Среднее арифметическое: " << avg << "\n";
                break;
            }

            case 0:
                cout << "Завершение работы.\n";
                freeTree(root);
                return 0;

            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}