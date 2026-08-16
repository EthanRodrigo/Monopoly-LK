#ifndef EVENTS_H
#define EVENTS_H

#include "types.h"

/* Rule-LK 12: possible inflation rates, drawn every ten rounds.
 * Negative values are deflation. */
#define INFLATION_RATE_COUNT 6

/* Appendix A: the deck holds twenty National Event Cards. */
#define EVENT_DECK_SIZE 20

/* Appendix A gives all three Event squares identical behaviour - draw from a
 * single shared deck held on GameStat. The square itself carries no data, so
 * this member exists only to keep the union complete. */
typedef struct {
    int unused;
} Event;

/* Appendix A: the twenty National Event Cards. Cards are drawn from the top
 * of the deck and returned to the bottom after execution, so the deck is a
 * fixed-size circular queue - one moving index over an array that is never
 * added to or removed from. */
typedef struct {
    int cards[EVENT_DECK_SIZE];   /* card ids, in draw order */
    int top;                      /* index of the next card to draw */
} EventDeck;

/* Forward declarations - events.c operates on the board and on game state,
 * but events.h is included by board.h, so the real types are not visible
 * here yet. */
typedef struct GameStat GameStat;
typedef struct Player Player;
typedef struct Square Square;

void init_event_deck(EventDeck *d);
void draw_event_card(GameStat *g, Player *p, Player *players, Square *board);

void add_effect(GameStat *g, const char *label, EffectTarget target,
                EffectScope scope, int scope_id, int delta_pct,
                int current_round, int duration);
void purge_expired(GameStat *g);

/* Total percentage adjustment applying to one square for one viewer.
 * Returns 100 when nothing applies, so the caller computes
 *   value = (base * pct + 50) / 100
 */
int effect_pct(const GameStat *g, const Square *s, int square_index,
               EffectTarget target, Owner viewer);

void print_market_conditions(const GameStat *g);

/* Rule-LK 12-14: inflation. Every ten rounds a rate is generated and applied
 * to every value the spec lists in Rule-LK 13. The effect compounds, so the
 * board's stored values are rewritten in place rather than tracked as a
 * temporary modifier. */
int  generate_inflation_rate(void);
void apply_inflation(Square *board, int rate);
int  inflate_value(int value, int rate);

#endif /* EVENTS_H */
