#include "Window.h"
Window::Window()
{
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab()
{
    Node<Tab> *sekme;
    if (activeTab == -1)
    {

        return Tab();
    }
    else
    {
        sekme = tabs.getNodeAtIndex(activeTab);
        return sekme->data;
    }
}

bool Window::isEmpty() const
{
    int sayi;
    sayi = tabs.getSize();
    if (sayi == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Window::newTab(const Tab &tab)
{
    if (isEmpty())
    {
        tabs.insertAtIndex(tab, 0);
        activeTab = 0;
    }
    else
    {
        tabs.insertAtIndex(tab, activeTab + 1);
        activeTab += 1;
    }
}

void Window::closeTab()
{
    int sekmesayisi = tabs.getSize();
    if (sekmesayisi == 0)
    {
    }
    else if (sekmesayisi == 1)
    {
        tabs.removeNodeAtIndex(0);
        activeTab = -1;
    }
    else
    {
        int index = activeTab;
        if (index != sekmesayisi - 1)
        {
            Node<Tab> *curr;
            curr = tabs.getNodeAtIndex(index);
            tabs.removeNode(curr);
        }
        else
        {
            activeTab--;
            tabs.removeNodeAtIndex(index);
        }
    }
}

void Window::moveActiveTabTo(int index)
{
    int length = tabs.getSize();
    if (tabs.getSize() == 0)
    {
    }
    else if (tabs.getSize() == 1)
    {
    }
    else
    {
        int oldindex = activeTab;
        tabs.moveToIndex(oldindex, index);
        if (index > length - 1)
        {
            activeTab = length - 1;
        }
        else
        {
            activeTab = index;
        }
    }
}

void Window::changeActiveTabTo(int index)
{
    int length = tabs.getSize();
    if (index > length - 1 || activeTab < 0 || index < 0)
    {
    }
    else
    {
        activeTab = index;
    }
}

void Window::addTab(Node<Tab> &tab)
{
    int sekmesayisi = tabs.getSize();
    if (sekmesayisi == 0)
    {
        tabs.append(tab.data);
        activeTab = 0;
    }
    else
    {
        tabs.append(tab.data);
    }
}

void Window::print()
{
    if (tabs.isEmpty())
        std::cout << "Window is empty" << std::endl;
    else
        std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}
