#ifndef PLAYERS_H
#define PLAYERS_H

#define NO_OF_PLAYERS 4

typedef enum {
    AGGRESSIVE_INVESTOR,
    CONSERVATIVE_BANKER,
    RISK_TAKER,
    OPPORTUNISTIC_TRADER
} PlayerType;

typedef enum {
    OG_BANK,
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4
} Owner;

// Forward declarion of Square so Player knows it exists
typedef struct Square Square;

typedef struct Player {
    PlayerType id;
    Owner owner_id;
    int cash;
    int net_worth;
    int position;
    int player_rounds;
    
    void (*buy_property)(struct Player *p, Square *s);
} Player;

void initialize_players(Player* players);

// Buying functions for each player
// TODO: Implement the buys
void aggressive_buy(Player *p, Square *board);
void conservative_buy(Player *p, Square *board);
void risky_buy(Player *p, Square *board);
void opportunistic_buy(Player *p, Square *board);

#endif /* PLAYERS_H */
