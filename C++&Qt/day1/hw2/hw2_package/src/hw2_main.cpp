/*
점들의 집합에서 임의의 두 점 사이의 거리를 계산할 때,
최솟값과 최댓값을 구하고 그것에 해당하는 각각의 두 점을 구하는 클래스를 작성하시오.

조건:
메모리 동적할당을 이용할 것
structure(구조체)를 이용하여 2차원 좌표를 구현할 것
2차원 점은 정해진 범위 내에서 랜덤으로 정해진 개수만큼 생성할 것
2차원 좌표의 범위와 점의 개수는 “cin”으로 입력받을 것
class는 헤더파일과 소스파일로 나누어 작성할 것

좌표(점) 개수, 좌표 범위 최솟값, 최댓값 순으로 받음
랜덤으로 생성한 좌표 출력
결과값에는 두 점 사이의 거리가 최소인 값과 두 점의 좌표
두 점 사이의 거리가 최대인 값과 두 점의 좌표를 출력하는 것 같음
*/

#include "../include/hw2.hpp"
#include <iostream>
#include <random>

using namespace std;

int main()
{
    int min, max;

    hw2 hw2;

    cout << "생성할 좌표 개수를 입력하세요: ";
    cin >> hw2.coordinate_count;

    hw2.newList();

    cout << "좌표 범위 최솟값: ";
    cin >> min;

    cout << "좌표 범위 최댓값: ";
    cin >> max;

    cout << endl;

    random_device rd;
    uniform_int_distribution<int> distribution(min, max);

    // 랜덤 좌표 생성 및 출력
    for (int i = 0; i < hw2.coordinate_count; i++)
    {
        hw2.clist[i].x = distribution(rd);
        hw2.clist[i].y = distribution(rd);
        cout << i + 1 << "번째 좌표 x: " << hw2.clist[i].x << " y: " << hw2.clist[i].y << endl;
    }

    hw2.gainMaxMin();

    cout << endl;
    cout << "거리 최댓값: " << hw2.d_max << endl;
    cout << "좌표: P1(" << hw2.clist[hw2.max_idx_1].x << ", " << hw2.clist[hw2.max_idx_1].y << ")";
    cout << " & P2(" << hw2.clist[hw2.max_idx_2].x << ", " << hw2.clist[hw2.max_idx_2].y << ")" << endl << endl;

    cout << "거리 최솟값: " << hw2.d_min << endl;
    cout << "좌표: P1(" << hw2.clist[hw2.min_idx_1].x << ", " << hw2.clist[hw2.min_idx_1].y << ")";
    cout << " & P2(" << hw2.clist[hw2.min_idx_2].x << ", " << hw2.clist[hw2.min_idx_2].y << ")" << endl;

    hw2.deleteList();
    return 0;
}