#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "players.h"
#include "game.h"
#include "events.h"

static const int inflation_rates[INFLATION_RATE_COUNT] = { -3, 0, 2, 5, 8, 12 };

int generate_inflation_rate(void){
    return inflation_rates[rand() % INFLATION_RATE_COUNT];
}

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
 * not matter because the percentages are summed. */
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

/* Total percentage adjustment applying to one square for one viewer.
 * Returns 100 when nothing applies, so the caller computes
 *   value = (base * pct + 50) / 100 */
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
             /* TODO: Rule-LK 10 says an insured property is compensated
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

        /* ---- Durational effects, applied through the ledger ---- */

        case CARD_TOURISM_HYPE:
            add_effect(g, "Tourism Hype", EFF_RENT, SCOPE_PLAYER,
                       p->owner_id, 100, g->game_round, 5);
            printf("Hotels earn double rent for 5 rounds.\n\n");
            break;

        case CARD_FESTIVAL_SEASON:
            add_effect(g, "Festival Season", EFF_RENT, SCOPE_PLAYER,
                       p->owner_id, 50, g->game_round, 15);
            printf("Hotels receive 50%% additional rent.\n\n");
            break;

        case CARD_FUEL_SHORTAGE:
            add_effect(g, "Fuel Shortage", EFF_RENT, SCOPE_SQUARE_TYPE,
                       RAILWAY, 100, g->game_round, 5);
            printf("Railway rent doubles for 5 rounds.\n\n");
            break;

        case CARD_POWER_FAILURE:
            add_effect(g, "Power Failure", EFF_RENT, SCOPE_SQUARE_TYPE,
                       UTILITY, -50, g->game_round, 3);
            printf("Utility income halved for 3 rounds.\n\n");
            break;

        case CARD_PORT_EXPANSION:
            add_effect(g, "Port Expansion", EFF_PURCHASE_PRICE,
                       SCOPE_SQUARE_TYPE, RAILWAY, 20, g->game_round, 15);
            printf("Railway station values increase by 20%%.\n\n");
            break;

        case CARD_STOCK_MARKET_RISE:
            add_effect(g, "Stock Market Rise", EFF_PURCHASE_PRICE,
                       SCOPE_GLOBAL, 0, 10, g->game_round, 15);
            printf("All property values increase by 10%%.\n\n");
            break;

        case CARD_ECONOMIC_DOWNTURN:
            add_effect(g, "Economic Downturn", EFF_PURCHASE_PRICE,
                       SCOPE_GLOBAL, 0, -15, g->game_round, 15);
            printf("Property values decrease by 15%%.\n\n");
            break;

        case CARD_HOUSING_SUBSIDY:
            add_effect(g, "Housing Subsidy", EFF_BUILD_COST,
                       SCOPE_GLOBAL, 0, -30, g->game_round, 15);
            printf("House construction cost reduced by 30%%.\n\n");
            break;

        case CARD_CURRENCY_DEPRECIATION:
            add_effect(g, "Currency Depreciation", EFF_BUILD_COST,
                       SCOPE_GLOBAL, 0, 10, g->game_round, 15);
            printf("Construction costs increase by 10%%.\n\n");
            break;

        case CARD_PROPERTY_REVALUATION: {
            int grp = rand() % 8;
            add_effect(g, "Property Revaluation", EFF_PURCHASE_PRICE,
                       SCOPE_GROUP, grp, 15, g->game_round, 15);
            printf("A random property group appreciates by 15%%.\n\n");
            break;
        }

        case CARD_POLITICAL_RALLY: {
            /* "One random property closed for 2 rounds" - modelled as -100%
             * rent on that property's group, which is what closure means for
             * income. The ledger targets groups rather than single squares,
             * so this is broader than the card states. */
            Square *victim = random_developed_property(board);
            if (victim){
                add_effect(g, "Political Rally", EFF_RENT, SCOPE_GROUP,
                           (int)victim->data.property.group, -100,
                           g->game_round, 2);
                printf("%s is closed for 2 rounds.\n\n", victim->name);
            } else {
                printf("No property was affected.\n\n");
            }
            break;
        }

        /* ---- Cards that need systems which do not exist ---- */

        case CARD_INTEREST_RATE_CUT:
        case CARD_INTEREST_RATE_INCREASE:
            /* TODO: Rule-LK 13 says existing loan rates remain unchanged, so
             * this must move the Bank's offered rate rather than live loans.
             * LOAN_INTEREST_PERCENT is still a compile-time constant. */
            printf("(Not applied - the Bank rate is currently fixed.)\n\n");
            break;

        case CARD_INSURANCE_DISCOUNT:
            /* TODO: insurance is not implemented (Rules-LK 8 to 11). */
            printf("(Not applied - insurance is not implemented.)\n\n");
            break;

        case CARD_FOREIGN_FUNDING:
            /* TODO: "commercial property" is never defined in the spec. It
             * would need a tag on each square and an interpretation of which
             * Sri Lankan locations count as commercial. */
            printf("(Not applied - the spec does not define which properties"
                   " are commercial.)\n\n");
            break;

        case CARD_LABOUR_STRIKE:
            /* Construction suspended for 2 rounds - a gate on the build step
             * rather than a percentage, so it uses its own field. */
            g->construction_blocked_until = g->game_round + 2;
            printf("Construction suspended for 2 rounds.\n\n");
            break;

        default:
            break;
    }

}

/* Appendix A Labour Strike and Rule-LK 18 Fuel Crisis both suspend or slow
 * construction. Suspension is a gate on the build step, not a percentage, so
 * it is checked directly rather than through the ledger. */
bool construction_allowed(const GameStat *g){
    return g->game_round >= g->construction_blocked_until;
}

/* Rule-LK 18: every fifteen rounds one national economic event occurs,
 * affecting every player. Each is given a fifteen-round interval between events. 
 * Thus exactly one is active at a time. */
void trigger_economic_event(GameStat *g){
    static const char *names[] = {
        "Tourism Boom", "Fuel Crisis", "Heavy Monsoon", "Economic Recession",
        "Stock Market Boom", "Government Housing Programme",
        "Foreign Investment", "Political Unrest"
    };
    int pick = rand() % 8;

    printf("Economic Event\n\n%s\n\n", names[pick]);

    switch (pick){
        case 0:
            add_effect(g, "Tourism Boom", EFF_RENT, SCOPE_GLOBAL, 0,
                       100, g->game_round, 15);
            printf("Hotels receive double rent.\n\n");
            break;

        case 1:
            add_effect(g, "Fuel Crisis", EFF_RENT, SCOPE_SQUARE_TYPE,
                       RAILWAY, 100, g->game_round, 15);
            add_effect(g, "Fuel Crisis", EFF_BUILD_COST, SCOPE_GLOBAL, 0,
                       20, g->game_round, 15);
            printf("Railway rent doubles. Development costs increase 20%%.\n\n");
            break;

        case 2:
            /* TODO: "flood risk" needs the insurance and disaster systems,
             * and "coastal properties" is never defined by the spec. */
            printf("Flood risk increases.\n\n");
            printf("(Not applied - coastal properties are not defined.)\n\n");
            break;

        case 3:
            add_effect(g, "Economic Recession", EFF_PURCHASE_PRICE,
                       SCOPE_GLOBAL, 0, -15, g->game_round, 15);
            add_effect(g, "Economic Recession", EFF_RENT,
                       SCOPE_GLOBAL, 0, -10, g->game_round, 15);
            printf("Property values fall 15%%. Rent falls 10%%.\n\n");
            break;

        case 4:
            add_effect(g, "Stock Market Boom", EFF_PURCHASE_PRICE,
                       SCOPE_GLOBAL, 0, 10, g->game_round, 15);
            printf("Property values increase 10%%.\n\n");
            break;

        case 5:
            add_effect(g, "Government Housing Programme", EFF_BUILD_COST,
                       SCOPE_GLOBAL, 0, -25, g->game_round, 15);
            printf("House construction costs reduce 25%%.\n\n");
            break;

        case 6:
            /* TODO: "commercial properties" is never defined by the spec. */
            printf("(Not applied - commercial properties are not defined.)\n\n");
            break;

        case 7:
            add_effect(g, "Political Unrest", EFF_RENT, SCOPE_GLOBAL, 0,
                       -50, g->game_round, 15);
            printf("Hotel occupancy falls; hotel rent drops 50%%.\n\n");
            break;
    }
}

/* Rule-LK 24: every twenty rounds one government regulation is selected. */
void trigger_regulation(GameStat *g){
    static const char *names[] = {
        "Increase Property Tax", "Reduce Loan Interest", "Housing Subsidy",
        "Luxury Property Tax", "Railway Modernization",
        "Electricity Tariff Revision", "Insurance Regulation",
        "Anti-Speculation Act"
    };
    int pick = rand() % 8;

    printf("Government Regulation\n\n%s Introduced.\n\n", names[pick]);

    switch (pick){
        case 2:
            add_effect(g, "Housing Subsidy", EFF_BUILD_COST, SCOPE_GLOBAL, 0,
                       -30, g->game_round, 20);
            printf("Construction costs reduced by 30%%.\n\n");
            break;

        case 4:
            add_effect(g, "Railway Modernization", EFF_RENT,
                       SCOPE_SQUARE_TYPE, RAILWAY, 25, g->game_round, 20);
            printf("Railway rents increase 25%%.\n\n");
            break;

        case 5:
            add_effect(g, "Electricity Tariff Revision", EFF_RENT,
                       SCOPE_SQUARE_TYPE, UTILITY, 20, g->game_round, 20);
            printf("Utility rents increase 20%%.\n\n");
            break;

        /* TODO: the remaining regulations need systems that do not exist -
         * a mutable income tax (0), a variable Bank rate (1), a hotel
         * maintenance tax (3), insurance premiums (6), and a cap on
         * undeveloped holdings with forced development (7). */
        default:
            printf("(Not applied - requires a system that is not"
                   " implemented.)\n\n");
            break;
    }
}

/* Rules-LK 30 to 33: every ten rounds the property market is reviewed. One
 * group is selected for a Market Boom and another for a Market Decline, each
 * lasting ten rounds. Rule-LK 33 bars a group from reselection until thirty
 * rounds have elapsed. */
void review_property_market(GameStat *g){
    int boom = -1, decline = -1;

    for (int attempt = 0; attempt < 20 && boom < 0; attempt++){
        int c = rand() % 8;
        if (g->game_round - g->group_last_event[c] >= 30) boom = c;
    }
    for (int attempt = 0; attempt < 20 && decline < 0; attempt++){
        int c = rand() % 8;
        if (c != boom && g->game_round - g->group_last_event[c] >= 30) decline = c;
    }

    if (boom >= 0){
        g->group_last_event[boom] = g->game_round;
        add_effect(g, "Market Boom", EFF_PURCHASE_PRICE, SCOPE_GROUP, 
                boom, 15, g->game_round, 10);
        add_effect(g, "Market Boom", EFF_MORTGAGE_VALUE, SCOPE_GROUP, 
                boom, 15, g->game_round, 10);
        add_effect(g, "Market Boom", EFF_RENT,           SCOPE_GROUP, 
                boom, 25, g->game_round, 10);
        add_effect(g, "Market Boom", EFF_BUILD_COST,     SCOPE_GROUP, 
                boom, 10, g->game_round, 10);
        printf("Market Boom declared for property group %d.\n\n", boom + 1);
    }

    if (decline >= 0){
        g->group_last_event[decline] = g->game_round;
        add_effect(g, "Market Decline", EFF_PURCHASE_PRICE, SCOPE_GROUP, 
                decline, -15, g->game_round, 10);
        add_effect(g, "Market Decline", EFF_RENT,           SCOPE_GROUP, 
                decline, -20, g->game_round, 10);
        add_effect(g, "Market Decline", EFF_MORTGAGE_VALUE, SCOPE_GROUP, 
                decline, -10, g->game_round, 10);
        printf("Market Decline declared for property group %d.\n\n", decline + 1);
    }
}
