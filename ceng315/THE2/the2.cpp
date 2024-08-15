#include "the2.h"
// do not add extra libraries here
/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    ascending : true for ascending, false for descending
    size      : number of elements in the array
*/
int loop = 0;
void count_sort(std::string *arr, int size, int index)
{
    int count[27];
    for (int i = 0; i < 27; i++)
    {
        count[i] = 0;
    }
    std::string *temp = new std::string[size];
    for (int i = 0; i < size; i++)
    {
        int ix;
        if (index >= (arr[i].length()))
        {
            ix = 0;
        }
        else
        {
            ix = arr[i][index] - 'A';
            ix++;
        }
        count[ix]++;
        loop++;
    }
    for (int j = 1; j <= 26; j++)
    {
        loop++;
        count[j] += count[j - 1];
    }
    for (int k = size - 1; k >= 0; k--)
    {
        int ix;
        if (index >= (arr[k].length()))
        {
            ix = 0;
        }
        else
        {
            ix = arr[k][index] - 'A';
            ix++;
        }
        temp[count[ix] - 1] = arr[k];
        count[ix]--;
        loop++;
    }
    for (int l = 0; l < size; l++)
    {
        arr[l] = temp[l];
        loop++;
    }
    delete[] temp;
}
int radix_string_sort(std::string *arr, int size, bool ascending)
{
    int max = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i].length() > max)
        {
            max = arr[i].length();
        }
    }
    for (int index = max - 1; index >= 0; index--)
    {
        count_sort(arr, size, index);
    }
    if (!ascending)
    {
        std::string *temp = new std::string[size];
        for (int i = 0; i < size; i++)
        {
            temp[i] = arr[size - (i + 1)];
        }
        for (int i = 0; i < size; i++)
        {
            arr[i] = temp[i];
        }
        delete[] temp;
    }
    return loop;
}
