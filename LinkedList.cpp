#include "LinkedList.h"
template <class T>
LinkedList<T>::LinkedList()
{
    head = NULL;
    size = 0;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    Node<T> *curr;
    curr = obj.head;
    head = NULL;
    size = 0;
    for (int i = 0; i < obj.size; i++)
    {
        append(curr->data);
        curr = curr->next;
    }
    size = obj.size;
}

template <class T>
LinkedList<T>::~LinkedList()
{
    Node<T> *curr;
    curr = head;
    for (int i = 0; i < size; i++)
    {
        Node<T> *temp;
        temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = NULL;
    size = 0;
}

template <class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    emptythelist();
    Node<T> *curr;
    curr = rhs.head;
    size = 0;
    for (int i = 0; i < rhs.size; i++)
    {
        append(curr->data);
        curr = curr->next;
    }
    size = rhs.size;
    return *this;
}

template <class T>
int LinkedList<T>::getSize() const
{
    return size;
}

template <class T>
bool LinkedList<T>::isEmpty() const
{
    if (size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    Node<T> *curr;
    curr = head;
    if (curr == NULL)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if (curr == node)
        {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

template <class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    Node<T> *curr;
    if (head == NULL)
    {
        return NULL;
    }
    curr = head;
    return curr;
}

template <class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    Node<T> *curr;
    if (head == NULL)
    {
        return NULL;
    }
    curr = head->prev;
    return curr;
}

template <class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    Node<T> *curr;
    curr = head;
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            if (curr->data == data)
            {
                return curr;
            }
            curr = curr->next;
        }
        return NULL;
    }
}

template <class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    Node<T> *curr;
    curr = head;
    if (index > size - 1)
    {
        return NULL;
    }
    else if (index < 0)
    {
        return NULL;
    }
    else if (head == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }
    return curr;
}

template <class T>
void LinkedList<T>::append(const T &data)
{
    Node<T> *newnode = new Node<T>(data), *curr, *temp;

    if (head == NULL)
    {
        head = newnode;
        newnode->next = newnode;
        newnode->prev = newnode;
        size++;
    }

    else
    {
        curr = head;
        temp = head->prev;
        temp->next = newnode;
        newnode->prev = temp;
        curr->prev = newnode;
        newnode->next = curr;
        size++;
    }
}

template <class T>
void LinkedList<T>::prepend(const T &data)
{
    Node<T> *newnode = new Node<T>(data);
    Node<T> *temp, *curr;
    if (head == NULL)
    {
        head = newnode;
        newnode->next = newnode;
        newnode->prev = newnode;
        size++;
    }
    else
    {
        curr = head;
        temp = head->prev;
        newnode->next = curr;
        curr->prev = newnode;
        head = newnode;
        newnode->prev = temp;
        temp->next = newnode;
        size++;
    }
}

template <class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    Node<T> *curr;
    curr = head;
    if (head == NULL)
    {
    }
    else
    {
        int matched = 0;
        for (int i = 0; i < size; i++)
        {
            if (curr == node)
            {
                matched = 1;
                break;
            }
            curr = curr->next;
        }
        if (matched == 0)
        {
        }
        else if (matched)
        {
            Node<T> *newnode = new Node<T>(data);
            Node<T> *temp;
            temp = node;
            curr = curr->next;
            temp->next = newnode;
            curr->prev = newnode;
            newnode->next = curr;
            newnode->prev = temp;
            size++;
        }
    }
}

template <class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    if (index > size || index < 0)
    {
    }
    else if (head == NULL && index == 0)
    {
        Node<T> *newnode = new Node<T>(data);
        head = newnode;
        newnode->next = newnode;
        newnode->prev = newnode;
        size++;
    }
    else if (index == 0)
    {
        prepend(data);
    }
    else if (index == size)
    {
        append(data);
    }
    else
    {
        Node<T> *newnode = new Node<T>(data);
        Node<T> *curr, *temp;
        curr = head;
        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }
        temp = curr->prev;
        newnode->next = curr;
        newnode->prev = temp;
        temp->next = newnode;
        curr->prev = newnode;
        size++;
    }
}

template <class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    Node<T> *curr;
    curr = head;
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        if (curr == node)
        {
            index = i;
            break;
        }
        curr = curr->next;
    }
    return index;
}

template <class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    int son = 0;
    if (head == NULL)
    {
    }
    else if (currentIndex == newIndex)
    {
    }
    else if (currentIndex > size - 1 || currentIndex < 0 || newIndex < 0)
    {
    }

    else if (newIndex > size - 1)
    {
        son = size - 1;
        Node<T> *curr, *temp, *nodetc;
        curr = head;
        for (int i = 0; i < currentIndex; i++)
        {
            curr = curr->next;
        }
        nodetc = curr;

        curr = curr->next;
        temp = nodetc->prev;
        temp->next = curr;
        curr->prev = temp;
        if (nodetc == head)
        {
            head = head->next;
        }
        curr = head;
        temp = curr->prev;
        temp->next = nodetc;
        curr->prev = nodetc;

        nodetc->next = curr;
        nodetc->prev = temp;
        if (newIndex == 0)
        {
            head = nodetc;
        }
    }
    else
    {
        Node<T> *curr, *temp, *nodetc;
        curr = head;
        for (int i = 0; i < currentIndex; i++)
        {
            curr = curr->next;
        }
        nodetc = curr;
        curr = curr->next;
        temp = nodetc->prev;
        temp->next = curr;
        curr->prev = temp;
        if (nodetc == head)
        {
            head = head->next;
        }
        curr = head;
        for (int i = 0; i < newIndex; i++)
        {
            curr = curr->next;
        }
        temp = curr->prev;
        temp->next = nodetc;
        curr->prev = nodetc;
        nodetc->next = curr;
        nodetc->prev = temp;
        if (newIndex == 0)
        {
            head = nodetc;
        }
    }
}

template <class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    if (head == NULL)
    {
    }
    else if (node == head && size == 1)
    {
        Node<T> *temp;
        temp = head;
        head = NULL;
        delete temp;
        size--;
    }
    else if (containsNode(node))
    {
        Node<T> *curr, *temp;
        curr = node->next;
        temp = node->prev;
        temp->next = curr;
        curr->prev = temp;
        if (node == head)
        {
            head = curr;
        }
        delete node;
        size--;
    }
}

template <class T>
void LinkedList<T>::removeNode(const T &data)
{
    int iterate = size;
    Node<T> *curr, *silinecek;
    if (head == NULL)
    {
    }
    else
    {
        curr = head;
        for (int i = 0; i < iterate; i++)
        {
            if (curr->data == data)
            {
                silinecek = curr;
                curr = curr->next;
                removeNode(silinecek);
            }
            else
            {
                curr = curr->next;
            }
        }
    }
}

template <class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    Node<T> *curr, *temp, *silinecek;
    if (head == NULL)
    {
    }
    if (index < 0 || index >= size)
    {
    }
    else
    {
        curr = head;
        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }
        silinecek = curr;
        removeNode(silinecek);
    }
}

template <class T>
void LinkedList<T>::removeAllNodes()
{
    Node<T> *curr;
    curr = head;
    for (int i = 0; i < size; i++)
    {
        Node<T> *temp;
        temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = NULL;
    size = 0;
}

template <class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty())
    {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do
    {
        std::cout << *node << std::endl;
        node = node->next;
    } while (node != this->getFirstNode());
}
