#ifndef FINANCE_H
#define FINANCE_H

#define TAX_BAND_COUNT 4
/* For un-mortgaging, standard Monopoly rules are applied. 
 * The un-mortgage value is 110% of the mortgaged value. */
#define UNMORTGAGE_PERCENT 110

#define LOAN_DURATION          20   // Rule-LK 4 
#define MAX_LOAN_PERCENT       75   // Rule-LK 2
#define LOAN_INTEREST_PERCENT   8   /* Table 9, stable economy.
                                     * TODO: varies with economic condition
                                     * once events exists. */
#include <stdint.h>
#include "players.h"
#include "board.h"

typedef struct GameStat GameStat;

typedef struct {
    char *protect_against;
    int premium;
    int compensation;
} InsuranceType;

typedef struct {
    char *name;
    InsuranceType *type;
} Insurance;

// An unused struct, cannot remove as it would raise errors in the data union 
// inside the Square struct in the board.h
typedef struct {
    int abc;
} Bank;

typedef struct {
    int base_rate;      // percent - 15 at the start of the game
} Tax;

int start_auction(Player *players, Square *board, int square_index, const GameStat *g);

// income tax
int projected_income_tax(const Square *board, int cash);

/* Community Development Fund (square 2): levies 10% of the player's total 
 * property assets at current market rate. Buildings are excluded. */
int pay_development_fund(Player *p, Square *board, const GameStat *g);
int pay_income_tax(Player *p, Square *board, const Tax *t, const GameStat *g);

// mortgaging
bool raise_cash(Player *p, Square *board, int needed, const GameStat *g);
int  unmortgage_square(Player *p, Square *s, const GameStat *g);
int  mortgage_and_pay(Player *p, Square *board, int amount, const GameStat *g);

// banking
void resolve_bank_visit(Player *p, Square *board, int game_round, const GameStat *g);
int  max_loan_amount(const Player *p, const Square *board, const GameStat *g);
void accrue_loan_interest(Player *p);
void check_loan_default(Player *p, Square *board, int game_round, const GameStat *g);

// bankruptcy 
void declare_bankrupt(Player *p);
void liquidate_assets(Player *p, Player *players, Square *board, const GameStat *g);
int  solvent_count(const Player *players);

// depreciation and maintainance 
void age_properties(Square *board);
void degrade_buildings(Square *board);
void perform_maintenance(Player *p, Square *board, const GameStat *g);
int  renovate_property(Player *p, Square *s, const GameStat *g);

#endif /* FINANCE_H */
