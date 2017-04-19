#pragma once
#include "Ship.h"
#include "IBattleshipGameAlgo.h"
#include "Player.h"

class Game : public IBattleshipGameAlgo
{
public:
	Game(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips, std::string* fullPathToAttackFileA, std::string* fullPathToAttackFileB);
	~Game();
};

