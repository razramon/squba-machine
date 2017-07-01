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
			std::cout <<std::endl;
		}
	}
}

std::string moduloFive(int x)
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

// The main was built according to http://moodle.tau.ac.il/mod/forum/discuss.php?d=71564
int main() {
	cout << "Test 1" << endl << "---------" << endl;
	Matrix2d<int> martix1 = { { 1,2,6 },{ 3,4 },{ 3,4,6 } }; 
	auto groups1 = martix1.groupValues([](int i) {return i == 1 ? 1 : 0; });
	print(groups1);
	cout << endl;

	cout << "Test 2" << endl << "---------" << endl;
	Matrix2d<char> martix2 = { { 'a', 'A','a' },{ 'B','a', 'B' },{ 'B', 'a', 'B' } };
	auto groups2 = martix2.groupValues([](char i) {return islower(i) ? "L" : "U"; });
	print(groups2);
	cout << endl;

	cout << "Test 3" << endl << "---------" << endl;
	Matrix3d<int> martix3 = { { { 1, 2, 3 },{ 1, 2 },{ 1, 2 } },{ { 1, 2 },{ 1, 2, 3, 4 } } };
	auto groups3 = martix3.groupValues([](auto i) {return i % 3 ? "!x3" : "x3"; });
	print(groups3);
	cout << endl;

	cout << "Test 4" << endl << "---------" << endl;
	Matrix<int, 4> martix4 = { { { { 11, 25 },{ 0, 2,53 },{ 77, 55 } },{ { 31, 354 },{ 1, 2, 3, 4 } } } ,
						{ { { 21, 27, 35 },{ 502, 16, 17 },{ 4,5,6 } },{ { 7,8,9 },{ 19,20,21,22 } } } };
	auto groups4 = martix4.groupValues([](auto i) {return i % 5 ? "NOT Multiple of 5" : "Multiple Of 5"; });
	print(groups4);
	cout << endl;

	cout << "Test 5" << endl << "---------" << endl;
	auto groups5 = martix4.groupValues(moduloFive);
	print(groups5);
	cout << endl;
}