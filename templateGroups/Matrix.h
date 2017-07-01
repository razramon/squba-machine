#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

typedef std::vector<int> coordinate;
typedef std::vector<coordinate> group;

template<class T, size_t DIMENSIONS>
class Matrix;

/*
 * Gets 2 coordinates and a number, n, representing the number of positions in each coordinate.
 * Returns true if two coordinates are neighbours.
 * Note: this function is outside class Matrix: because we don't need to create it for each matrix.
 */
static bool areNeighbours(const coordinate& c1,const coordinate& c2, size_t n)
{
	int numOfDifferentPositions = 0;
	for (size_t i = 0; i < n; ++i)
	{
		if (c1[i] != c2[i])
		{
			int temp = c1[i];

			if (temp == c2[i] + 1 || temp == c2[i]-1)
			{
				numOfDifferentPositions++;
			} else //the two coordinates differ by more than 1 in at least 1 position => they aren't neighbours
			{
				return false;
			}
			if (numOfDifferentPositions > 1)
			{
				return false;
			}
		}
	}
	if (numOfDifferentPositions!=1)
	{
		return false;
	}
	return true;
}

/*
 * Iterates over "allCoords" in search for neighbours of coordinate c:
 * if finds a neighbour, deletes it from "allCoords" and inserts it to "groupy"
 * 
 * Note: gets coordinate *not* by reference, because the original "c" is inside "groupy", 
 *		 and we're changing groupy inside this function: so original "c"'s place changes 
 */
static void insertNeighboursToGroup(std::vector<coordinate>& groupy, std::vector<coordinate>& allCoords, const coordinate c, size_t n) {

	std::vector<coordinate>::iterator iter = allCoords.begin();

	while (iter != allCoords.end()) {
		if (areNeighbours(c, *iter, n)) {
			groupy.push_back(std::move(*iter));
			iter = allCoords.erase(iter);
			continue;
		}
		++iter;
	}

}

/*
 * Gets a vector of all coordinates and divides them to groups:
 * Returns a vector of neighbour-groups
 * Note: empties allCoords!
 */
static std::vector<group> buildNeighbourGroups(std::vector<coordinate>& allCoords, size_t n)
{

	std::vector<group> allGroups;

	while (allCoords.size() != 0) {

		std::vector<coordinate> currentGroup;
		currentGroup.push_back(allCoords[0]);
		allCoords.erase(allCoords.begin());

		size_t c = 0;

		while (c < currentGroup.size()) {

			insertNeighboursToGroup(currentGroup, allCoords, currentGroup[c], n);
			++c;
		}

		allGroups.push_back(currentGroup);

	}
	
	return allGroups;
}


//author Amir Kirsh (+small fix)
template<class T, size_t DIMENSIONS>
struct MatrixCopier {
	static void copy(T* dest, size_t dest_size, const size_t* dest_dimensions, const T* source, size_t source_size, const size_t* source_dimensions) {
		size_t dest_size0 = 0;
		size_t source_size0 = 0;

		// Makes sure no dividing by 0
		if (dest_dimensions[0]) {
			dest_size0 = dest_size / dest_dimensions[0];
		}

		if (source_dimensions[0]) {
			source_size0 = source_size / source_dimensions[0];
		}

		for (size_t i = 0; i < source_dimensions[0]; ++i) {
			MatrixCopier<T, DIMENSIONS - 1>::copy(dest + (i * dest_size0), dest_size0, dest_dimensions + 1, source + (i * source_size0), source_size0, source_dimensions + 1);
		}
	}
};

//author Amir Kirsh 
template<class T>
struct MatrixCopier<T, 1> {
	static void copy(T* dest, size_t dest_size, const size_t* dest_dimensions, const T* source, size_t source_size, const size_t* source_dimensions) {
		for (size_t i = 0; i < source_size; ++i) {
			dest[i] = source[i];
		}
	}
};

template<class T, size_t DIMENSIONS>
class Matrix {
	constexpr static size_t NUM_DIMENSIONS = DIMENSIONS;
	std::unique_ptr<T[]> _array = nullptr;
	size_t _dimensions[NUM_DIMENSIONS] = {};
	const size_t _size = 0;
	friend class Matrix<T, NUM_DIMENSIONS + 1>;

	std::vector<int> getCoordByLocationInArray(int loc, std::vector<size_t> dimentionsSizesByDimention) {

		coordinate coord(NUM_DIMENSIONS); //creates a vector of size NUM_DIMENSTIONS, that represents current coordinate

		for (int indexDimention = 0; indexDimention < NUM_DIMENSIONS; indexDimention++) {

			// Cast to int - from size_t
			coord.at(indexDimention) = (loc / dimentionsSizesByDimention[indexDimention]) % static_cast<int>(_dimensions[indexDimention]);
		}

		return coord;
	}

public:

	size_t size() const { return _size; }
	Matrix() {}

	// DIMENSIONS == 1
	template<typename G = T>
	Matrix(const std::initializer_list<typename std::enable_if_t<NUM_DIMENSIONS == 1, G>>& values) {
		const_cast<size_t&>(_size) = values.size();
		_dimensions[0] = _size;
		_array = std::make_unique<T[]>(_size);
		size_t i = 0;
		for (auto& val : values) {
			_array[i++] = val;
		}
	}

	// DIMENSIONS > 1
	template<typename G = T>
	Matrix(const std::initializer_list<Matrix<typename std::enable_if_t<(NUM_DIMENSIONS > 1), G>, NUM_DIMENSIONS - 1>>& values) {
		_dimensions[0] = values.size();
		for (auto& m : values) {
			for (size_t dim = 0; dim < NUM_DIMENSIONS - 1; ++dim) {
				if (m._dimensions[dim] > _dimensions[dim + 1]) {
					_dimensions[dim + 1] = m._dimensions[dim];
				}
			}
		}
		size_t size = 1;
		for (size_t dim = 0; dim < NUM_DIMENSIONS; ++dim) {
			size *= _dimensions[dim];
		}
		const_cast<size_t&>(_size) = size;
		_array = std::make_unique<T[]>(_size); // "zero initialized" - T()
		size_t i = 0;
		size_t dest_size = _size / _dimensions[0];
		for (auto& m : values) {
			MatrixCopier<T, NUM_DIMENSIONS - 1>::copy(&(_array[i * dest_size]), dest_size, _dimensions + 1, m._array.get(), m._size, m._dimensions);
			++i;
		}
	}

	//We added "noexcept" because we assume it will be used only with relevant Matrix type
	Matrix(Matrix&& m) noexcept {
		*this = std::move(m);
	}

	//We added "noexcept" because we assume it will be used only with relevant Matrix type
	auto& operator=(Matrix&& m) noexcept {
		std::swap(_array, m._array);
		std::swap(const_cast<size_t&>(_size), const_cast<size_t&>(m._size));
		std::swap(_dimensions, m._dimensions);
		return *this;
	}

	size_t getDimension(size_t i) const {
		return _dimensions[i];
	}

	/*
	 * Retruns groups of neighbour coordinates that belong to the same group according to groupFunc()
	 * NOTE: there's a lot of documentation inside this function's code! it only 23 lines of code :)
	 */
	template<typename GROUPFUNC, typename G = T>
	auto groupValues(GROUPFUNC groupFunc) {

		using groupResult = std::result_of_t<GROUPFUNC(G&)>;

		/*
		 * Creates a vector of size NUM_DIMENSIONS, initialized to 1s.
		 * Will be used to calculate the location of each coordinate
		 */
		std::vector<size_t> dimensionsSizesByDimension(NUM_DIMENSIONS, 1); 

		/*
		 * We'll explain the propose of next 3 lines with an example:
		 *		suppose we get a matrix of dimensions: 4x3x9x17x5
		 *		we calculate each dimension's size: note that it depends on the sizes of all next dimensions
		 *		in our example, the second dimension's ("3") size depends on the sizes of 
		 *		the 3rd(9), 4th(17) and 5th (5) dimensions: the second dimension size is: 765(=9*17*5).
		 *		The 3 lines updates dimentionsSizesByDimention to contain (in our example):
		 *		dimentionsSizesByDimention = (2295, 459, 27, 3, 1)
		 */
		for (int indexDimention = NUM_DIMENSIONS - 2 ; indexDimention >= 0; --indexDimention) {

			dimensionsSizesByDimension[indexDimention] *= dimensionsSizesByDimension[indexDimention + 1] * _dimensions[indexDimention + 1];
		}


		std::map<groupResult, std::vector<coordinate>> mapCoordinateToGroupResult;
		
		/*
		*	Updates mapCoordinateToGroupResult to contain:
		*				keys = all "return types" of function groupFunc() that appeared when applying it on the matrix
		*				values = a vector containing all coordinates that returned the relevant key when called with groupFunc()
		*						(Not divided to neighbour groups yet!)
		*/
		for (int index = 0; index < _size; index++) {

			//Accessing std::map with map[i] - if key i doesn't exist, creates it, so no need to check if key "groupFunc(_array[index])" exists. 
			mapCoordinateToGroupResult[groupFunc(_array[index])].push_back(getCoordByLocationInArray(index, dimensionsSizesByDimension));
		}

		std::vector<std::pair<groupResult, std::vector<group>>> allGroups;

		//build allGroups:
		for (typename std::map<groupResult, std::vector<coordinate>>::iterator iter = mapCoordinateToGroupResult.begin(); iter!= mapCoordinateToGroupResult.end(); ++iter)
		{

			std::vector<group > groupOfResult = buildNeighbourGroups(iter->second, NUM_DIMENSIONS);

			// Sorting the coordinates in the group before inserting
			for (int indexGroup = 0; indexGroup < groupOfResult.size(); indexGroup++) {

				sort(groupOfResult[indexGroup].begin(), groupOfResult[indexGroup].end());
			}

			allGroups.push_back(std::make_pair(iter->first, groupOfResult));
		}

		return allGroups;
	}
};


template<class T>
using Matrix2d = Matrix<T, 2>;

template<class T>
using Matrix3d = Matrix<T, 3>;