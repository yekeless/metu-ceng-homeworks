#include <stdio.h>
#define W_INDEX 0
#define H_INDEX 1
#define T_INDEX 2
#define P_INDEX 3
#define S_INDEX 4
#define A_INDEX 5
#define N_INDEX 6

int main()
{
    int k, l, m, n, p, i = 0, wall_length, tribe_number, width, height, time, first_attack, speed, attack_interval, attack_number, wall_array[100001] = {0};
    scanf("%d %d", &wall_length, &tribe_number);
    for (k = 0; k < wall_length + 1; k++)
    {
        wall_array[k] = 1;
    }
    while (i < tribe_number)
    {
        scanf(" %d %d %d %d %d %d %d", &width, &height, &time, &first_attack, &speed, &attack_interval, &attack_number);
        for (l = 0; l < attack_number; l++)
        {
            m = first_attack + width;
            n = first_attack;
            while (n < m)
            {
                if ((n >= 0 && n <= wall_length) && (height > wall_array[n]))
                {
                    wall_array[n] = height;
                }
                n++;
            }
            first_attack = first_attack + (speed * attack_interval);
        }

        i++;
    }
    for (p = 0; p < wall_length + 1; p++)
    {
        if ((p == 100001) || (wall_array[p + 1] == 0))
        {
            printf("%d\n", wall_array[p]);
        }
        else if (wall_array[p + 1] != 0)
        {
            printf("%d ", wall_array[p]);
        }
    }
    return 0;
}
