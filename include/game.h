#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "players.h"

#define MAX_PLAYERS 4
#define MAX_ROUNDS 500

typedef struct {
    Square board[BOARD_SIZE];
    Player players[MAX_PLAYERS];
    int current_player;
} Game;

void find_roll_order(int* play_order, int* sum, int len);
int roll();
void start_simulation(void);

#endif /* GAME_H */
