#ifndef TYPES_H
#define TYPES_H

/*
 * This file contains types which are required by multiple files to avoid circular dependencies
 * */

typedef enum {
    BROWN,
    LIGHT_BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE
} Group;

typedef enum {
    OG_BANK,
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4
} Owner;

typedef enum {
    LOAN_DO_NOTHING,
    LOAN_OBTAIN,
    LOAN_REPAY_PART,
    LOAN_REPAY_FULL,
    LOAN_EXTEND,
    LOAN_INCREASE
} LoanAction;

/* Rule-LK 5 decision: which transaction, and how much. Carrying both in one
 * return value means a strategy cannot choose an action and forget the
 * amount. `amount` is ignored for REPAY_FULL, EXTEND and DO_NOTHING. */
typedef struct {
    LoanAction action;
    int amount;
} LoanDecision;

#endif /* TYPES_H */
