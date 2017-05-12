//#pragma once
//#include <conio.h>
//#include <Windows.h>
//#include <iostream>
//#include <stdlib.h>
//#include "Sizes.h"
//#include "Ship.h"
//HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
//
//class GUIBoard {
//	enum class textColor : int {
//		// Dark green
//		SHIP_P_SMALL_COLOR = 2,
//
//		// Dark red
//		SHIP_P_BIG_COLOR = 4,
//		
//		// Hit enemy color
//		HIT_ENEMY = 8,
//
//		// Light green
//		SHIP_D_SMALL_COLOR = 10,
//
//		// Light red
//		SHIP_D_BIG_COLOR = 12,
//
//		// Self hit
//		SELF_HIT = 15,
//
//		// Blue for the sea
//		BLUE_SEA = 17,
//
//		// Dark green blue
//		SHIP_B_SMALL_COLOR = 18,
//
//		// Dark red blue
//		SHIP_B_BIG_COLOR = 20,
//
//		// Light green blue
//		SHIP_M_SMALL_COLOR = 26,
//	
//		// Light red blue
//		SHIP_M_BIG_COLOR = 28
//
//	};
//
//	void printWithQuiet(char ** board, int player);
//
//	textColor getColor(char letter, int player);
//
//	GUIBoard::textColor getColorForShip(char letter, int player);
//
//	void setVisibilityOfCursor(bool show);
//
//	void clearScreenForNextMove();
//
//	void gotoxy(int column, int line);
//
//	void setTextColor(textColor color);
//
//
//};
//
