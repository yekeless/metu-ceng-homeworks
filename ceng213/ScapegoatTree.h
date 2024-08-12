#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod
{
    preorder,
    inorder,
    postorder
};

template <class T>
class ScapegoatTree
{
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp = inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    void helper_cc(Node<T> *&root, const Node<T> *rhs)
    {
        if (rhs == NULL)
        {
            root = NULL;
            return;
        }
        else
        {
            root = new Node<T>;
            root->element = rhs->element;
            helper_cc(root->left, rhs->left);
            helper_cc(root->right, rhs->right);
        }
    }
    void destructor(Node<T> *&root)
    {
        if (root == NULL)
        {
            return;
        }
        else
        {
            destructor(root->left);
            destructor(root->right);
            delete root;
            root = NULL;
        }
    }
    int height(const Node<T> *root) const
    {
        int left_height, right_height;

        if (root == NULL)
        {
            return -1;
        }
        else
        {
            left_height = height(root->left);
            right_height = height(root->right);
            if (left_height > right_height)
            {
                return 1 + left_height;
            }
            else
            {
                return 1 + right_height;
            }
        }
    }
    int helper_size(Node<T> *root) const
    {
        if (root == NULL)
        {
            return 0;
        }
        else
        {
            return 1 + helper_size(root->right) + helper_size(root->left);
        }
    }
    const T &getMin(Node<T> *root) const
    {
        Node<T> *curr = root;
        while (curr->left != NULL)
        {
            curr = curr->left;
        }
        return curr->element;
    }
    const T &getMax(Node<T> *root) const
    {
        Node<T> *curr = root;
        while (curr->right != NULL)
        {
            curr = curr->right;
        }
        return curr->element;
    }
    Node<T> *helper_next(Node<T> *root, const T &element) const
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            if (root->element <= element)
            {
                return helper_next(root->right, element);
            }
            else if (root->element > element)
            {
                Node<T> *temp = helper_next(root->left, element);
                if (temp != NULL && temp->element < root->element)
                {
                    return temp;
                }
                else
                {
                    return root;
                }
            }
        }
    }
    Node<T> *get(Node<T> *root, const T &element) const
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            if (root->element == element)
            {
                return root;
            }
            else
            {
                if (root->element < element)
                {
                    return get(root->right, element);
                }
                else if (root->element > element)
                {
                    return get(root->left, element);
                }
            }
        }
    }
    void make_array(Node<T> *root, T *arr, int &index)
    {
        if (root == NULL)
        {
        }
        else
        {
            make_array(root->left, arr, index);
            arr[index] = root->element;
            index++;
            make_array(root->right, arr, index);
        }
    }
    Node<T> *construct(T *array, int start, int end)
    {
        if (start > end)
        {
            return NULL;
        }
        else if (end - start < 0)
        {
            return NULL;
        }
        else
        {
            Node<T> *root = new Node<T>;
            int mid = (start + end) / 2;
            root->element = array[mid];
            root->left = construct(array, start, mid - 1);
            root->right = construct(array, mid + 1, end);
            return root;
        }
    }
    Node<T> *Ceil(Node<T> *root, const T &element) const
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            if (root->element == element)
            {
                return root;
            }
            else if (root->element < element)
            {
                return Ceil(root->right, element);
            }
            else if (root->element > element)
            {
                Node<T> *temp = Ceil(root->left, element);
                if (temp != NULL && temp->element < root->element)
                {
                    return temp;
                }
                else
                {
                    return root;
                }
            }
        }
    }
    Node<T> *Floor(Node<T> *root, const T &element) const
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            if (root->element > element)
            {
                return Floor(root->left, element);
            }
            else if (root->element < element)
            {
                Node<T> *temp = Floor(root->right, element);
                if (temp != NULL && temp->element > root->element)
                {
                    return temp;
                }
                else
                {
                    return root;
                }
            }
            else if (root->element == element)
            {
                return root;
            }
        }
    }
    bool helper_insert(Node<T> *&root, const T &element)
    {
        if (root == NULL)
        {
            root = new Node<T>;
            root->element = element;
            root->right = NULL;
            root->left = NULL;
            upperBound++;
            return true;
        }
        else
        {
            if (root->element == element)
            {
                return false;
            }
            else if (root->element < element)
            {
                return helper_insert(root->right, element);
            }
            else if (root->element > element)
            {
                return helper_insert(root->left, element);
            }
        }
    }
    bool is_skape(Node<T> *root, Node<T> *child)
    {
        if (root == NULL || child == NULL)
        {
            return false;
        }
        else
        {
            int rootsize = helper_size(root);
            int childsize = helper_size(child);
            if (3 * childsize > 2 * rootsize)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    Node<T> *&helper_skape(Node<T> *&root, const T &element, int upper, int height)
    {
        if (root == NULL)
        {
            return root;
        }
        else
        {
            Node<T> **curr = &root;
            Node<T> **skape;
            int i = 0;
            while ((*curr))
            {
                if ((*curr)->element < element)
                {
                    if (is_skape((*curr), (*curr)->right))
                    {
                        skape = curr;
                        i = 1;
                    }
                    (curr) = &((*curr)->right);
                }
                else if ((*curr)->element > element)
                {
                    if (is_skape((*curr), (*curr)->left))
                    {
                        skape = curr;
                        i = 1;
                    }
                    curr = &((*curr)->left);
                }
                else if ((*curr)->element == element)
                {
                    break;
                }
            }
            if (i)
            {
                return *skape;
            }
            else
            {
                return *curr;
            }
        }
    }
    bool helper_remove(Node<T> *&root, const T &element)
    {
        if (root == NULL)
        {
            return false;
        }
        else if (root->element > element)
        {
            helper_remove(root->left, element);
        }
        else if (root->element < element)
        {
            helper_remove(root->right, element);
        }
        else
        {
            if (root->right != NULL && root->left != NULL)
            {
                root->element = getMax(root->left);
                helper_remove(root->left, root->element);
            }
            else if (root->right != NULL || root->left != NULL)
            {
                if (root->right != NULL)
                {
                    Node<T> *old = root;
                    root = root->right;
                    delete old;
                }
                else
                {
                    Node<T> *old = root;
                    root = root->left;
                    delete old;
                }
            }
            else
            {
                delete root;
                root = NULL;
            }

            return true;
        }
    }

    bool need_to_balance(Node<T> *root, int size, int upper)
    {

        if (upper <= 2 * size && size <= upper)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;
};

#endif // TREE_H

template <class T>
ScapegoatTree<T>::ScapegoatTree()
{
    root = NULL;
    upperBound = 0;
}

template <class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj)
{
    if (obj.root == NULL)
    {
        root = NULL;
        upperBound = 0;
    }
    else
    {
        helper_cc(root, obj.root);
        upperBound = obj.upperBound;
    }
}

template <class T>
ScapegoatTree<T>::~ScapegoatTree()
{
    if (root == NULL)
    {
    }
    else
    {
        destructor(root);
        upperBound = 0;
    }
}

template <class T>
bool ScapegoatTree<T>::isEmpty() const
{
    return (root == NULL);
}

template <class T>
int ScapegoatTree<T>::getHeight() const
{
    if (root == NULL)
    {
        return -1;
    }
    else
    {
        return height(root);
    }
}

template <class T>
int ScapegoatTree<T>::getSize() const
{
    if (root == NULL)
    {
        return 0;
    }
    else
    {
        return helper_size(root);
    }
}

template <class T>
bool ScapegoatTree<T>::insert(const T &element)
{
    bool inserted = false;
    inserted = helper_insert(root, element);
    if (inserted)
    {
        int upper = upperBound;
        int h = getHeight();
        if (pow(1.5, h) > upper)
        {
            Node<T> *&problematic = helper_skape(root, element, upper, h);
            if (problematic != NULL)
            {
                int size = helper_size(problematic);
                int index = 0;
                T *arr = new T[size];
                make_array(problematic, arr, index);
                destructor(problematic);
                problematic = construct(arr, 0, size - 1);
                delete[] arr;
            }
        }
    }
    return inserted;
}

template <class T>
bool ScapegoatTree<T>::remove(const T &element)
{
    bool removed = false;
    removed = helper_remove(root, element);
    if (removed)
    {
        int size = helper_size(root);
        int upper = upperBound;
        if (need_to_balance(root, size, upper))
        {
            balance();
            upperBound = helper_size(root);
        }
    }
    return removed;
}

template <class T>
void ScapegoatTree<T>::removeAllNodes()
{
    if (root == NULL)
    {
    }
    else
    {
        destructor(root);
        upperBound = 0;
    }
}

template <class T>
const T &ScapegoatTree<T>::get(const T &element) const
{
    if (root == NULL)
    {
        throw NoSuchItemException();
    }
    else
    {
        Node<T> *po = get(root, element);
        if (po == NULL)
        {
            throw NoSuchItemException();
        }
        return po->element;
    }
}

template <class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const
{

    if (tp == preorder)
    {
        if (isEmpty())
        {
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl
                  << "}" << std::endl;
    }
    else if (tp == inorder)
    {
        if (isEmpty())
        {
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }
        std::cout << "BST_inorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl
                  << "}" << std::endl;
    }
    else if (tp == postorder)
    {
        if (isEmpty())
        {
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl
                  << "}" << std::endl;
    }
}

template <class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const
{

    if (tp == preorder)
    {
        if (node == NULL)
            return;
        std::cout << "\t" << node->element;
        if (node->left)
        {
            std::cout << "," << std::endl;
        }
        print(node->left, preorder);

        if (node->right)
        {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);
    }
    else if (tp == inorder)
    {
        if (node == NULL)
            return;

        print(node->left, inorder);
        if (node->left)
        {
            std::cout << "," << std::endl;
        }

        std::cout << "\t" << node->element;

        if (node->right)
        {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    }
    else if (tp == postorder)
    {
        if (node == NULL)
            return;

        print(node->left, postorder);
        if (node->left)
        {
            std::cout << "," << std::endl;
        }
        print(node->right, postorder);
        if (node->right)
        {
            std::cout << "," << std::endl;
        }

        std::cout << "\t" << node->element;
    }
}

template <class T>
void ScapegoatTree<T>::printPretty() const
{

    if (isEmpty())
    {
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;
}

template <class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const
{

    if (node == NULL)
        return;

    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i)
    {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    printPretty(node->left, indentLevel + 1, true);

    printPretty(node->right, indentLevel + 1, false);
}

template <class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs)
{
    if (this != &rhs)
    {
        removeAllNodes();
        if (rhs.root == NULL)
        {
            root = NULL;
        }
        else
        {
            helper_cc(root, rhs.root);
        }
        upperBound = rhs.upperBound;
    }

    return *this;
}

template <class T>
void ScapegoatTree<T>::balance()
{
    if (root == NULL)
    {
        return;
    }
    int size = getSize(), index = 0;
    T *arr = new T[size];
    make_array(root, arr, index);
    removeAllNodes();
    root = construct(arr, 0, size - 1);
    delete[] arr;
}

template <class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const
{
    if (root == NULL)
    {
        throw NoSuchItemException();
    }
    else
    {
        Node<T> *p = NULL;
        p = Ceil(root, element);
        if (p == NULL)
        {
            throw NoSuchItemException();
        }
        return p->element;
    }
}

template <class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const
{
    if (root == NULL)
    {
        throw NoSuchItemException();
    }
    else
    {
        Node<T> *p = NULL;
        p = Floor(root, element);
        if (p == NULL)
        {
            throw NoSuchItemException();
        }
        return p->element;
    }
}

template <class T>
const T &ScapegoatTree<T>::getMin() const
{
    if (root == NULL)
    {
        throw NoSuchItemException();
    }
    else
    {
        return getMin(root);
    }
}

template <class T>
const T &ScapegoatTree<T>::getMax() const
{
    if (root == NULL)
    {
        throw NoSuchItemException();
    }
    else
    {
        return getMax(root);
    }
}

template <class T>
const T &ScapegoatTree<T>::getNext(const T &element) const
{
    if (root == NULL)
    {
        throw NoSuchItemException();
    }
    else
    {
        Node<T> *po = helper_next(root, element);
        if (po == NULL)
        {
            throw NoSuchItemException();
        }
        return po->element;
    }
}
