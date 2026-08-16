#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "players.h"
/* game.h defines struct GameStat, which events.h only forward-declares -
 * the effect ledger and the event deck both live on it. */
#include "game.h"
#include "events.h"

/* ---- Inflation (Rules-LK 12 to 14) -------------------------------------
 *
 * Rule-LK 12: every ten rounds an inflation rate is generated from a fixed
 * set of possible values. Negative values indicate deflation.
 *
 * Rule-LK 13: inflation modifies property prices, building costs, hotel
 * costs, rental values, insurance premiums, repair costs and loan interest
 * rates. Existing loan rates remain unchanged.
 *
 * Rule-LK 14: New Value = Previous Value x (1 + Inflation Rate).
 *
 * DESIGN NOTE: inflation is permanent and compounding, so unlike the timed
 * economic events it rewrites the board's stored values directly rather than
 * being held as a temporary modifier. Temporary effects cannot mutate stored
 * values, because integer percentage changes are not reversible - a value
 * scaled up and back down does not always return to its starting point.
 * Inflation is never reversed, so that objection does not apply here.
 */

static const int inflation_rates[INFLATION_RATE_COUNT] = { -3, 0, 2, 5, 8, 12 };

int generate_inflation_rate(void){
    return inflation_rates[rand() % INFLATION_RATE_COUNT];
}

/* Rule-LK 14 applied with integer arithmetic, as the program requirements
 * demand. Truncation is toward zero, so repeated small deflations erode a
 * value slightly faster than equivalent inflations restore it. */
int inflate_value(int value, int rate){
    return value + (value * rate / 100);
}

/* Rewrite every value on the board that Rule-LK 13 lists. Squares that are
 * not purchasable carry none of these fields and are skipped. */
void apply_inflation(Square *board, int rate){
    if (rate == 0) return;

    for (int i = 0; i < BOARD_SIZE; i++){
        Square *s = &board[i];

        switch (s->type){
            case PROPERTY: {
                Property *prop = &s->data.property;

                prop->purchase_price   = inflate_value(prop->purchase_price, rate);
                prop->base_rental      = inflate_value(prop->base_rental, rate);
                prop->mortgage_value   = inflate_value(prop->mortgage_value, rate);
                prop->house_const_cost = inflate_value(prop->house_const_cost, rate);
                prop->hotel_const_cost = inflate_value(prop->hotel_const_cost, rate);
                break;
            }

            case RAILWAY: {
                Railway *r = &s->data.railway;

                r->purchase_price = inflate_value(r->purchase_price, rate);
                r->mortgage_value = inflate_value(r->mortgage_value, rate);
                /* base_rental is a lookahead estimate only - actual railway
                 * rent comes from Table 7, which is a fixed schedule. */
                r->base_rental    = inflate_value(r->base_rental, rate);
                break;
            }

            case UTILITY: {
                Utility *u = &s->data.utility;

                u->purchase_price = inflate_value(u->purchase_price, rate);
                u->mortgage_value = inflate_value(u->mortgage_value, rate);
                u->base_rental    = inflate_value(u->base_rental, rate);
                break;
            }

            default:
                break;
        }
    }

    /* TODO: Rule-LK 13 also inflates insurance premiums and repair costs.
     * Neither system exists yet. */
}

/* ---- Active effect ledger ----------------------------------------------
 * Rule-LK 34: when multiple events affect the same target, all percentage
 * changes are cumulative. The spec does not say whether that means additive
 * or compounding.
 *
 * INTERPRETATION: additive. Percentage points are summed and a single
 * multiplication is applied at the end. Two reasons: integer addition is
 * order-independent, so the result does not depend on the order effects
 * happen to sit in the array; and one multiplication means one truncation
 * instead of one per effect, which is materially more accurate under the
 * integer-only arithmetic the assignment requires.
 */

void add_effect(GameStat *g, const char *label, EffectTarget target,
                EffectScope scope, int scope_id, int delta_pct,
                int current_round, int duration){
    if (g->effect_count >= MAX_EFFECTS) return;   /* silently ignore overflow */

    Effect *e = &g->effects[g->effect_count++];

    e->label         = label;
    e->target        = target;
    e->scope         = scope;
    e->scope_id      = scope_id;
    e->delta_pct     = delta_pct;
    e->expires_round = current_round + duration;
}

/* Remove expired effects. Swap-and-pop rather than shifting down: order does
 * not matter because the percentages are summed, so overwriting a dead slot
 * with the last live one is safe and O(1). */
void purge_expired(GameStat *g){
    for (int i = 0; i < g->effect_count; ){
        if (g->game_round >= g->effects[i].expires_round){
            g->effects[i] = g->effects[--g->effect_count];
        } else {
            i++;
        }
    }
}

/* Does this effect apply to this square, seen by this player? */
static bool effect_applies(const Effect *e, const Square *s, int square_index,
                           Owner viewer){
    switch (e->scope){
        case SCOPE_GLOBAL:
            return true;

        /* Appendix A: card effects apply to the player who drew the card. */
        case SCOPE_PLAYER:
            return (Owner)e->scope_id == viewer;

        case SCOPE_GROUP:
            return s->type == PROPERTY &&
                   (int)s->data.property.group == e->scope_id;

        case SCOPE_SQUARE_TYPE:
            return (int)s->type == e->scope_id;

        case SCOPE_SQUARE:
            return square_index == e->scope_id;

        default:
            return false;
    }
}

int effect_pct(const GameStat *g, const Square *s, int square_index,
               EffectTarget target, Owner viewer){
    int total = 100;

    for (int i = 0; i < g->effect_count; i++){
        const Effect *e = &g->effects[i];

        if (e->target != target) continue;
        if (!effect_applies(e, s, square_index, viewer)) continue;

        total += e->delta_pct;
    }

    /* A stack of penalties must not drive a value negative. */
    if (total < 0) total = 0;

    return total;
}

/* Rule-LK 36: the simulation shall display the currently active regional
 * market conditions at the end of every round. */
void print_market_conditions(const GameStat *g){
    if (g->effect_count == 0 && g->inflation_rate == 0) return;

    printf("=========================================\n");
    printf("Current Market Conditions\n");
    printf("=========================================\n\n");

    for (int i = 0; i < g->effect_count; i++){
        const Effect *e = &g->effects[i];
        printf("%s (%+d%%)\n", e->label, e->delta_pct);
        printf("Rounds Remaining : %d\n\n", e->expires_round - g->game_round);
    }

    printf("Inflation\n------------\n%+d%%\n\n", g->inflation_rate);
    printf("=========================================\n\n");
}

/* ---- National Event Cards (Appendix A) ---------------------------------
 * A deck of twenty cards. When a player lands on an Event square the top
 * card is drawn, executed, and returned to the bottom of the deck.
 *
 * Only the cards whose effects are immediate are implemented so far - those
 * that move cash or damage a property need nothing beyond what already
 * exists. The durational cards are listed as TODO against the effect ledger,
 * which requires the value getters to consult GameStat before it can do
 * anything.
 */
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

static const char *card_name(EventCard c){
    switch (c){
        case CARD_TAX_AMNESTY:           return "Tax Amnesty";
        case CARD_GOVERNMENT_GRANT:      return "Government Grant";
        case CARD_NATIONAL_DISASTER:     return "National Disaster";
        case CARD_HEAVY_FLOODS:          return "Heavy Floods";
        case CARD_TOURISM_HYPE:          return "Tourism Hype";
        case CARD_FUEL_SHORTAGE:         return "Fuel Shortage";
        case CARD_POLITICAL_RALLY:       return "Political Rally";
        case CARD_STOCK_MARKET_RISE:     return "Stock Market Rise";
        case CARD_ECONOMIC_DOWNTURN:     return "Economic Downturn";
        case CARD_HOUSING_SUBSIDY:       return "Housing Subsidy";
        case CARD_INTEREST_RATE_CUT:     return "Interest Rate Cut";
        case CARD_INTEREST_RATE_INCREASE:return "Interest Rate Increase";
        case CARD_POWER_FAILURE:         return "Power Failure";
        case CARD_FOREIGN_FUNDING:       return "Foreign Funding";
        case CARD_PORT_EXPANSION:        return "Port Expansion";
        case CARD_FESTIVAL_SEASON:       return "Festival Season";
        case CARD_LABOUR_STRIKE:         return "Labour Strike";
        case CARD_INSURANCE_DISCOUNT:    return "Insurance Discount";
        case CARD_PROPERTY_REVALUATION:  return "Property Revaluation";
        case CARD_CURRENCY_DEPRECIATION: return "Currency Depreciation";
        default:                         return "Unknown Card";
    }
}

/* Fisher-Yates shuffle, so the draw order differs between runs. */
void init_event_deck(EventDeck *d){
    for (int i = 0; i < EVENT_DECK_SIZE; i++) d->cards[i] = i;

    for (int i = EVENT_DECK_SIZE - 1; i > 0; i--){
        int j = rand() % (i + 1);
        int t = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = t;
    }
    d->top = 0;
}

/* Pick a random developed property, for the disaster cards. Returns NULL if
 * no developed property exists. */
static Square *random_developed_property(Square *board){
    int candidates[BOARD_SIZE];
    int n = 0;

    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type != PROPERTY) continue;
        if (get_owner(&board[i]) == OG_BANK) continue;
        if (!is_developed(&board[i])) continue;
        candidates[n++] = i;
    }

    if (n == 0) return NULL;
    return &board[candidates[rand() % n]];
}

void draw_event_card(GameStat *g, Player *p, Player *players, Square *board){
    EventCard card = (EventCard)g->deck.cards[g->deck.top];

    /* Return the card to the bottom: advancing the head over a ring buffer
     * is equivalent, since the drawn card becomes the last one seen again. */
    g->deck.top = (g->deck.top + 1) % EVENT_DECK_SIZE;

    printf("Economic Event\n\n%s\n\n", card_name(card));

    switch (card){

        /* ---- Implemented: immediate effects ---- */

        case CARD_TAX_AMNESTY:
            /* "Each player receives LKR 2,000" */
            for (int i = 0; i < NO_OF_PLAYERS; i++){
                if (players[i].bankrupt) continue;
                players[i].cash += 2000;
            }
            printf("Each player receives LKR 2,000.\n\n");
            break;

        case CARD_GOVERNMENT_GRANT: {
            /* "Random player receives LKR 5,000" */
            int idx = rand() % NO_OF_PLAYERS;
            players[idx].cash += 5000;
            printf("%s receives LKR 5,000.\n\n", player_name(players[idx].id));
            break;
        }

        case CARD_NATIONAL_DISASTER:
        case CARD_HEAVY_FLOODS: {
            /* "Random developed property damaged". Rule-LK 28 already models
             * damage as reduced value and reduced maximum rent, so the same
             * state is reused here rather than inventing a second one.
             * TODO: Rule-LK 10 says an insured property is compensated
             * instead. Insurance is not implemented. */
            Square *victim = random_developed_property(board);

            if (!victim){
                printf("No developed property was affected.\n\n");
                break;
            }

            if (!victim->data.property.structural_damage){
                victim->data.property.structural_damage = true;
                victim->data.property.purchase_price =
                    victim->data.property.purchase_price * 85 / 100;
            }

            printf("Affected Property :\n\n%s.\n\n", victim->name);
            printf("The owner bears the full repair cost.\n\n");
            break;
        }

        /* ---- Not yet implemented ----
         * Every card below needs the effect ledger to be consulted by the
         * value getters, which requires threading GameStat into
         * get_purchase_price, property_rent, get_mortgage_value and the
         * construction costs. The ledger and add_effect exist; the getters
         * do not read them yet.
         *
         * CARD_TOURISM_HYPE           hotels double rent, 5 rounds
         * CARD_FUEL_SHORTAGE          railway rent doubles, 5 rounds
         * CARD_POLITICAL_RALLY        one property closed, 2 rounds
         * CARD_STOCK_MARKET_RISE      all property values +10%
         * CARD_ECONOMIC_DOWNTURN      property values -15%
         * CARD_HOUSING_SUBSIDY        house construction cost -30%
         * CARD_INTEREST_RATE_CUT      loan interest -2%
         * CARD_INTEREST_RATE_INCREASE loan interest +2%
         * CARD_POWER_FAILURE          utility income halved, 3 rounds
         * CARD_FOREIGN_FUNDING        commercial property values +15%
         * CARD_PORT_EXPANSION         railway station values +20%
         * CARD_FESTIVAL_SEASON        hotels receive 50% additional rent
         * CARD_LABOUR_STRIKE          construction suspended, 2 rounds
         * CARD_INSURANCE_DISCOUNT     premiums -20%
         * CARD_PROPERTY_REVALUATION   random group appreciates 15%
         * CARD_CURRENCY_DEPRECIATION  construction costs +10%
         */
        default:
            printf("No effect is applied yet for this card.\n\n");
            break;
    }

    (void)g;
    (void)p;
}
