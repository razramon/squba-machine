#include "BoardCreator.h"


const std::string BoardCreator::BOARD_SUFF = ".sboard";
const char BoardCreator::DELIMETER = 'x';

//std::pair<char **, char**> BoardCreator::getInitBoardForEachPlayer(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips)
//{
//	return std::make_pair(getBoardFromShips((*playersShips).first), getBoardFromShips((*playersShips).second));
//}

std::shared_ptr<boardType> BoardCreator::getBoardFromShips(ptrToShipsVector ships, int numRows, int numCols, int numDepth)
{
	std::shared_ptr<boardType> board = std::move(createBoard(numRows, numCols, numDepth));
	updateShipsInBoard(board, ships);
	return board;
}

void BoardCreator::updateShipsInBoard(std::shared_ptr<boardType> board, ptrToShipsVector ships)
{
	for (int i = 0; i < (*ships).size(); ++i)
	{
		std::shared_ptr<Ship> s = (*ships)[i];

		int** positions = (*s).getPosition();
		char letter = (*s).getLetter();

		for (int pos = 0; pos < (*s).getShipSize(); ++pos)
		{
			(*board)[positions[pos][Ship::INDEX_3D::depth_index]][positions[pos][Ship::INDEX_3D::row_index]][positions[pos][Ship::INDEX_3D::column_index]] = letter;
		}
	}
}

/*
* Returns the relevant index of the "bad coodinates vector"
*/
int BoardCreator::getIndexOfRelevantBadCoordsVector(char letter)
{
	switch (letter)
	{
	case 'B': return 0;
	case 'b': return 1;
	case 'P': return 2;
	case 'p': return 3;
	case 'M': return 4;
	case 'm': return 5;
	case 'D': return 6;
	case 'd': return 7;
	default: return -1;
	}
}

void BoardCreator::printCoord(Coordinate c)
{
	std::cout << "Row: " << c.row << ", Column: " << c.col << ", Depth: " << c.depth << std::endl;
}
/*
* Gets:
*		indexes of row, colomn and depth, a dimentionToCheck
* Updates:
*		adds +1 to the relrevant coordinate, as transffered by dimentionToCheck
* Returns:
*		the (actual) updated index
*/
int& BoardCreator::updateCoordinate(int& row_ind, int& col_ind, int& depth_ind, Ship::INDEX_3D dimentionToCheck)
{
	switch (dimentionToCheck)
	{
	case (Ship::INDEX_3D::row_index):
		row_ind++;
		return row_ind;
	case (Ship::INDEX_3D::column_index):
		col_ind++;
		return col_ind;
	default: //dimentionToCheck == INDEX_3D::depth_index
		depth_ind++;
		return depth_ind;
	}
}

/*
*	Gets:
*		currPos - current position
*		dimentionToCheck - tells us whether we need to search for the same letter in rows/columns/depth
*		maxIndex - limit index to check (for exmaple: if maxIndex is 5, we'll check until 4, which is < 5)
*		board - the board :) NOTE: changes content of board!
*		letter - the letter we want to check for multiple appearances
*	Returns:
*		number of consecutive same-letter appearances in dimentionToCheck.
*/
int BoardCreator::checkSequence(const Coordinate& currPos, Ship::INDEX_3D dimentionToCheck, int maxIndex, std::unique_ptr<boardType>& board, char letter)
{
	int shipCells = 1;
	int row_ind = currPos.row;
	int col_ind = currPos.col;
	int depth_ind = currPos.depth;

	while ((updateCoordinate(row_ind, col_ind, depth_ind, dimentionToCheck) < maxIndex) && ((*board)[depth_ind][row_ind][col_ind] == letter))
	{
		shipCells++;
		(*board)[depth_ind][row_ind][col_ind] = DEFAULT_LETTER;
	}
	return shipCells;
}

/*
* Gets:
*		shipCells = number of not-valid-ship-cells that should be inserted to "badLetterCoords"
*		int row, col, depth = the current coordinate that from it and on the bad coords appear
*		updates wrongSizeOrShape to be "true"
*/
void BoardCreator::insertBadCoords(int shipCells, bool& wrongSizeOrShape, std::shared_ptr<std::vector<Coordinate>>& badLetterCoords,
	int row, int col, int depth, Ship::INDEX_3D dimentionToUpdate)
{
	wrongSizeOrShape = true;
	for (int i = 0; i < shipCells; ++i) //insert all "bad indexes" letter appearences into the list
	{
		Coordinate c1 = Coordinate(row, col, depth);
		(*badLetterCoords).push_back(c1);
		updateCoordinate(row, col, depth, dimentionToUpdate);
	}
}

/*
* Creates a ship of relevant size (sizeOfShip(letter)),
* Inserts it to shipsOfPlayer, and updates numShipsForCurrPlayer.
*/
void BoardCreator::createShipAtCoord(char letter, std::vector<std::shared_ptr<Ship>>& shipsOfPlayer, int& numShipsForCurrPlayer,
	int row, int col, int depth, Ship::INDEX_3D dimentionToUpdate)
{
	std::shared_ptr<Ship> ship = std::make_shared<Ship>(letter);
	for (int i = 0; i < Ship::sizeOfShip(letter); ++i)
	{

		(*ship).setPosition(i, row, col, depth, 0);
		//last line used to be: (*(shipsOfPlayer[numShipsForCurrPlayer])).setPosition(i, row, col, depth, 0);
		updateCoordinate(row, col, depth, dimentionToUpdate);

	}
	shipsOfPlayer.push_back(ship);
	numShipsForCurrPlayer++;
}

/*
* Gets a letter and the first index of the ship on the board:
* If there's sequence that might represent a valid ship - creates it
* Changes all ships (valid/invalid) letters to the default letter
* Updates values of:
*		numShipsForCurrPlayer - (+1) if a ship was added
*		wrongSizeOrShape - to "true" if the current letter is from a deformed ship
*		shipsOfPlayer - adds a ship to the players' ships (if ship might be valid)
*		badLetterCoords - if it's not a ship, adds the "bad" coordinates of this letter to a list
*		board - updates each visited cell to the DEFAULT_LETTER
*/
void BoardCreator::checkShipBorders(std::unique_ptr<boardType>& board, int numRows, int numCols, int numDepth, const Coordinate& currPos,
	char letter, int& numShipsForCurrPlayer, std::vector<std::shared_ptr<Ship>>& shipsOfPlayer,
	bool& wrongSizeOrShape, std::shared_ptr<std::vector<Coordinate>>& badLetterCoords)
{
	int shipCells = 1;
	int row = currPos.row;
	int col = currPos.col;
	int depth = currPos.depth;

	(*board)[depth][row][col] = DEFAULT_LETTER;

	//check for ship cells on next columns:
	if ((col < numCols - 1) && ((*board)[depth][row][col + 1]) == letter)
	{
		shipCells = checkSequence(currPos, Ship::INDEX_3D::column_index, numCols, board, letter);
		if (shipCells != Ship::sizeOfShip(letter)) //ship has a wrong size (too small/too big)
		{
			insertBadCoords(shipCells, wrongSizeOrShape, badLetterCoords, row, col, depth, Ship::INDEX_3D::column_index);
		}
		else
		{
			//If gets here,shipCells == Ship::sizeOfShip(letter):
			createShipAtCoord(letter, shipsOfPlayer, numShipsForCurrPlayer, row, col, depth, Ship::INDEX_3D::column_index);
		}
	}
	// Searching rows for the rest of the ship
	else if ((row < numRows - 1) && ((*board)[depth][row + 1][col]) == letter)
	{
		shipCells = checkSequence(currPos, Ship::INDEX_3D::row_index, numRows, board, letter);
		if (shipCells != Ship::sizeOfShip(letter)) //ship has a wrong size (too small/too big)
		{
			insertBadCoords(shipCells, wrongSizeOrShape, badLetterCoords, row, col, depth, Ship::INDEX_3D::row_index);
		}
		else //If gets here,shipCells == Ship::sizeOfShip(letter):
		{
			createShipAtCoord(letter, shipsOfPlayer, numShipsForCurrPlayer, row, col, depth, Ship::INDEX_3D::row_index);
		}

	}
	// Searching depth for the rest of the ship
	else if ((depth < numDepth - 1) && ((*board)[depth + 1][row][col]) == letter)
	{
		shipCells = checkSequence(currPos, Ship::INDEX_3D::depth_index, numDepth, board, letter);
		if (shipCells != Ship::sizeOfShip(letter)) //ship has a wrong size (too small/too big)
		{
			insertBadCoords(shipCells, wrongSizeOrShape, badLetterCoords, row, col, depth, Ship::INDEX_3D::depth_index);
		}
		else //shipCells == Ship::sizeOfShip(letter):
		{
			createShipAtCoord(letter, shipsOfPlayer, numShipsForCurrPlayer, row, col, depth, Ship::INDEX_3D::depth_index);
		}
	}
	else //It's here because the 3 "if"s checked the "next"cell, so we still need to check the case of 1 cell ship
	{
		if (Ship::sizeOfShip(letter) != 1)
		{
			insertBadCoords(shipCells, wrongSizeOrShape, badLetterCoords, row, col, depth, Ship::INDEX_3D::depth_index);
		}
		else
		{
			createShipAtCoord(letter, shipsOfPlayer, numShipsForCurrPlayer, row, col, depth, Ship::INDEX_3D::depth_index);
		}
	}
}


bool BoardCreator::twoCoordsNeighbours(Coordinate c1, Coordinate c2)
{
	bool neighbourXY = (c1.col == c2.col && c1.row == c2.row) && (c1.depth - 1 == c2.depth || c1.depth + 1 == c2.depth);
	bool neighbourYZ = (c1.row == c2.row && c1.depth == c2.depth) && (c1.col - 1 == c2.col || c1.col + 1 == c2.col);
	bool neighbourXZ = (c1.col == c2.col && c1.depth == c2.depth) && (c1.row - 1 == c2.row || c1.row + 1 == c2.row);
	return neighbourXY || neighbourYZ || neighbourXZ;
}

/*
* Returns true if ship is next to another ship/"bad index", of the same letter! (==wrong shape)
* @Params:
*		badLetterCoords - a vector containing all coordinates in which the letter appeard in, and weren't valid
*		ship - the ship we're checking
*		letter - the ship's letter
*/
bool BoardCreator::checkShipShape(std::shared_ptr<Ship>& ship, char letter,
	std::shared_ptr<std::vector<Coordinate>>& badLetterCoords, std::vector<std::shared_ptr<Ship>>& shipsOfPlayer)
{
	bool res = false;
	int** pos = (*ship).getPosition();
	for (int i = 0; i < (*ship).getShipSize(); ++i)
	{
		Coordinate c1 = Coordinate(pos[i][Ship::INDEX_3D::row_index], 
						pos[i][Ship::INDEX_3D::column_index], pos[i][Ship::INDEX_3D::depth_index]);
		for (int k = 0; k < (*badLetterCoords).size(); ++k)
		{
			if (twoCoordsNeighbours(c1, (*badLetterCoords).at(i)))
			{
				res = true;
				break;
			}
		}
		if (res) break;
	}

	if (!res) //now check in "valid" ships
	{
		for (int i = 0; i < shipsOfPlayer.size(); ++i)
		{
			//makes sure it's NOT the same ship: 
			if (!(pos[0][Ship::INDEX_3D::row_index] == (*(shipsOfPlayer.at(i))).getPosition()[0][Ship::INDEX_3D::row_index] &&
				pos[0][Ship::INDEX_3D::column_index] == (*(shipsOfPlayer.at(i))).getPosition()[0][Ship::INDEX_3D::column_index]) &&
				pos[0][Ship::INDEX_3D::depth_index] == (*(shipsOfPlayer.at(i))).getPosition()[0][Ship::INDEX_3D::depth_index])
			{
				if ((*(shipsOfPlayer.at(i))).getLetter() == letter)
				{
					for (int j = 0; j < (*ship).getShipSize(); ++j)
					{
						Coordinate c1 = Coordinate(pos[j][Ship::INDEX_3D::row_index], pos[j][Ship::INDEX_3D::column_index], pos[j][Ship::INDEX_3D::depth_index]);
						for (int k = 0; k < (*shipsOfPlayer.at(i)).getShipSize(); ++k)
						{
							int** position = (*shipsOfPlayer.at(i)).getPosition();
							Coordinate c2 = Coordinate(position[k][Ship::INDEX_3D::row_index], position[k][Ship::INDEX_3D::column_index], position[k][Ship::INDEX_3D::depth_index]);
							if (twoCoordsNeighbours(c1, c2))
							{
								res = true;
								break;
							}
						}
						if (res) break;
					}
				}
			}
			if (res) break;
		}
	}
	if (res)
	{
		for (int i = 0; i < Ship::sizeOfShip(letter); ++i)
		{
			Coordinate badCoord = Coordinate(pos[i][Ship::INDEX_3D::row_index], pos[i][Ship::INDEX_3D::column_index], pos[i][Ship::INDEX_3D::depth_index]);
			(*badLetterCoords).push_back(badCoord);
		}
	}
	return res;
}

/*
*Returns true if there IS an adjacent (different) ship (next to (currentRow,currentCol,currentDepth)).
*/
bool BoardCreator::checkNeighbourShips(std::unique_ptr<boardType>& board, int currentRow, int currentCol,
	int currentDepth, int numRows, int numCols, int numDepth)
{
	char letter = (*board)[currentDepth][currentRow][currentCol];

	if (currentRow - 1 >= 0)
	{
		if (Ship::isShip((*board)[currentDepth][currentRow - 1][currentCol]) && ((*board)[currentDepth][currentRow - 1][currentCol] != letter))
		{
			return true;
		}
	}

	if (currentCol - 1 >= 0)
	{
		if (Ship::isShip((*board)[currentDepth][currentRow][currentCol - 1]) && ((*board)[currentDepth][currentRow][currentCol - 1] != letter))
		{
			return true;
		}
	}

	if (currentDepth - 1 >= 0)
	{
		if (Ship::isShip((*board)[currentDepth - 1][currentRow][currentCol]) && ((*board)[currentDepth - 1][currentRow][currentCol] != letter))
		{
			return true;
		}
	}

	if (currentRow + 1 < numRows)
	{
		if (Ship::isShip((*board)[currentDepth][currentRow + 1][currentCol]) && ((*board)[currentDepth][currentRow + 1][currentCol] != letter))
		{
			return true;
		}
	}

	if (currentCol + 1 < numCols)
	{
		if (Ship::isShip((*board)[currentDepth][currentRow][currentCol + 1]) && ((*board)[currentDepth][currentRow][currentCol + 1] != letter))
		{
			return true;
		}
	}

	if (currentDepth + 1 < numDepth)
	{
		if (Ship::isShip((*board)[currentDepth + 1][currentRow][currentCol]) && ((*board)[currentDepth + 1][currentRow][currentCol] != letter))
		{
			return true;
		}
	}
	return false;
}

std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector >> BoardCreator::checkBoard(std::unique_ptr<boardType>& board, int numRows, int numCols, int numDepth)
{
	ptrToShipsVector shipsA = std::make_shared<std::vector<std::shared_ptr<Ship>>>();
	ptrToShipsVector shipsB = std::make_shared<std::vector<std::shared_ptr<Ship>>>();

	std::vector<std::shared_ptr<std::vector<Coordinate>>> allBadCoordinates(VALID_LETTERS);

	for (int i = 0; i < VALID_LETTERS; ++i)
	{
		allBadCoordinates[i] = std::make_shared<std::vector<Coordinate>>();
	}

	std::set<char> wrongSizeShapeShips;

	int indexShipA = 0;
	int indexShipB = 0;

	bool boardNotValid = false;
	bool wrongSizeOrShape;
	bool adjacentShips = false;
	char letter = '`';

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			for (int depth = 0; depth < numDepth; depth++)
			{
				if (Ship::isShip((*board)[depth][row][col]))
				{
					adjacentShips = checkNeighbourShips(board, row, col, depth, numRows, numCols, numDepth);
				}
				if (adjacentShips) break;
			}
			if (adjacentShips) break;
		}
		if (adjacentShips) break;
	}

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			for (int depth = 0; depth < numDepth; depth++)
			{
				wrongSizeOrShape = false;
				letter = (*board)[depth][row][col];
				int ind = getIndexOfRelevantBadCoordsVector(letter);
				if (Ship::isShip(letter))
				{
					Coordinate c = Coordinate(row, col, depth);
					if (islower(letter))//letter is "player B"'s letter (small letters)
					{
						checkShipBorders(board, numRows, numCols, numDepth, c, letter, indexShipB, *shipsB, wrongSizeOrShape, allBadCoordinates[ind]);
					}
					else //"player A"'s letter
					{
						checkShipBorders(board, numRows, numCols, numDepth, c, letter, indexShipA, *shipsA, wrongSizeOrShape, allBadCoordinates[ind]);
					}
					if (wrongSizeOrShape)
					{
						wrongSizeShapeShips.insert(letter);
					}
				}
			}
		}
	}

	//check for wrong size or shape(in near columns/rows)
	for (int m = 0; m < 2; ++m)
	{
		ptrToShipsVector ships = (m == 0) ? shipsA : shipsB;
		size_t numOfShipsToCheck = (m == 0) ? (*shipsA).size() : (*shipsB).size();
		int originalShipIndex = 0;
		int currShipToCheck = 0;
		while (originalShipIndex < numOfShipsToCheck)
		{
			int ind = getIndexOfRelevantBadCoordsVector((*((*ships).at(currShipToCheck))).getLetter());
			if (checkShipShape((*ships).at(currShipToCheck), (*((*ships).at(currShipToCheck))).getLetter(), allBadCoordinates[ind], (*ships)))//if true, it's invalid ship
			{
				(m == 0) ? indexShipA-- : indexShipB--;
				wrongSizeShapeShips.insert((*(*ships).at(currShipToCheck)).getLetter());
				(*ships).erase((*ships).begin() + currShipToCheck);
				currShipToCheck--;
			}
			currShipToCheck++;
			originalShipIndex++;
		}
	}

	/*As instructed, no need to print errors of the board*/

	if (wrongSizeShapeShips.size() > 0)
	{
		boardNotValid = true;
		/*Printing errors as instructed - wrong size or shape*/

		std::string msg;

		for (std::set<char>::iterator i = wrongSizeShapeShips.begin(); i != wrongSizeShapeShips.end(); ++i)
		{
			if (!islower(*i))
			{
				msg = "Wrong size or shape for ship " + (*i);
				msg += " for player A";

				Logger::instance().log(msg, Logger::kLogLevelError);
			}
		}
		for (std::set<char>::iterator i = wrongSizeShapeShips.begin(); i != wrongSizeShapeShips.end(); ++i)
		{
			if (islower(*i))
			{
				msg = "Wrong size or shape for ship " + *i;
				msg += " for player B";

				Logger::instance().log(msg, Logger::kLogLevelError);
			}
		}
	}

	/*Printing errors as instructed - too many / to few ships*/
	if ((*shipsA).size() > NUMBER_SHIPS)
	{
		boardNotValid = true;
		Logger::instance().log("Too many ships for player A", Logger::kLogLevelError);
	}
	if ((*shipsA).size() < NUMBER_SHIPS)
	{
		boardNotValid = true;
		Logger::instance().log("Too few ships for player A", Logger::kLogLevelError);
	}
	if ((*shipsB).size() > NUMBER_SHIPS)
	{
		boardNotValid = true;
		Logger::instance().log("Too many ships for player B", Logger::kLogLevelError);
	}
	if ((*shipsB).size() < NUMBER_SHIPS)
	{
		boardNotValid = true;
		Logger::instance().log("Too few ships for player B", Logger::kLogLevelError);
	}
	if (adjacentShips)
	{
		boardNotValid = true;
		Logger::instance().log("Adjacent Ships on Board", Logger::kLogLevelError);
	}

	/*If board's invalid - deletes all ships and return nullptr*/
	if (boardNotValid)
	{
		return nullptr;
	}

	std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector >> ret = std::make_shared<std::pair<ptrToShipsVector, ptrToShipsVector >>(std::pair<ptrToShipsVector, ptrToShipsVector >(shipsA, shipsB));
	return ret;
}

/*
* Gets first line of the file, its format is: <number of columns>x<number of rows>x<depth>
* Updates the values of numRows, numCols, numDepth to contain the relevant sizes
* Because uses Utilities::isNumeric(), might throw an exception
*/
void BoardCreator::getDimentions(int & numRows, int & numCols, int & numDepth, std::string line)
{
	std::transform(line.begin(), line.end(), line.begin(), std::tolower); //converts line to small-letters
	if (2 == Utilities::countAppearances(DELIMETER, line)) //makes sure 'x' appears exactly twice in "line"
	{
		size_t firstX = line.find_first_of(DELIMETER);
		size_t secondX = line.find_last_of(DELIMETER);
		std::string cols = line.substr(0, firstX);
		if (Utilities::isNumeric(cols))
		{
			std::string rows = line.substr(firstX + 1, secondX - (firstX + 1));
			if (Utilities::isNumeric(rows))
			{
				std::string depth = line.substr(secondX + 1, line.length() - (secondX + 1));
				if (Utilities::isNumeric(depth))
				{
					numRows = std::stoi(rows);
					numCols = std::stoi(cols);
					numDepth = std::stoi(depth);
				}
			}
		}
	}
}



/*
* This Function returns a pointer to a 3D-game board
* TODO:: update it accurding to: http://moodle.tau.ac.il/mod/forum/discuss.php?d=61341
* Note: might throw exception!
*/
std::unique_ptr<boardType> BoardCreator::getBoardFromFile(const char* boardFile, int& numRows, int& numCols, int& numDepth)
{
	//opening boardfile:
	std::ifstream bfile(boardFile);
	if (!bfile)
	{
		throw Exception("Error: failed opening board file.");
	}
	numRows = -1;
	numCols = -1;
	numDepth = -1;

	std::string line;
	line.clear();

	/*Gets first line of the file, format is: <>x<>x<>*/
	try
	{
		Utilities::getAllKindsOfLine(bfile, line);
		getDimentions(numRows, numCols, numDepth, line);
	}
	catch (std::exception& e)
	{
		bfile.close();
		std::string str = e.what();
		str.insert(0, "Error: failed reading from board file or first line is not of format: <number of columns>x<number of rows>x<depth>: ");
		throw Exception(str.c_str());
	}
	if (numRows < 0 || numCols<0 || numDepth<0)
	{
		throw Exception("Error: wrong board dimentions format");
	}

	std::unique_ptr<boardType> board = std::move(createBoard(numRows, numCols, numDepth));

	int depth = 0;
	while (depth < numDepth)
	{
		int row = -1; //row == -1 means it should be an empty line
		while (row < numRows)
		{
			//reads 1 line from the file:
			line.clear();
			try
			{
				Utilities::getAllKindsOfLine(bfile, line);
			}
			catch (std::exception& e)
			{
				if (bfile.eof())
				{
					break;
				}
				bfile.close();
				std::string str = e.what();
				str.insert(0, "Error: failed reading from board file: ");
				throw Exception(str.c_str());
			}
			if (row == -1) //makes sure there's an empty line between dimention and boards
			{
				if (Utilities::isLineEmpty(line))
				{
					row++;
					continue;
				}
				else
				{
					throw Exception("Error: wrong board format: no empty line");
				}
			}
			if (line.length() < numCols)
			{
				for (size_t col = 0; col < line.length(); ++col)
				{
					(*board)[depth][row][col] = line.at(col);
				}
				for (size_t col = line.length(); col < numCols; ++col)
				{
					(*board)[depth][row][col] = ' ';
				}
			}
			else
			{
				for (int col = 0; col < numCols; ++col)
				{
					(*board)[depth][row][col] = line.at(col);
				}
			}
			++row;
		}

		if (row < numRows) //file ended (got here through break) 
		{
			for (int i = row; i < numRows; ++i)
			{
				for (int j = 0; j < numCols; ++j)
				{
					(*board)[depth][i][j] = ' ';
				}
			}
			++depth;
			break;
		}

		++depth;
	}
	if (depth < numDepth) //file ended (got here through break) 
	{
		for (int k = depth; k < numDepth; ++k)
		{
			for (int i = 0; i < numRows; ++i)
			{
				for (int j = 0; j < numCols; ++j)
				{
					(*board)[k][i][j] = ' ';
				}
			}
		}
	}

	bfile.close();
	return board;
}

/*
* Gets a path,
* If it's a valid path and there exists a board file - updates "boardFile" to contain the name of the board file
* and returns true.
* If path is valid but there's no board file - returns false.
* If path file isn't valid or an error accured - throws exception
*/
bool BoardCreator::findBoardFile(const char* path, size_t pathLen, char** boardFile)
{
	try
	{
		if (!(Utilities::doesPathExist(path)))
		{
			throw Exception(exceptionInfo(WRONG_PATH, path));
		}
	}
	catch (std::exception& e)
	{
		throw e;
	}

	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	bool retVal = false;

	// Check that the input path plus 3 is not longer than MAX_PATH: 3 characters for "\*" plus NULL appended below.
	StringCchLengthA(path, MAX_PATH, &pathLen);
	if (pathLen > (MAX_PATH - 3))
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	// Prepare string for use with FindFile functions: copy the string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path);
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		throw Exception("Error: failed finding files in path");
	}

	// checks all files in the directory in search for ".sboard" files.
	std::string currFileName;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			currFileName = std::string(ffd.cFileName);
			if (Utilities::endsWith(currFileName, std::string(".sboard")))
			{
				*boardFile = new char[currFileName.size() + 1];
				strcpy_s(*boardFile, currFileName.size() + 1, ffd.cFileName);
				retVal = true;
				break;
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
	return retVal;
}


/*
* Initiates an empty board of size: numDepth X numRows X numCols, returns a smart pointer to it.
*/
std::unique_ptr<boardType> BoardCreator::createBoard(int numRows, int numCols, int numDepth)
{
	std::unique_ptr<boardType> board = std::make_unique<boardType>();
	(*board).resize(numDepth);
	for (int d = 0; d < numDepth; d++)
	{
		(*board)[d].resize(numRows);
		for (int r = 0; r < numRows; r++)
		{
			(*board)[d][r].resize(numCols);
			for (int c = 0; c < numCols; c++)
			{
				(*board)[d][r][c] = ' ';
			}
		}
	}
	return std::move(board);
}

/*
* Returns a copy of "board"
*/
std::unique_ptr<boardType> BoardCreator::copyBoard(const std::unique_ptr<boardType>& board, int numRows, int numCols, int numDepth)
{
	std::unique_ptr<boardType> retBoard = std::move(createBoard(numRows, numCols,numDepth));
	for (int depth = 0; depth < numDepth; depth++)
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				(*retBoard)[depth][row][col] = (*board)[depth][row][col];
			}
		}
	}

	return retBoard;
}

/*Prints board layer by layer:*/
void BoardCreator::printBoard(std::shared_ptr<boardType> board, int numRows, int numCols, int numDepth)
{
	for (int d = 0; d < numDepth; d++)
	{
		std::cout << "*******Layer " << d << "is: *******" << std::endl;
		for (int r = 0; r < numRows; r++)
		{
			for (int c = 0; c < numCols; c++)
			{
				std::cout << (*board)[d][r][c];
			}
			std::cout << std::endl;
		}
	}
}

//char ** BoardCreator::createCommonBoard(std::vector<Ship*>* shipsA, std::vector<Ship*>* shipsB)
//{
//	char** commonBoard = getBoardFromShips(shipsA);
//	updateShipsInBoard(commonBoard, shipsB);
//	return commonBoard;
//}
