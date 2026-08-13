#ifndef PLAYERS_H
#define PLAYERS_H

#define NO_OF_PLAYERS 4
// bidding
#define BUY_BOUGHT      1
#define BUY_DECLINED    0
#define BUY_INELIGIBLE (-1)
#define BID_INCREMENT 250

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
    bool in_jail;
    int jail_turns;
    
    int (*buy_property)(struct Player *p, Square *board);
    void (*build_property)(struct Player *p, Square *board, Group target_group);
    int (*bid)(const struct Player *p, int current_bid, int market_value);
} Player;

void initialize_players(Player* players);
bool has_monopoly(Owner owner_id, const Square *board, Group target_group);
Player* find_player(Player *players, Owner id);

#endif /* PLAYERS_H */
