#include "../include/hw3.hpp"
#include <iostream>

using namespace std;

Monster::Monster(int x, int y, int HP)
{
    this->x = x;
    this->y = y;
    this->HP = HP;
}
Player::Player(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Monster::Be_Attacked()
{
    HP = HP - 10;
    cout << "남은 체력: " << HP << endl;

    if (HP <= 0) // 1이면 사망
        return 1;
    else
        return 0;
}

void Player::Attack(Monster &target)
{

    // 좌표 맞고 MP 있을 때, MP 부족일 때, 그 외로 분류
    if (target.x == x && target.y == y && MP > 0)
    {
        MP = MP - 1;
        cout << "공격 성공!" << endl;
        target.Be_Attacked();
    }
    else
    {
        MP = MP - 1;
        cout << "공격 실패!" << endl;
    }
}

void Player::Show_status()
{
    cout << "HP: " << HP << endl;
    cout << "MP: " << MP << endl;
    cout << "Position: " << x << ", " << y << endl;
}

void Player::X_move(int move)
{
    x = x + move;
    cout << "X Position " << move << " moved!" << endl;
}

void Player::Y_move(int move)
{
    y = y + move;
    cout << "Y Position " << move << " moved!" << endl;
}