#include "the1.h"
int recursion = 0;
double total = 0;
double absolute(double x, double y)
{
    if (x > y)
    {
        return x - y;
    }
    else
    {
        return y - x;
    }
}
unsigned short *median_lomuto(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, int first, int last)
{
    int middle = (first + last) / 2;
    short temp;
    double distance;
    if ((arr[last] >= arr[first] && arr[last] <= arr[middle]) || (arr[last] <= arr[first] && arr[last] >= arr[middle]))
    {
    }
    else if ((arr[first] > arr[last] && arr[first] < arr[middle]) || (arr[first] < arr[last] && arr[first] > arr[middle]))
    {
        temp = arr[last];
        arr[last] = arr[first];
        arr[first] = temp;

        distance = absolute(last, first);
        total += distance;
        swap++;

        if (max_dist < distance)
        {
            max_dist = distance;
        }
    }
    else
    {
        temp = arr[last];
        arr[last] = arr[middle];
        arr[middle] = temp;

        distance = absolute(last, middle);
        total += distance;
        swap++;

        if (max_dist < distance)
        {
            max_dist = distance;
        }
    }
    return arr;
}
unsigned short *median_hoare(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, int first, int last)
{
    int middle = (first + last) / 2;
    short temp;
    double distance;
    if ((arr[middle] >= arr[first] && arr[middle] <= arr[last]) || (arr[middle] <= arr[first] && arr[middle] >= arr[last]))
    {
    }
    else if ((arr[first] < arr[middle] && arr[first] > arr[last]) || (arr[first] > arr[middle] && arr[first] < arr[last]))
    {
        temp = arr[first];
        arr[first] = arr[middle];
        arr[middle] = temp;

        distance = absolute(first, middle);
        total += distance;
        swap++;

        if (max_dist < distance)
        {
            max_dist = distance;
        }
    }
    else
    {
        temp = arr[last];
        arr[last] = arr[middle];
        arr[middle] = temp;

        distance = absolute(last, middle);
        total += distance;
        swap++;

        if (max_dist < distance)
        {
            max_dist = distance;
        }
    }
    return arr;
}
int lomuto(unsigned short *arr, int start, int end, long &swap, double &avg_dist, double &max_dist, bool median_of_3)
{
    if (median_of_3 && (end - start > 1))
    {
        arr = median_lomuto(arr, swap, avg_dist, max_dist, start, end);
    }
    short x = arr[end], temp;
    int i = start - 1;
    double distance;
    for (int j = start; j < (end); j++)
    {
        if (arr[j] >= x)
        {
            i = i + 1;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            distance = absolute(i, j);
            total += distance;
            swap++;

            if (max_dist < distance)
            {
                max_dist = distance;
            }
        }
    }
    temp = arr[i + 1];
    arr[i + 1] = arr[end];
    arr[end] = temp;
    distance = absolute(end, (i + 1));
    total += distance;
    swap++;
    if (max_dist < distance)
    {
        max_dist = distance;
    }
    return i + 1;
}
void quick_lomuto(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, bool median_of_3, int start, int end)
{
    recursion++;
    int p;
    if (start < end)
    {
        p = lomuto(arr, start, end, swap, avg_dist, max_dist, median_of_3);
        quick_lomuto(arr, swap, avg_dist, max_dist, median_of_3, start, p - 1);
        quick_lomuto(arr, swap, avg_dist, max_dist, median_of_3, p + 1, end);
    }
}
int hoare(unsigned short *arr, int start, int end, long &swap, double &avg_dist, double &max_dist, bool median_of_3)
{
    if (median_of_3 && (end - start > 1))
    {
        arr = median_hoare(arr, swap, avg_dist, max_dist, start, end);
    }
    short x = arr[(start + end) / 2], temp;
    double distance;
    int i = start - 1, j = end + 1;
    while (1)
    {
        do
        {
            j = j - 1;
        } while (arr[j] < x);
        do
        {
            i = i + 1;
        } while (arr[i] > x);
        if (i < j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            distance = absolute(i, j);
            total += distance;
            swap++;
            if (max_dist < distance)
            {
                max_dist = distance;
            }
        }
        else
        {
            return j;
        }
    }
}
void quick_hoare(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, bool median_of_3, int start, int end)
{
    recursion++;
    int p;
    if (start < end)
    {
        p = hoare(arr, start, end, swap, avg_dist, max_dist, median_of_3);
        quick_hoare(arr, swap, avg_dist, max_dist, median_of_3, start, p);
        quick_hoare(arr, swap, avg_dist, max_dist, median_of_3, p + 1, end);
    }
}
int quickSort(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, bool hoare, bool median_of_3, int size)
{
    // Your code here
    recursion = 0;
    swap = 0, avg_dist = 0, max_dist = 0;
    if (size > 1)
    {
        if (hoare)
        {
            quick_hoare(arr, swap, avg_dist, max_dist, median_of_3, 0, (size - 1));
        }
        else
        {
            quick_lomuto(arr, swap, avg_dist, max_dist, median_of_3, 0, (size - 1));
        }
        avg_dist = total / swap;
        return recursion;
    }
    else
    {
        return 1;
    }
}
