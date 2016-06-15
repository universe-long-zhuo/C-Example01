// sqrt.cpp -- using the sqrt() function

#include <iostream>
#include<cmath> // or math.h

int main()
{
	using namespace std;

	double area;
	cout << "进入建筑面积,在平方英尺,你的房子: ";
	cin >> area;
	double side;
	side = sqrt(area);
	cout << "这是相当于一个正方形 " << side
		<< "脚边。" << endl;
	cout << "多么的迷人! " << endl;
	return 0;
}