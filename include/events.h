#ifndef EVENTS_H
#define EVENTS_H

#include "types.h"

#define INFLATION_RATE_COUNT 6
#define EVENT_DECK_SIZE 20

/* This is used in the Square struct inside the data union. So removing will give a headache */
typedef struct {
    int unused;
} Event;

/* Appendix A: the twenty National Event Cards. 
 * Cards are drawn from the top of the deck and returned to the bottom after execution, 
 * so the deck is a fixed-size circular queue - one moving index over an array that is never
 * added to or removed from. */
typedef enum {
    CARD_TAX_AMNESTY,
    CARD_GOVERNMENT_GRANT,
    CARD_NATIONAL_DISASTER,
    CARD_HEAVY_FLOODS,
    CARD_TOURISM_HYPE,
    CARD_FUEL_SHORTAGE,
    CARD_POLITICAL_RALLY,
    CARD_STOCK_MARKET_RISE,
    CARD_ECONOMIC_DOWNTURN,
    CARD_HOUSING_SUBSIDY,
    CARD_INTEREST_RATE_CUT,
    CARD_INTEREST_RATE_INCREASE,
    CARD_POWER_FAILURE,
    CARD_FOREIGN_FUNDING,
    CARD_PORT_EXPANSION,
    CARD_FESTIVAL_SEASON,
    CARD_LABOUR_STRIKE,
    CARD_INSURANCE_DISCOUNT,
    CARD_PROPERTY_REVALUATION,
    CARD_CURRENCY_DEPRECIATION
} EventCard;

typedef struct {
    int cards[EVENT_DECK_SIZE];   // card ids, in draw order 
    int top;                      // index of the next card to draw 
} EventDeck;

/* Forward declarations - events.c operates on the board and on game state,
 * but events.h is included by board.h, so the real types are not visible here yet. */
typedef struct GameStat GameStat;
typedef struct Player Player;
typedef struct Square Square;

// Events
void init_event_deck(EventDeck *d);
void draw_event_card(GameStat *g, Player *p, Player *players, Square *board);
void add_effect(GameStat *g, const char *label, EffectTarget target,
                EffectScope scope, int scope_id, int delta_pct,
                int current_round, int duration);
void purge_expired(GameStat *g);
int effect_pct(const GameStat *g, const Square *s, int square_index,
               EffectTarget target, Owner viewer);

void print_market_conditions(const GameStat *g);

// Economic events, regulations and the property market
void trigger_economic_event(GameStat *g);
void trigger_regulation(GameStat *g);
void review_property_market(GameStat *g);
bool construction_allowed(const GameStat *g);

// Inflation
int  generate_inflation_rate(void);
void apply_inflation(Square *board, int rate);
int  inflate_value(int value, int rate);

#endif /* EVENTS_H */
