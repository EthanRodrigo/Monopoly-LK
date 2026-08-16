#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "board.h"
#include "players.h"
#include "types.h"

#define MAX_ROUNDS 500

// TODO: remove if ain't using
typedef struct {
    Square board[BOARD_SIZE];
    Player players[NO_OF_PLAYERS];
    int current_player;
} Game;

typedef struct GameStat {
    uint8_t players_passed_go;   /* a bitmap for all 4 players */
    int game_round;
    int last_roll;               /* dice total from the most recent move,
                                  * needed for utility rent (Table 8) */

    /* Rule-LK 12: the inflation rate currently in force, as a percentage.
     * Redrawn every ten rounds and applied once, compounding (Rule-LK 14). */
    int inflation_rate;

    /* Appendix A: the twenty National Event Cards, drawn from the top and
     * returned to the bottom. */
    EventDeck deck;

    /* Rules-LK 18, 24, 30-34 and Table 4: temporary modifiers with expiry
     * rounds. Base values on the board are never mutated by these, because
     * integer percentage changes are not reversible - the getters apply
     * them at read time instead. */
    Effect effects[MAX_EFFECTS];
    int    effect_count;

    void (*mark_player_game_rounds)(uint8_t* bitmap, int player_id);
    void (*reset_player_game_rounds)(uint8_t* bitmap);
} GameStat;

void find_roll_order(int* play_order, int* sum, int len);
void start_simulation(void);

#endif /* GAME_H */
