#pragma once

#define WRONG_SIZE_SHAPE(shipLetter, playerLetter) ("Wrong size or shape for ship " + shipLetter + " for player "+ playerLetter)
#define TOO_MANY_SHIPS(playerLetter) ("Too many ships for player "+ playerLetter)
#define TOO_FEW_SHIPS(playerLetter) ("Too few ships for player "+ playerLetter)
#define AJACENT_SHIPS ("Adjacent Ships on Board")
#define WRONG_PATH(path) ("Wrong path: " + path)
#define MISSING_BOARD(path) ("Missing board file (*.sboard) looking in path: " + path)
#define MISSING_ATTACK_FILE(playerLetter, path) ("Missing attack file for player " + playerLetter + " (*.attack-a) looking in path: "+ path)