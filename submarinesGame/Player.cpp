#include "Player.h"
#include <iostream>
#include "Ship.h"
#include "Sizes.h"
#include <vector>
#include "Utilities.h"

using namespace std;

std::vector<std::pair<int, int>> Player::getAttackFile(const char * attackFile)
{
	vector<pair<int, int>> attacks;

	//opening attackFile:
	std::ifstream bfile(attackFile);
	if (!bfile) {
		throw Exception("Error: failed opening attack file.");
	}
	std::string line;
	while (!bfile.eof())
	{
		try
		{
			Utilities::getAllKindsOfLine(bfile, line);
			if (line == "" || line.find(",") == std::string::npos || line.find_first_of(",") != line.find_last_of(","))
				continue;
		}
		catch (std::exception& e)
		{
			//TODO:: print the relevant messege. this one is temporary - meital: think i fixed it.. check it :)
			std::string s("Error: failed when reading attack file: ");
			s.append(e.what());
			throw Exception(s.c_str());
		}

		// Getting each number, deleting spaces and turning to integer
		std::string temp = line.substr(0, line.find(","));
		std::string row = Utilities::delSpaces(temp);
		temp.clear();
		temp = line.substr(line.find(",") + 1, line.length());
		std::string col = Utilities::delSpaces(temp);
		pair<int, int> attack;

		attack.first = stoi(row);
		attack.second = stoi(col);

		// Check if in range
		if (attack.first > 10 || attack.first < 1 || attack.second > 10 || attack.second < 1)
		{
			continue;
		}
		attacks.push_back(attack);
	}
	return attacks;
}

Player::Player(int playerNum, std::string fullPathToAttackFile, Ship* ships)
{
	if((playerNum!=PLAYER_A)&&(playerNum!=PLAYER_B))
	{
		throw Exception("Error: trying to create a player with a wrong player-number.");
	}
	this->playerNum = playerNum;

	try
	{
		attacks = getAttackFile(fullPathToAttackFile.c_str());
	} catch (exception &e)
	{
		throw e;
	}
	this->attackNumber = 0;

	this->ships = new Ship[NUMBER_SHIPS];
	for (int i = 0; i < NUMBER_SHIPS; ++i)
	{
		this->ships[i] = ships[i];
	}
}

Player::~Player()
{
	for (int i = 0; i < NUMBER_SHIPS; ++i)
	{
		delete &ships[i]; //TODO:: check this.. also make sure no memory leaks accure!
	}
	delete[] ships;
}

void Player::attackResOnPlayer(char shipLetter, int row, int col, AttackResult result)
{
	//TODO::complete

}

std::pair<int, int> Player::getAttack(int i)
{
	//TODO::complete
	return std::pair<int, int>();
}

int Player::getAttackNumber() const
{
	return attackNumber;
}
