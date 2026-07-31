#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "players.h"

#define MAX_PLAYERS 4

typedef struct {
    Square board[BOARD_SIZE];
    Player players[MAX_PLAYERS];
    int current_player;
} Game;

#endif /* GAME_H */
