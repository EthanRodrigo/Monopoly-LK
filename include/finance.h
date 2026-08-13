#ifndef FINANCE_H
#define FINANCE_H

#define TAX_BAND_COUNT 4

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
int pay_income_tax(Player *p, const Tax *t);

#endif /* FINANCE_H */
