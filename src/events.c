#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "players.h"
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
