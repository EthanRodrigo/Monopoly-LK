#ifndef PLAYERS_H
#define PLAYERS_H

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
    int net_worth;
} Player;

void initialize_players(void);

#endif /* PLAYERS_H */
