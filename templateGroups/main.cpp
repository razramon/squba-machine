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
			std::cout << std::endl;
		}
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

	char x;
	std::cin >> x;
}