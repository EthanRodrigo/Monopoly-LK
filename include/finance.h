#ifndef FINANCE_H
#define FINANCE_H

#define TAX_BAND_COUNT 4
/* Standard Monopoly rules are applied where the spec is silent: mortgaging
 * pays the mortgage value in cash, and lifting it costs 110% of that.
 * Without the premium, mortgaging would be free money and every strategy
 * would mortgage everything immediately. */
#define UNMORTGAGE_PERCENT 110

/* ---- Loans -------------------------------------------------------------
 * Rule-LK 1-7. Loans are available only on the Bank of Ceylon square, and a
 * player may hold only one at a time. */
#define LOAN_DURATION          20   /* Rule-LK 4 */
#define MAX_LOAN_PERCENT       75   /* Rule-LK 2 */
#define LOAN_INTEREST_PERCENT   8   /* Table 9, stable economy.
                                     * TODO: varies with economic condition
                                     * once events.c exists. */
#include <stdint.h>
#include "players.h"
#include "board.h"

typedef struct {
    char *protect_against;
    int premium;
    int compensation;
} InsuranceType;

typedef struct {
    char *name;
    InsuranceType *type;
} Insurance;

typedef struct {
    int abc;
} Bank;

typedef struct {
    int free_allowance;
    int band_width;
    int rates[TAX_BAND_COUNT];   // marginal %, last applies to all above 
} Tax;

int start_auction(Player *players, Square *board, int square_index);
int pay_income_tax(Player *p, Square *board, const Tax *t);

bool raise_cash(Player *p, Square *board, int needed);
int  unmortgage_square(Player *p, Square *s);
int  mortgage_and_pay(Player *p, Square *board, int amount);

void resolve_bank_visit(Player *p, Square *board, int game_round);
int  max_loan_amount(const Player *p, const Square *board);
void accrue_loan_interest(Player *p);
void check_loan_default(Player *p, Square *board, int game_round);

/* Prices the income tax bill a player would face at the given cash level.
 * Exposed so strategies can reason about an upcoming liability. */
int projected_income_tax(const Square *board, int cash);

/* ---- Bankruptcy (Rule 14) ---------------------------------------------- */
void declare_bankrupt(Player *p);
void liquidate_assets(Player *p, Player *players, Square *board);
int  solvent_count(const Player *players);

/* ---- Depreciation and maintenance (Rules-LK 15-17, 25-29) -------------- */
void age_properties(Square *board);
void degrade_buildings(Square *board);
void perform_maintenance(Player *p, Square *board);
int  renovate_property(Player *p, Square *s);

#endif /* FINANCE_H */
