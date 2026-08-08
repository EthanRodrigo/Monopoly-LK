#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "board.h"
#include "players.h"

#define MAX_ROUNDS 500

// TODO: remove if ain't using
typedef struct {
    Square board[BOARD_SIZE];
    Player players[NO_OF_PLAYERS];
    int current_player;
} Game;

typedef struct {
    uint8_t players_passed_go;  // a bitmap for all 4 players
    int game_round;
    void (*mark_player_game_rounds)(uint8_t* bitmap, int player_id);
    void (*reset_player_game_rounds)(uint8_t* bitmap);
} GameStat;

void find_roll_order(int* play_order, int* sum, int len);
void start_simulation(void);

#endif /* GAME_H */
