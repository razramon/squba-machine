#include "Matrix.h"

using std::cout;
using std::endl;

template<typename Groups>
void print(const Groups& all_groups) {
	for (const auto& groupType : all_groups) {
		std::cout << groupType.first << ":" << std::endl;
		for (const auto& groupOfType : groupType.second) {
			for (const auto& coord : groupOfType) {
				std::cout << "{ ";
				for (auto i : coord) {
					std::cout << i << ' ';
				}
				std::cout << "} ";
			}
			std::cout << "\n\n" <<std::endl;
		}
	}
}

char* moduloFive(int x)
{
	switch (x % 5)
	{
	case (0):
		return "0 mod 5";
	case (1):
		return "1 mod 5";
	case (2):
		return "2 mod 5";
	case (3):
		return "3 mod 5";
	case (4):
		return "4 mod 5";
	default:
		return "something has gone wrong!";
	}

}


int main() {
	cout << "***********mat1***********" << endl << endl;
	Matrix2d<int> m = { { 1,2,6 },{ 3,4 },{ 3,4,6 } }; 
	auto groups = m.groupValues([](int i) {return i == 1 ? 1 : 0; });
	print(groups);
	cout << endl;
	cout << "***********mat2***********" << endl << endl;
	Matrix2d<char> m1 = { { 'a', 'A','a' },{ 'B','a', 'B' },{ 'B', 'a', 'B' } };
	auto all_groups = m1.groupValues([](auto i) {return islower(i) ? "L" : "U"; });
	print(all_groups);
	cout << endl;
	cout << "***********mat3***********" << endl << endl;
	Matrix3d<int> m2 = { { { 1, 2, 3 },{ 1, 2 },{ 1, 2 } },{ { 1, 2 },{ 1, 2, 3, 4 } } };
	auto groupss = m2.groupValues([](auto i) {return i % 3 ? "!x3" : "x3"; });
	print(groupss);
	cout << endl;
	cout << "***********mat4***********" << endl << endl;
	Matrix<int, 4> m3 = { { { { 11, 25 },{ 0, 2,53 },{ 77, 55 } },{ { 31, 354 },{ 1, 2, 3, 4 } } } ,
						{ { { 21, 27, 35 },{ 502, 16, 17 },{ 4,5,6 } },{ { 7,8,9 },{ 19,20,21,22 } } } };
	auto grooooops = m3.groupValues([](auto i) {return i % 5 ? "NOT Multiple of 5" : "Multiple Of 5"; });
	print(grooooops);
	cout << endl;
	cout << "***********mat4, AGAIN with other function***********" << endl << endl;
	auto grooops = m3.groupValues(moduloFive);
	print(grooops);
	cout << endl;

	char x;
	std::cin >> x;
}