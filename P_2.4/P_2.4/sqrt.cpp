// sqrt.cpp -- using the sqrt() function

#include <iostream>
#include<cmath> // or math.h

int main()
{
	using namespace std;

	double area;
	cout << "���뽨�����,��ƽ��Ӣ��,��ķ���: ";
	cin >> area;
	double side;
	side = sqrt(area);
	cout << "�����൱��һ�������� " << side
		<< "�űߡ�" << endl;
	cout << "��ô������! " << endl;
	return 0;
}