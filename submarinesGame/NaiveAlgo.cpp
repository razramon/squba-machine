#include "NaiveAlgo.h"

std::pair<int, int> NaiveAlgo::attack()
{
	std::pair<int, int> attack = this->moves[this->moveNum];
	this->moveNum++;
	return attack;
}

void NaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	
}

void NaiveAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	this->board = new char*[numRows];
	for(int indexRow = 0; indexRow < numRows; indexRow++)
	{
		this->board[indexRow] = new char[numCols];
		for(int indexCol = 0; indexCol < numCols; indexCol++)
		{
			this->board[indexRow][indexCol] = board[indexRow][indexCol];
		}
	}
	this->player = player;
	this->numRows = numRows;
	this->numCols = numCols;
}

bool NaiveAlgo::init(const std::string& path)
{
	bool success = true;

	for (int indexRow = 0; indexRow < numRows; indexRow++)
	{
		this->board[indexRow] = new char[numCols];
		for (int indexCol = 0; indexCol < numCols; indexCol++)
		{
			if (isShip(this->board[indexRow][indexCol]))
			{
				
				std::pair<int, int> possibleAttackPos;
				possibleAttackPos.first = indexRow;
				possibleAttackPos.second = indexCol;
				this->moves.push_back(possibleAttackPos);
			}
		}
	}
	

	return success;
}

bool NaiveAlgo::isShip(char c)
{
	c = tolower(c);
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}