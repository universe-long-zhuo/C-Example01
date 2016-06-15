// getinfo --输入和输出
#include <iostream>

int main()
{
	using namespace std;

	int carrots; 

	cout << "你有多少胡萝卜?" << endl;
	cin >> carrots;
	cout << "这里有两个。";
	carrots = carrots + 2;
	cout << "现在你有 " << carrots << " 胡萝卜。" << endl;
	return 0;
}