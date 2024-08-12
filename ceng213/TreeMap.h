#ifndef TREEMAP_H
#define TREEMAP_H

#include <iostream>
#include <vector>

#include "KeyValuePair.h"
#include "ScapegoatTree.h"

template <class K, class V>
class TreeMap
{
public: // DO NOT CHANGE THIS PART.
    TreeMap();

    void clear();

    const V &get(const K &key) const;

    bool pop(const K &key);

    bool update(const K &key, const V &value);

    const KeyValuePair<K, V> &ceilingEntry(const K &key);

    const KeyValuePair<K, V> &floorEntry(const K &key);

    const KeyValuePair<K, V> &firstEntry();

    const KeyValuePair<K, V> &lastEntry();

    void pollFirstEntry();

    void pollLastEntry();

    std::vector<KeyValuePair<K, V>> subMap(K fromKey, K toKey) const;

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
private: // DO NOT CHANGE THIS PART.
    ScapegoatTree<KeyValuePair<K, V>> stree;
};

template <class K, class V>
TreeMap<K, V>::TreeMap() {}

template <class K, class V>
void TreeMap<K, V>::clear()
{
    stree.removeAllNodes();
}

template <class K, class V>
const V &TreeMap<K, V>::get(const K &key) const
{

    KeyValuePair<K, V> t = KeyValuePair<K, V>(key);
    return (stree.get(t)).getValue();
}

template <class K, class V>
bool TreeMap<K, V>::pop(const K &key)
{
    int i = 0, iterate = stree.getSize();
    if (stree.isEmpty())
    {
        return false;
    }
    KeyValuePair<K, V> t = stree.getMin();
    KeyValuePair<K, V> s = KeyValuePair<K, V>(key);
    for (int k = 0; k < iterate; k++)
    {
        if (t == s)
        {
            i = 1;
            break;
        }
        else
        {
            if (t == stree.getMax())
            {
                break;
            }
            else
            {
                t = stree.getNext(t);
            }
        }
    }

    if (i)
    {
        stree.remove(t);
        return true;
    }
    else
    {
        return false;
    }
}

template <class K, class V>
bool TreeMap<K, V>::update(const K &key, const V &value)
{
    KeyValuePair<K, V> t = KeyValuePair<K, V>(key, value);
    return stree.insert(t);
}

template <class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::ceilingEntry(const K &key)
{
    KeyValuePair<K, V> t = KeyValuePair<K, V>(key);
    return stree.getCeiling(t);
}

template <class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::floorEntry(const K &key)
{
    KeyValuePair<K, V> t = KeyValuePair<K, V>(key);
    return stree.getFloor(t);
}

template <class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::firstEntry()
{
    return stree.getMin();
}

template <class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::lastEntry()
{
    return stree.getMax();
}

template <class K, class V>
void TreeMap<K, V>::pollFirstEntry()
{
    KeyValuePair<K, V> t = stree.getMin();
    if (stree.isEmpty())
    {
        throw NoSuchItemException();
    }
    else
    {
        stree.remove(t);
    }
}

template <class K, class V>
void TreeMap<K, V>::pollLastEntry()
{
    KeyValuePair<K, V> t = stree.getMax();
    if (stree.isEmpty())
    {
        throw NoSuchItemException();
    }
    else
    {
        stree.remove(t);
    }
}

template <class K, class V>
std::vector<KeyValuePair<K, V>> TreeMap<K, V>::subMap(K fromKey, K toKey) const
{
    std::vector<KeyValuePair<K, V>> t;
    KeyValuePair<K, V> first = stree.get(fromKey);
    KeyValuePair<K, V> last = stree.get(toKey);
    while (first != last)
    {
        t.push_back(first);
        first = stree.getNext(first);
    }
    t.push_back(last);
    return t;
}

template <class K, class V>
void TreeMap<K, V>::print() const
{

    std::cout << "# Printing the tree map ..." << std::endl;

    std::cout << "# ScapegoatTree<KeyValuePair<K, V> > stree:" << std::endl;
    stree.printPretty();

    std::cout << "# Printing is done." << std::endl;
}

#endif // TREEMAP_H
