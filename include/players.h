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

typedef struct Player {
    PlayerType id;
    Owner owner_id;
    int cash;
    int net_worth;
    int position;
    int player_rounds;
    bool in_jail;
    int jail_turns;
    int loan_amount;
    int loan_round;
    int loan_duration;
    bool bankrupt;

    int (*buy_property)(struct Player *p, Square *board);
    void (*build_property)(struct Player *p, Square *board, Group target_group);
    int (*bid)(const struct Player *p, int current_bid, int market_value);
    /* Rule-LK 5: which of the five bank actions this strategy takes.
     * Returns a LoanAction; finance.c executes it. */
    LoanDecision (*loan_action)(const struct Player *p, const Square *board, int max_loan);
} Player;

void initialize_players(Player* players);
bool has_monopoly(Owner owner_id, const Square *board, Group target_group);
Player* find_player(Player *players, Owner id);

const char *player_name(PlayerType id);
const char *lkr(int amount);

/*
 * TODO: the insurance, loan, interest and tax-due terms are all zero until
 * finance.c exists. */
int calculate_net_worth(const Player *p, const Square *board);

/* Counts for the round summary block. */
int count_properties(const Player *p, const Square *board);
int count_hotels(const Player *p, const Square *board);
int total_property_value(const Player *p, const Square *board);

#endif /* PLAYERS_H */
