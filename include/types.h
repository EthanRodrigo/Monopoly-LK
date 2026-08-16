#ifndef TYPES_H
#define TYPES_H

/*
 * This file contains types which are required by multiple files to avoid circular dependencies
 * */

#define MAX_EFFECTS 64

/* Which quantity an effect modifies. Rule-LK 13, 18, 24, 31, 32 and Table 4
 * between them touch all of these. */
typedef enum {
    EFF_PURCHASE_PRICE,
    EFF_RENT,
    EFF_MORTGAGE_VALUE,
    EFF_BUILD_COST,
    EFF_PROPERTY_VALUE,
    EFF_LOAN_INTEREST,
    EFF_INSURANCE_PREMIUM
} EffectTarget;

/* Who or what the effect applies to. */
typedef enum {
    SCOPE_GLOBAL,        /* every square, every player */
    SCOPE_PLAYER,        /* Appendix A: cards affect only the drawing player */
    SCOPE_GROUP,         /* Rule-LK 30: one colour group */
    SCOPE_SQUARE_TYPE,   /* RAILWAY, UTILITY - Fuel Crisis, Port Expansion */
    SCOPE_SQUARE         /* Table 4: one named square */
} EffectScope;

/* A single active modifier.
 *
 * DESIGN: base values on the board are never mutated by a temporary effect.
 * Integer percentage changes are not reversible - 350 raised 15% and lowered
 * back gives 349 - so a mutate-then-undo scheme corrupts the board over 500
 * rounds. Instead the base stays untouched, effects are held here with an
 * expiry round, and the getters apply them at read time. Expiry is simply
 * removal from this array; nothing is ever undone.
 *
 * Inflation (Rule-LK 14) is the deliberate exception: it is permanent and
 * compounding, so it rewrites stored values directly and never appears here.
 */
typedef struct {
    const char *label;      /* for the Rule-LK 36 market conditions block */
    EffectTarget target;
    EffectScope  scope;
    int   scope_id;         /* Group, SquareType, square index, or Owner */
    int   delta_pct;        /* signed percentage points: +25, -30 */
    int   expires_round;    /* absolute round; effect is dead once reached */
} Effect;

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
