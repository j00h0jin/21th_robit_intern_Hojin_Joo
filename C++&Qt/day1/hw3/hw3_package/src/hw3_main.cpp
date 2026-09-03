/*
A U D R L S

A: 어택
U: y +1
D: y -1
R: x +1
L: x -1
S: status(현재 hp, mp, 좌표) player 기준 50 10이 디폴트인 것 같음

monster와 좌표가 동일할 때 공격이 성공하는 것 같음
다르면 실패(MP 차감)
공격력은 10
*/

#include "../include/hw3.hpp"
#include <iostream>

int main()
{
    Player player(0, 0);
    Monster monster(1, 1, 50);

    player.HP = 50, player.MP = 10;

    char ch;
    int is_end = 0;

    while (1)
    {
        std::cout << "Type Command(A/U/D/R/L/S)" << std::endl;
        std::cin >> ch;

        switch (ch)
        {
        case 65: // A
            if (player.MP == 0)
            {
                is_end = 1;
                break;
            }
            player.Attack(monster);
            if (monster.HP == 0)
            {
                is_end = 1;
                break;
            }
            break;

        case 85: // U
            player.Y_move(1);
            break;

        case 68: // D
            player.Y_move(-1);
            break;

        case 82: // R
            player.X_move(1);
            break;

        case 76: // L
            player.X_move(-1);
            break;

        case 83: // S
            player.Show_status();
            break;

        default:
            break;
        }

        if (is_end == 1)
            break;

        std::cout << std::endl;
    }

    return 0;
}