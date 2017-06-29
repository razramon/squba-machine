#pragma once

template <class TYPE, size_t DIMENSIONS, class MATRIX_TYPE>
class Coord
{
	constexpr static size_t NUM_DIMENSIONS = DIMENSIONS;
	int location;
	std::vector<int> coordinates;
	TYPE value;

public:
	Coord(int loc, MATRIX_TYPE matrix): location(loc), coordinates(NUM_DIMENSIONS)
	{
		//TODO:: find value, update coordinates

	}
	~Coord() = default;
};

