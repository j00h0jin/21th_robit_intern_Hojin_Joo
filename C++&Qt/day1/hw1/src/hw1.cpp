#include "../include/hw1.hpp"

void hw1::makeArr()
{
    arr = new int[count];
}

void hw1::gainValue()
{
    max = arr[0];
    min = arr[0];
    sum = arr[0];

    for (int i = 1; i < count; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }

        if (min > arr[i])
        {
            min = arr[i];
        }

        sum += arr[i];
    }

    avg = sum / (float)count;
}

void hw1::deleteArr()
{
    delete[] arr;
}
