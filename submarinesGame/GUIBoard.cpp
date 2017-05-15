#include "GUIBoard.h"

const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void GUIBoard::printChar(char ch, int player)
{
	setTextColor(getColor(ch, player));
	std::cout << ch;
}

void GUIBoard::updateCor(int row, int col, char ch, int player)
{
	gotoxy(row + OFFSET, col + OFFSET);
	printChar(ch, player);
}

void GUIBoard::updatePosAfterPrint()
{
	gotoxy(BOARD_LENGTH + 2 * OFFSET, 0);
	setVisibilityOfCursor(true);
	setTextColor(textColor::DEFAULT_COLOR);
}


void GUIBoard::printGUI(char ** board, int player) {

	// Prints board
	setVisibilityOfCursor(false);
	for (int indexRow = 0; indexRow < BOARD_LENGTH; indexRow++) {
		for (int indexCol = 0; indexCol < BOARD_LENGTH; indexCol++) {

			char currLetter = board[indexRow][indexCol];
			textColor color = getColor(currLetter, player);

			gotoxy(indexRow+OFFSET, indexCol+OFFSET);
			setTextColor(color);
			std::cout << currLetter;
		}
		std::cout << std::endl;
	}
	setTextColor(textColor::DEFAULT_COLOR);
}

void GUIBoard::updateGUIBoard(char ** board, int damaged, int row, int col, Ship* damagedShip, bool quiet, int delayMS, int player)
{
	if (row < 0 || col < 0 || row >= BOARD_LENGTH || col >= BOARD_LENGTH) return;
	switch(damaged)
	{

	case (HIT):
	case (BAD_HIT):
		board[row][col] = Utilities::HIT_SIGN;
		if (!quiet)
		{
			updateCor(row, col, Utilities::HIT_SIGN, player);
		}
		break;
	case (SUNK):
	case (SELF_DESTRUCT):
		for (int i = 0; i < damagedShip->getShipSize(); ++i){
			board[damagedShip->getPosition()[i][0]][damagedShip->getPosition()[i][1]] = Utilities::SINK_SIGN;
			if (!quiet)
			{
				updateCor(damagedShip->getPosition()[i][0], damagedShip->getPosition()[i][1], Utilities::SINK_SIGN, player);
			}
		}
		break;
	case(MISS):
	default:
		break;
	}
	if (!quiet) Sleep(delayMS);
}

GUIBoard::textColor GUIBoard::getColor(char letter, int player) {
	
	switch (letter) {

	case 'p':
	case 'd':
	case 'b':
	case 'm':
		return textColor::B_COLOR;

	case 'P':
	case 'D':
	case 'B':
	case 'M':
		return textColor::A_COLOR;

	case '*':
		return textColor::HIT_COLOR;

	default:
		return textColor::SINK_COLOR;

	}
}


void GUIBoard::setVisibilityOfCursor(bool show) {

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = show;
	lpCursor.dwSize = 15;
	SetConsoleCursorInfo(consoleHandle, &lpCursor);

}

void GUIBoard::gotoxy(int row, int col)
{
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

void GUIBoard::setTextColor(textColor color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (int)color);
}
