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
#include "GameManager.h"
#include "../Common/Logger.h"

#define THREADS_DEFAULT_NUMBER 4;

int main(int argc, char* argv[]);


/*
* Frees all dlls' libraries
*/
void freeDlls();

