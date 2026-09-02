/*
다음 과제를 C++로, Class를 이용하여 작성하시오
모든 변수와 함수를 클래스 멤버변수, 멤버함수로 작성할 것
(코드에서 생성하는 클래스 객체는 1개)
과제에서 각각의 클래스는 헤더파일과 소스 파일로 따로 나누어 작성할 것

개수 받고 데이터 받고
max min sum avg 구하기
*/

#include "../include/hw1.hpp"
#include <iostream>

using namespace std;

int main()
{

    hw1 test;

    cout << "몇 개의 원소를 할당하시겠습니까?: ";
    cin >> test.count;

    test.makeArr();

    for (int i = 0; i < test.count; i++)
    {
        cout << "정수형 데이터 입력: ";
        cin >> test.temp;
        test.arr[i] = test.temp;
    }
    test.gainValue();

    cout << "최댓값: " << test.max << endl;
    cout << "최솟값: " << test.min << endl;
    cout << "전체합: " << test.sum << endl;
    cout << "평 균: " << test.avg << endl;

    test.deleteArr();

    return 0;
}