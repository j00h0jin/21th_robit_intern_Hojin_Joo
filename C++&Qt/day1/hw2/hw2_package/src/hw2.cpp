#include "../include/hw2.hpp"
#include <math.h>

void hw2::newList()
{
    clist = new Coordinate[coordinate_count];
}

void hw2::gainMaxMin()
{
    for (int i = 0; i < coordinate_count; i++)
    {
        for (int j = i + 1; j < coordinate_count; j++)
        {
            // 두 점 사이의 거리 공식 ((x2 - x1)^2 + (y2 - y1)^2)^(1/2)
            float squareXY = pow(clist[j].x - clist[i].x, 2) + pow(clist[j].y - clist[i].y, 2);
            dist = sqrt(squareXY);

            // 맨 처음 계산한 거리를 min으로 잡아두고 앞으로 계산할 값과 비교하여 min 갱신
            if (i == 0 && j == 1)
            {
                d_min = dist;
                min_idx_1 = 0;
                min_idx_2 = 1;
            }
            else if (d_min > dist)
            {
                d_min = dist;
                min_idx_1 = i;
                min_idx_2 = j;
            }

            if (d_max < dist)
            {
                d_max = dist;
                max_idx_1 = i;
                max_idx_2 = j;
            }
        }
    }
}

void hw2::deleteList()
{
    delete[] clist;
}
