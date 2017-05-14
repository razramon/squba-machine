#pragma once
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include "sizes.h"
#include "ship.h"
//handle console = getstdhandle(std_output_handle);

class GUIBoard
{
	enum textColor {
		
		// dark green
		SHIP_P_SMALL_COLOR = 2,

		// light green
		SHIP_D_SMALL_COLOR = 10,

		// dark green blue
		SHIP_B_SMALL_COLOR = 18,

		// light green blue
		SHIP_M_SMALL_COLOR = 26,

		// dark red
		SHIP_P_BIG_COLOR = 4,

		// light red
		SHIP_D_BIG_COLOR = 12,

		// dark red blue
		SHIP_B_BIG_COLOR = 20,

		// light red blue
		SHIP_M_BIG_COLOR = 28,
		
		// hit
		HIT_COLOR = 15,
		
		// blue  - sink color
		SINK_COLOR = 17

		// hit enemy color
		//hit_enemy = 8,

	};


	static void gotoxy(int column, int line);

	static void setTextColor(textColor color);

	static void setVisibilityOfCursor(bool show);

	static void clearScreenForNextMove();

	static textColor getColor(char letter, int player);

	GUIBoard() = delete;
	~GUIBoard() = delete;
public:

	static void printGUI(char ** board, int player);

};

