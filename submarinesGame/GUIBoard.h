#pragma once
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include "sizes.h"
#include "ship.h"
#include "Utilities.h"


class GUIBoard
{
	static const HANDLE console;
	static const int OFFSET = 5;

	enum textColor {
		
		//player A's color - blue
		A_COLOR = 9,

		//player B's color - tourqize
		B_COLOR = 11,
		
		// hit - yellow
		HIT_COLOR_A = 14,

		// hit - 
		HIT_COLOR_B = 13,
		
		// sink color - red
		SINK_COLOR_A = 12,

		// sink color - 
		SINK_COLOR_B = 7,

		// default color - white
		DEFAULT_COLOR = 15

	};


	static void gotoxy(int row, int col);

	static void setTextColor(textColor color);

	static void setVisibilityOfCursor(bool show);

	static textColor getColor(char letter, int player);
	
	static void GUIBoard::printChar(char ch, int player);
	static void GUIBoard::updateCor(int row, int col, char ch, int player);
	GUIBoard() = delete;
	~GUIBoard() = delete;
public:

	static void printGUI(char ** board, int player);
	/*
	 * Updates board, according to damage resault - to represent hits, sinks and ships.
	 */
	static void updateGUIBoard(char** board, int damaged, int row, int col, Ship* damagedShip, bool quiet, int delayMS, int player);
	
	static void updatePosAfterPrint();
};

