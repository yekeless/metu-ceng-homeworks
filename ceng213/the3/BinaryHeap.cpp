#include "BinaryHeap.h"

BinaryHeap::BinaryHeap()
{
}

bool BinaryHeap::contains(int uniqueId) const
{
    int i = elements.size();
    for (int j = 0; j < i; j++)
    {
        if ((elements[j]).uniqueId == uniqueId)
        {
            return true;
        }
    }
    return false;
}
bool BinaryHeap::Add(int uniqueId, double weight)
{
    if (contains(uniqueId) == false)
    {
        HeapElement newNode;
        newNode.uniqueId = uniqueId;
        newNode.weight = weight;
        elements.push_back(newNode);

        int hole = elements.size() - 1;
        while (hole > 0 && (elements[hole]).weight < (elements[(hole - 1) / 2]).weight)
        {
            HeapElement temp = elements[hole];
            elements[hole] = elements[(hole - 1) / 2];
            elements[(hole - 1) / 2] = temp;
            hole = (hole - 1) / 2;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool BinaryHeap::PopHeap(int &outUniqueId, double &outWeight)
{
    if (elements.size() == 0)
    {
        return false;
    }
    else
    {

        int size = elements.size();
        outUniqueId = (elements[0]).uniqueId;
        outWeight = (elements[0]).weight;
        elements[0] = elements[size - 1];
        elements.pop_back();
        percolatedown(0);
        return true;
    }
}
void BinaryHeap::percolatedown(int index)
{
    int left, right;
    int size = elements.size();
    while (1)
    {
        left = index * 2 + 1;
        right = index * 2 + 2;
        int min = index;
        if (left < size && (elements[left]).weight < (elements[min]).weight)
        {
            min = left;
        }
        if (right < size && (elements[right]).weight < (elements[min]).weight)
        {
            min = right;
        }
        if (min != index)
        {
            HeapElement temp = elements[index];
            elements[index] = elements[min];
            elements[min] = temp;
            index = min;
        }
        else
        {
            break;
        }
    }
}
void BinaryHeap::buildheap()
{

    int size = elements.size();
    for (int i = (size - 1) / 2; i >= 0; i--)
    {
        percolatedown(i);
    }
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    if (contains(uniqueId))
    {
        int size = elements.size();
        for (int i = 0; i < size; i++)
        {
            if ((elements[i]).uniqueId == uniqueId)
            {
                (elements[i]).weight = newWeight;
                break;
            }
        }

        buildheap();
        return true;
    }
    else
    {
        return false;
    }
}

int BinaryHeap::HeapSize() const
{
    return elements.size();
}