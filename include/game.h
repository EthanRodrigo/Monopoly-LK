#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "board.h"
#include "players.h"
#include "types.h"

#define MAX_ROUNDS 500

typedef struct GameStat {
    uint8_t players_passed_go;    // a bitmap to maintain players passing GO
    int game_round;
    int last_roll;               // dice total from the most recent move,
    int inflation_rate; //The inflation rate currently in force, as a percentage.
    EventDeck deck;     // National Event Cards deck
    int construction_blocked_until;     // Some events blocks construction

    /* Rule-LK 33: a group affected by a boom or decline cannot be selected
     * again until thirty rounds have elapsed. */
    int group_last_event[8];    

    Effect effects[MAX_EFFECTS];
    int    effect_count;
} GameStat;

void find_roll_order(int* play_order, int* sum, int len);
void start_simulation(void);

#endif /* GAME_H */
