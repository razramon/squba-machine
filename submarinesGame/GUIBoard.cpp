//#include "GUIBoard.h"
//void GUIBoard::printWithQuiet(char ** board, int player) {
//
//	// Print the board
//	for (int indexRow = 0; indexRow < BOARD_LENGTH; indexRow++) {
//		for (int indexCol = 0; indexCol < BOARD_LENGTH; indexCol++) {
//
//			char currLetter = board[indexRow][indexCol];
//			textColor color = GUIBoard::getColor(currLetter, player);
//
//			gotoxy(indexRow, indexCol);
//			setTextColor(color);
//			std::cout << currLetter;
//		}
//		std::cout << std::endl;
//	}
//}
//
//GUIBoard::textColor GUIBoard::getColor(char letter, int player) {
//
//	textColor tc;
//	// TODO: add hit self and hit enemy color
//	switch (letter) {
//
//	case 'p':
//		tc = GUIBoard::textColor::SHIP_P_SMALL_COLOR;
//		break;
//
//	case 'd':
//		tc = GUIBoard::textColor::SHIP_D_SMALL_COLOR;
//		break;
//
//	case 'b':
//		tc = GUIBoard::textColor::SHIP_B_SMALL_COLOR;
//		break;
//
//	case 'm':
//		tc = GUIBoard::textColor::SHIP_M_SMALL_COLOR;
//		break;
//
//	case 'P':
//		tc = GUIBoard::textColor::SHIP_P_BIG_COLOR;
//		break;
//
//	case 'D':
//		tc = GUIBoard::textColor::SHIP_D_BIG_COLOR;
//		break;
//
//	case 'B':
//		tc = GUIBoard::textColor::SHIP_B_BIG_COLOR;
//		break;
//
//	case 'M':
//		tc = GUIBoard::textColor::SHIP_M_BIG_COLOR;
//		break;
//	default:
//		tc = GUIBoard::textColor::BLUE_SEA;
//		break;
//	}
//
//
//	return tc;
//}
//
//}
//void GUIBoard::setVisibilityOfCursor(bool show) {
//
//	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_CURSOR_INFO lpCursor;
//	lpCursor.bVisible = show;
//	lpCursor.dwSize = 100;
//	SetConsoleCursorInfo(consoleHandle, &lpCursor);
//
//}
//
//void GUIBoard::clearScreenForNextMove()
//{
//	system("cls");
//}
//
//void GUIBoard::gotoxy(int column, int line)
//{
//	COORD coord;
//	coord.X = column;
//	coord.Y = line;
//	SetConsoleCursorPosition(
//		GetStdHandle(STD_OUTPUT_HANDLE),
//		coord
//	);
//}
//
//void GUIBoard::setTextColor(textColor color)
//{
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(hConsole, (int)color);
//}