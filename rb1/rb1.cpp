#include <stdio.h>
#include <random>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <chrono>



struct node {
    node* left, * right, * parent;

    bool isRed;
    int data;
};

#define NIL &sentinel
node sentinel = { NIL, NIL, 0, false, 0 };


node* rotateLeft(node* root, node* x) {
    node* y = x->right;

    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;

    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    else {
        root = y;
    }

    y->left = x;
    if (x != NIL) x->parent = y;
    return root;
}

node* rotateRight(node* root, node* x) {
    node* y = x->left;

    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;

    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    }
    else {
        root = y;
    }

    y->right = x;
    if (x != NIL) x->parent = y;
    return root;
}

node* insertFixup(node* root, node* x) {
    while (x != root && x->parent->isRed == true) {
        if (x->parent == x->parent->parent->left) {
            node* y = x->parent->parent->right;
            if (y->isRed == true) {
                x->parent->isRed = false;
                y->isRed = false;
                x->parent->parent->isRed = true;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->right) {
                    x = x->parent;
                    root = rotateLeft(root, x);
                }
                x->parent->isRed = false;
                x->parent->parent->isRed = true;
                root = rotateRight(root, x->parent->parent);
            }
        }
        else {
            node* y = x->parent->parent->left;
            if (y->isRed == true) {
                x->parent->isRed = false;
                y->isRed = false;
                x->parent->parent->isRed = true;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    root = rotateRight(root, x);
                }
                x->parent->isRed = false;
                x->parent->parent->isRed = true;
                root = rotateLeft(root, x->parent->parent);
            }
        }
    }
    root->isRed = false;
    return root;
}

node* _insertNode(node* root, int data) {
    node* current, * parent, * x;

    current = root;
    parent = 0;
    while (current != NIL) {
        if (data == current->data) return (current);
        parent = current;
        current = data < current->data ?
            current->left : current->right;
    }

    x = new node;
    x->data = data;
    x->parent = parent;
    x->left = NIL;
    x->right = NIL;
    x->isRed = true;

    if (parent) {
        if (data < parent->data)
            parent->left = x;
        else
            parent->right = x;
    }
    else {
        root = x;
    }

    root = insertFixup(root, x);
    return root;
}

node* deleteFixup(node* root, node* x) {
    while (x != root && x->isRed == false) {
        if (x == x->parent->left) {
            node* w = x->parent->right;
            if (w->isRed == true) {
                w->isRed = false;
                x->parent->isRed = true;
                root = rotateLeft(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->isRed == false && w->right->isRed == false) {
                w->isRed = true;
                x = x->parent;
            }
            else {
                if (w->right->isRed == false) {
                    w->left->isRed = false;
                    w->isRed = true;
                    root = rotateRight(root, w);
                    w = x->parent->right;
                }
                w->isRed = x->parent->isRed;
                x->parent->isRed = false;
                w->right->isRed = false;
                root = rotateLeft(root, x->parent);
                x = root;
            }
        }
        else {
            node* w = x->parent->left;
            if (w->isRed == true) {
                w->isRed = false;
                x->parent->isRed = true;
                root = rotateRight(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->isRed == false && w->left->isRed == false) {
                w->isRed = true;
                x = x->parent;
            }
            else {
                if (w->left->isRed == false) {
                    w->right->isRed = false;
                    w->isRed = true;
                    root = rotateLeft(root, w);
                    w = x->parent->left;
                }
                w->isRed = x->parent->isRed;
                x->parent->isRed = false;
                w->left->isRed = false;
                root = rotateRight(root, x->parent);
                x = root;
            }
        }
    }
    x->isRed = false;
    return root;
}

node* _deleteNode(node* root, node* z) {
    node* x, * y;

    if (!z || z == NIL) return root;


    if (z->left == NIL || z->right == NIL) {
        y = z;
    }
    else {
        y = z->right;
        while (y->left != NIL) y = y->left;
    }

    if (y->left != NIL)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    else
        root = x;

    if (y != z) z->data = y->data;


    if (y->isRed == false)
        root = deleteFixup(root, x);

    free(y);
    return root;
}

node* _findNode(node* root, int data) {
    node* current = root;
    while (current != NIL)
        if (data == current->data)
            return (current);
        else
            current = data < current->data ?
            current->left : current->right;
    return(0);
}

void _inorder(node* root, bool last) {
    if (root != NIL) {
        _inorder(root->left, false);

        std::cout << root->data << (last && root->right == NIL ? "\n" : " -> ");

        _inorder(root->right, last);
    }
}

struct rbTree {
private:
    node* root = NIL;

public:
    void insertNode(int data) {
        this->root = _insertNode(this->root, data);
    }

    void inorder() {
        _inorder(this->root, true);
    }

    void deleteNode(int data) {
        node* t = _findNode(this->root, data);
        if (t != NIL)
            this->root = _deleteNode(this->root, t);
    }

    node* findNode(int data) {
        return _findNode(this->root, data);
    }
    int h(node* r) {

    
        if (r->left == NIL && r->right == NIL)
            return 1;
        if (r->left != NIL && r->right == NIL)
            return h(r->left) + 1;
        if (r->left == NIL && r->right != NIL)
            return h(r->right) + 1;


        return std::max(h(r->left), h(r->right)) + 1;
    }

public:
    int h() {
        return h(this->root);
    }
};

int main() {
    rbTree tree;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 500; i++)
    {
        int t = i;

        tree.insertNode(t);
    }

        /*for (int i = 0; i < 500; i++) {
        int t = rand()%500;
        // std::cout << t << ' ';
        tree.insertNode(t);
    }*/

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    std::cout << "t" << duration.count() * 1000 << "\n";

    std::cout << '\n';
    

    std::cout << tree.h();

 
}
    
