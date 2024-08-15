#include "the4.h"

// do not add extra libraries here

int divide_land(int X, int Y, bool **possible_plots)
{
    int table[X + 1][Y + 1];
    for (int i = 0; i <= X; i++)
    {
        for (int j = 0; j <= Y; j++)
        {
            table[i][j] = 0;
        }
    }
    for (int i = 1; i <= X; i++)
    {
        for (int j = 1; j <= Y; j++)
        {
            table[i][j] = std::max(std::max(table[i - 1][j], table[i][j - 1]), table[i - 1][j - 1]);
            if (possible_plots[i][j])
            {
                table[i][j] = std::max(table[i][j], i * j);
            }
            for (int k = 1; k < i; k++)
            {
                table[i][j] = std::max(table[i][j], table[k][j] + table[i - k][j]);
            }
            for (int l = 1; l < j; l++)
            {
                table[i][j] = std::max(table[i][j], table[i][l] + table[i][j - l]);
            }
        }
    }
    return X * Y - table[X][Y];
}
