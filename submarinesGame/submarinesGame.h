#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include <iostream>
#include "Sizes.h"
#include "BoardCreator.h"
#include <chrono>
#include <future>
#include <thread>
#include "Utilities.h"
#include <stdlib.h>
#include "Game.h"
#include "GameInfo.h"

#define THREADS_DEFAULT_NUMBER 4;

std::vector<HINSTANCE> dlls;
int main(int argc, char* argv[]);
int gameNumber;
shared_ptr<std::vector<shared_ptr<GameInfo>>> allGamesData;
typedef std::vector<std::vector<std::vector<char>>> boardType;

/*
* Frees all dlls' libraries
*/
void freeDlls();

void runGameThread(GameInfo* game);
