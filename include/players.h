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

typedef struct {
    PlayerType type;
    int cash;
    int net_worth;
    int position;
} Player;

void initialize_players(Player* players);

#endif /* PLAYERS_H */
