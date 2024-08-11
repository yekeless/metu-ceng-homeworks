#include "Browser.h"
Browser::Browser()
{
    windows.append(Window());
}

void Browser::newWindow()
{
    windows.prepend(Window());
}

void Browser::closeWindow()
{
    if (windows.getSize() == 0)
    {
    }
    else
    {
        windows.removeNodeAtIndex(0);
    }
}

void Browser::switchToWindow(int index)
{
    windows.moveToIndex(index, 0);
}

Window &Browser::getWindow(int index)
{
    return windows.getNodeAtIndex(index)->data;
}

void Browser::moveTab(Window &from, Window &to)
{
    Tab temp;
    temp = from.getActiveTab();
    to.newTab(temp);
    from.closeTab();
}

void Browser::mergeWindows(Window &window1, Window &window2)
{
    window2.changeActiveTabTo(0);
    while (!(window2.isEmpty()))
    {
        Tab temp;
        temp = window2.getActiveTab();
        window1.newTab(temp);
        window2.closeTab();
    }
}

void Browser::mergeAllWindows()
{
    int iterate = windows.getSize() - 1;
    if (iterate == -1 || iterate == 0)
    {
    }
    else
    {
        Node<Window> *kafa, *temp;
        kafa = windows.getNodeAtIndex(0);
        temp = kafa->next;
        for (int i = 0; i < iterate; i++)
        {
            mergeWindows(kafa->data, temp->data);
            temp = temp->next;
        }
    }
}

void Browser::closeAllWindows()
{
    windows.removeAllNodes();
}

void Browser::closeEmptyWindows()
{
    int iterate = windows.getSize();
    Node<Window> *curr = windows.getNodeAtIndex(0), *temp;
    for (int i = 0; i < iterate; i++)
    {
        if (curr->data.isEmpty())
        {
            temp = curr;
            curr = curr->next;
            windows.removeNode(temp);
        }
        else
        {
            curr = curr->next;
        }
    }
}

void Browser::print()
{
    Node<Window> *head = windows.getFirstNode();
    if (head == NULL)
    {
        std::cout << "The browser is empty" << std::endl;
    }
    else
    {
        (head->data).print();
        head = head->next;
        while (head != windows.getFirstNode())
        {
            (head->data).print();
            head = head->next;
        }
    }
}
