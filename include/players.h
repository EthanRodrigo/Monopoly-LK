#ifndef PLAYERS_H
#define PLAYERS_H

#define NO_OF_PLAYERS 4

#include <stdbool.h>
#include "types.h"

typedef enum {
    AGGRESSIVE_INVESTOR,
    CONSERVATIVE_BANKER,
    RISK_TAKER,
    OPPORTUNISTIC_TRADER
} PlayerType;

// Forward declarion of Square so Player knows it exists
typedef struct Square Square;

typedef struct Player { PlayerType id;
    Owner owner_id;
    int cash;
    int net_worth;
   int position;
    int player_rounds;
    
    void (*buy_property)(struct Player *p, Square *board);
    void (*build_property)(struct Player *p, Square *board, Group target_group);
} Player;

void initialize_players(Player* players);
bool has_monopoly(Owner owner_id, const Square *board, Group target_group);

#endif /* PLAYERS_H */
