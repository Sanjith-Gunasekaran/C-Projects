#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main (void)
{
    typedef struct
    {
        int doorval;
        bool locked;
        bool chosen;
    } door;
    door doors[3];
    int x0;
    int x100;
    for (int j = 0; j < 1000000; j++)
    {
        int rand100 = rand() % 3;
        doors[rand100].doorval = 100;
        for (int i = 0; i < 3; i++)
        {
            doors[i].locked = true;
            doors[i].chosen = false;
            if (i != rand100)
            {
                doors[i].doorval = 0;
            }
        }
        int rand0 = rand() % 3;
        doors[rand0].chosen = true;
        for (int i = 0; i < 3; i++)
        {
            if (i != rand100 && i != rand0)
            {
                doors[i].locked = false;
                break;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            if (doors[i].locked == true && i != rand0)
            {
                doors[rand0].chosen = false;
                doors[i].chosen = true;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            if (doors[i].chosen == true && doors[i].doorval == 100)
            {
                x100++;
            }
            else if (doors[i].chosen == true && doors[i].doorval == 0)
            {
                x0++;
            }
        }
    }
    printf("%d  %d", x0, x100);
}