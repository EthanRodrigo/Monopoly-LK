#include <stdio.h>
#include <string.h>
#include "players.h"
#include "board.h"
#include "game.h"
#include "events.h"
#include "finance.h"

/* This function predicts the next roll's highest rent. But it only considers the base rent.
 * We can apply that logic but would change a lot of functions pointers creating a lot of 
 * unused variables and issues like circular dependecies. 
 * */
static int get_next_highest_rent(int curr_pos, const Square *s, const GameStat *g){
    int highest_rent = 0;

    for(int offset = 2; offset <= 12; offset++){
        int next_pos = resolve_out_of_bounds(curr_pos, offset);
        const Square *next = &s[next_pos];

        // should fail if no owner, cz bank is 0 in the enum
        if (next->purchasable && get_owner(next)){ 
            int rent = get_rent(g, next);
            highest_rent = highest_rent < rent ? rent : highest_rent;  
        }
    }
    return highest_rent;
}

/* @param p A pointer to the current player
 * @param board A pointer to the board to pass to the get_next_highest_rent function
 * */
int aggressive_buy(Player *p, Square *board, const GameStat *g){
    Square *s = &board[p->position];    // the current square the player in
    if (!s->purchasable || get_owner(s)) return BUY_INELIGIBLE;

    int price = get_purchase_price(g, s);
    // only save upto highest rent in the next roll
    int has_enough_cash = ((p->cash - price) > get_next_highest_rent(p->position, board, g));
    
    if (has_enough_cash){
        set_owner(s, p->owner_id);
        p->cash -= price;
        return BUY_BOUGHT;
    }
    return BUY_DECLINED;
}

int conservative_buy(Player *p, Square *board, const GameStat *g){
    Square *s = &board[p->position];    
    if (!s->purchasable || get_owner(s)) return BUY_INELIGIBLE;

    int price = get_purchase_price(g, s);
    int calculated_remaining = p->cash - price; 

    if (calculated_remaining >= (p->cash / 2)){
        set_owner(s, p->owner_id);
        p->cash -= price;
        return BUY_BOUGHT;
    }
    return BUY_DECLINED;
}

int risky_buy(Player *p, Square *board, const GameStat *g){
    Square *s = &board[p->position];    
    if (!s->purchasable || get_owner(s)) return BUY_INELIGIBLE;

    int price = get_purchase_price(g, s);
    if (p->cash >= price){
        set_owner(s, p->owner_id);
        p->cash -= price;
        return BUY_BOUGHT;
    }
    return BUY_DECLINED;
}

int opportunistic_buy(Player *p, Square *board, const GameStat *g){
    Square *s = &board[p->position];
    if (!s->purchasable || get_owner(s)) return BUY_INELIGIBLE;

    int price = get_purchase_price(g, s);
    if (p->cash < price) return BUY_DECLINED;

    // TODO: replace with real "projected appreciation vs construction cost"
    // once inflation / market boom-decline / regional development cards exist.
    // For now: treat rent-to-price ratio as a stand-in for "good return."
    int rent = get_rent(g, s);
    int good_return = (rent * 100 >= price * 8);

    if (good_return){
        set_owner(s, p->owner_id);
        p->cash -= price;
        return BUY_BOUGHT;
    }
    return BUY_DECLINED;
}

/* Each bidder raises by the minimum increment according to their behavior and withdraws once 
 * its own ceiling is passed. Returning 0 means withdraw, each player bids exactly 250 
 * than the current bid. */

// bids aggressively until the property reaches 120% of the markset value
int bid_aggressive(const Player *p, int current_bid, int market_value){
    int next = current_bid + BID_INCREMENT;
    int ceiling = market_value * 120 / 100;

    if (next > ceiling) return 0;
    if (next > p->cash) return 0;        /* Rule-LK 22: cannot bid beyond cash */
    return next;
}

// Rule 3.2 - "participates in auctions only when bidding below market value". 
int bid_conservative(const Player *p, int current_bid, int market_value){
    int next = current_bid + BID_INCREMENT;

    if (next >= market_value) return 0;  /* strictly below market value */
    if (next > p->cash) return 0;
    return next;
}

// Rule 3.3 - "bids until available cash is exhausted". 
int bid_risky(const Player *p, int current_bid, int market_value){
    (void)market_value;                  // market value is not needed here.
    int next = current_bid + BID_INCREMENT;

    if (next > p->cash) return 0;
    return next;
}

/* Rule 3.4 - "prefers discounted auction purchases rather than direct
 * purchases". No numeric threshold is given.
 * PLACEHOLDER: treat "discounted" as at most 75% of market value. Not derived
 * from the spec - be ready to justify at the viva.
 * TODO: revisit once events.c gives real market valuation. */
int bid_opportunistic(const Player *p, int current_bid, int market_value){
    int next = current_bid + BID_INCREMENT;
    int ceiling = market_value * 75 / 100;

    if (next > ceiling) return 0;
    if (next > p->cash) return 0;
    return next;
}

// TODO: Paused the build functions till the required extensions are added.
/* ---- Construction (Rule 3 step 6) --------------------------------------
 * Construction is portfolio-scoped, not tied to where the player landed:
 * Rule 8 requires only a monopoly, and Rule 3 lists construction as a step
 * separate from the landing action and purchase. Each function is called
 * once per colour group by the turn loop; groups the player has no monopoly
 * in are a no-op, since can_build_house checks has_monopoly internally.
 *
 * can_build_house and can_build_hotel enforce all of Rules 8, 9 and 10 -
 * ownership, monopoly, the four-house cap and even development - so the
 * strategy layer decides only WHETHER to build, never whether it is legal.
 */

/* Rule 3.1 - "constructs the maximum possible number of houses immediately
 * after obtaining a monopoly" and "converts houses into hotels as soon as
 * legally permitted". No cash cushion: the "sufficient funds for one future
 * rent" clause in 3.1 is scoped to purchasing, not construction. */
void aggressive_build(Player *p, Square *board, Group target_group, const GameStat *g){
    int built_something;

    /* Repeat full passes until a pass builds nothing. Even development (Rule
     * 9) allows only one house per property per pass - once a property takes
     * a house it sits above the group minimum until the others catch up later
     * in the same pass, and a forward scan cannot revisit it. */
    do {
        built_something = 0;

        for (int i = 1; i < BOARD_SIZE; i++){
            Square *s = &board[i];

            /* union guard - never read .data.property on a non-property */
            if (s->type != PROPERTY) continue;
            if (s->data.property.group != target_group) continue;

            int house_cost = (s->data.property.house_const_cost
                            * effect_pct(g, s, i, EFF_BUILD_COST, p->owner_id) + 50) / 100;
            int hotel_cost = (s->data.property.hotel_const_cost
                            * effect_pct(g, s, i, EFF_BUILD_COST, p->owner_id) + 50) / 100;

            if (p->cash >= house_cost && can_build_house(board, s, p->owner_id)){
                p->cash -= house_cost;
                s->data.property.no_of_houses++;
                built_something = 1;

                printf("%s constructed one house on %s.\n\n",
                       player_name(p->id), s->name);
                printf("Construction Cost : LKR %s.\n\n", lkr(house_cost));
                continue;
            }

            if (p->cash >= hotel_cost && can_build_hotel(board, s, p->owner_id)){
                p->cash -= hotel_cost;
                s->data.property.no_of_houses = 0;   /* Rule 10: hotel replaces houses */
                s->data.property.has_hotel = true;
                built_something = 1;

                printf("%s upgraded %s to a Hotel.\n\n",
                       player_name(p->id), s->name);
                printf("Construction Cost : LKR %s.\n\n", lkr(hotel_cost));
            }
        }
    } while (built_something);
}

/* Rule 3.2 - the spec states no construction pace for this player.
 * ASSUMPTION: the stated purchase rule, "purchases properties only if at
 * least 50% of current cash remains after purchase", is applied to
 * construction too, as the only cash guidance given for a player that
 * "maintains the largest emergency cash reserve". Self-limiting - each build
 * shrinks cash, so the test tightens on its own.
 *
 * Rule 3.2 also states "never develops hotels until all outstanding loans
 * have been settled". Now that loans exist this is a real condition. */
void conservative_build(Player *p, Square *board, Group target_group, const GameStat *g){
    bool allow_hotels = (p->loan_amount == 0);

    for (int i = 1; i < BOARD_SIZE; i++){
        Square *s = &board[i];

        if (s->type != PROPERTY) continue;
        if (s->data.property.group != target_group) continue;

        int house_cost = (s->data.property.house_const_cost
                            * effect_pct(g, s, i, EFF_BUILD_COST, p->owner_id) + 50) / 100;
        int hotel_cost = (s->data.property.hotel_const_cost
                            * effect_pct(g, s, i, EFF_BUILD_COST, p->owner_id) + 50) / 100;

        if ((p->cash - house_cost) >= (p->cash / 2) &&
            can_build_house(board, s, p->owner_id)){
            p->cash -= house_cost;
            s->data.property.no_of_houses++;

            printf("%s constructed one house on %s.\n\n",
                   player_name(p->id), s->name);
            printf("Construction Cost : LKR %s.\n\n", lkr(house_cost));
            return;   /* one building per turn */
        }

        if (allow_hotels &&
            (p->cash - hotel_cost) >= (p->cash / 2) &&
            can_build_hotel(board, s, p->owner_id)){
            p->cash -= hotel_cost;
            s->data.property.no_of_houses = 0;
            s->data.property.has_hotel = true;

            printf("%s upgraded %s to a Hotel.\n\n",
                   player_name(p->id), s->name);
            printf("Construction Cost : LKR %s.\n\n", lkr(hotel_cost));
            return;
        }
    }
}

/* Rule 3.3 - "constructs hotels as early as possible", with no cash reserve
 * held. Under the traditional rules this is identical to the Aggressive
 * Investor: both develop to the legal maximum with no cushion. The two
 * strategies diverge in loan usage ("always borrows the maximum permitted"
 * versus borrowing only for projected rental income) and in selling assets
 * to finance development - not in construction itself. */
void risky_build(Player *p, Square *board, Group target_group, const GameStat *g){
    aggressive_build(p, board, target_group, g);
}

/* Rule 3.4 - the spec states no construction pace, only that this player
 * "delays construction during inflation" and "accelerates construction during
 * Government Housing Subsidy periods". Both depend on events.c. With no
 * inflation and no subsidy active neither modifier fires, so baseline
 * affordability applies - the condition is vacuously satisfied, not skipped.
 * TODO: once events.c exists, the pace modifiers belong here as a gate on
 * the whole function, not as a per-building cost test. */
void opportunistic_build(Player *p, Square *board, Group target_group, const GameStat *g){
    int built_something;

    do {
        built_something = 0;

        for (int i = 1; i < BOARD_SIZE; i++){
            Square *s = &board[i];

            if (s->type != PROPERTY) continue;
            if (s->data.property.group != target_group) continue;

            int house_cost = (s->data.property.house_const_cost
                            * effect_pct(g, s, i, EFF_BUILD_COST, p->owner_id) + 50) / 100;
            int hotel_cost = (s->data.property.hotel_const_cost
                            * effect_pct(g, s, i, EFF_BUILD_COST, p->owner_id) + 50) / 100;

            if (p->cash >= house_cost && can_build_house(board, s, p->owner_id)){
                p->cash -= house_cost;
                s->data.property.no_of_houses++;
                built_something = 1;

                printf("%s constructed one house on %s.\n\n",
                       player_name(p->id), s->name);
                printf("Construction Cost : LKR %s.\n\n", lkr(house_cost));
                continue;
            }

            if (p->cash >= hotel_cost && can_build_hotel(board, s, p->owner_id)){
                p->cash -= hotel_cost;
                s->data.property.no_of_houses = 0;
                s->data.property.has_hotel = true;
                built_something = 1;

                printf("%s upgraded %s to a Hotel.\n\n",
                       player_name(p->id), s->name);
                printf("Construction Cost : LKR %s.\n\n", lkr(hotel_cost));
            }
        }
    } while (built_something);
}

/* Cost of taking every property in a group from its current state to full
 * hotels. Rule 9 requires even development, so each property must reach four
 * houses before Rule 10 permits the hotel that replaces them - both costs
 * are therefore part of the total. */
static int cost_to_develop(const Square *board, Group grp, const GameStat *g){
    int total = 0;

    for (int i = 0; i < BOARD_SIZE; i++){
        const Square *s = &board[i];

        if (s->type != PROPERTY) continue;
        if (s->data.property.group != grp) continue;
        if (s->data.property.has_hotel) continue;   /* already complete */

        int houses_needed = 4 - s->data.property.no_of_houses;
        total += houses_needed * s->data.property.house_const_cost;
        total += s->data.property.hotel_const_cost;
    }
    return total;
}

/* Largest rent anyone could charge this player right now. Utilities are
 * priced at a roll of 12, the worst case, since a cautious player plans for
 * the largest liability rather than the average one. */
static int highest_rent_on_board(const Player *p, const Square *board, const GameStat *g){
    int highest = 0;

    for (int i = 0; i < BOARD_SIZE; i++){
        const Square *s = &board[i];
        Owner owner = get_owner(s);

        if (owner == OG_BANK || owner == p->owner_id) continue;
        if (is_mortgaged(s)) continue;

        int rent = 0;
        switch (s->type){
            case PROPERTY: rent = property_rent(g, s);                 break;
            case RAILWAY:  rent = railway_rent(g, board, owner);        break;
            case UTILITY:  rent = utility_rent(g, board, owner, 12);    break;
            default: continue;
        }
        if (rent > highest) highest = rent;
    }
    return highest;
}

LoanDecision loan_aggressive(const Player *p, const Square *board, int max_loan, const GameStat *g){
    LoanDecision d = { LOAN_DO_NOTHING, 0 };

    if (p->loan_amount > 0){
        if (p->cash > p->loan_amount * 2) d.action = LOAN_REPAY_FULL;
        return d;
    }

    if (max_loan <= 0) return d;

    /* additional funds increase projected rental income when the
     * player holds a monopoly that is not yet fully developed, since only a
     * monopoly permits construction (Rule 8). The amount requested is what full
     * development of that group would cost. */
    for (int grp = BROWN; grp <= DARK_BLUE; grp++){
        if (!has_monopoly(p->owner_id, board, (Group)grp)) continue;

        int needed = cost_to_develop(board, (Group)grp, g);
        if (needed <= 0) continue;              // already all hotels 
        if (p->cash >= needed) continue;        // can already afford it 

        d.action = LOAN_OBTAIN;
        d.amount = needed - p->cash;            // borrow only the shortfall 
        return d;
    }
    return d;
}

/* Rule 3.2 - "avoids obtaining loans unless bankruptcy is imminent" and
 * "repays loans immediately whenever visiting the Bank if sufficient funds exist". */
LoanDecision loan_conservative(const Player *p, const Square *board, int max_loan, const GameStat *g){
    LoanDecision d = { LOAN_DO_NOTHING, 0 };

    if (p->loan_amount > 0){
        if (p->cash >= p->loan_amount){
            d.action = LOAN_REPAY_FULL;
        } else {
            d.action = LOAN_REPAY_PART;
            d.amount = p->cash;      // pay down as much as cash allows 
        }
        return d;
    }

    if (max_loan <= 0) return d;

    // bankruptcy is imminent when cash cannot cover the largest liability player could face
    int rent_risk = highest_rent_on_board(p, board, g);
    int tax_risk  = projected_income_tax(board, p->cash);
    int liability = rent_risk > tax_risk ? rent_risk : tax_risk;

    if (p->cash < liability){   
        d.action = LOAN_OBTAIN;
        d.amount = liability - p->cash;
    }
    return d;
}

/* Rule 3.3 - "always borrows the maximum loan permitted" and "frequently
 * refinances loans to increase available capital". */
LoanDecision loan_risky(const Player *p, const Square *board, int max_loan, const GameStat *g){
    LoanDecision d = { LOAN_DO_NOTHING, 0 };
    (void)board;

    if (p->loan_amount > 0){
        if (max_loan > p->loan_amount){
            d.action = LOAN_INCREASE;            /* refinance for more capital */
            d.amount = max_loan - p->loan_amount;
        } else {
            d.action = LOAN_EXTEND;
        }
        return d;
    }

    if (max_loan > 0){
        d.action = LOAN_OBTAIN;
        d.amount = max_loan;
    }
    return d;
}

/* Rule 3.4 - "obtains loans only when projected return exceeds borrowing
 * cost".
 * PLACEHOLDER: projected return depends on the market system (inflation,
 * booms, regional development) that events.c would provide, so it cannot be
 * computed. Borrowing cost is known - the loan's frozen interest rate per
 * round. As a stand-in the player borrows only when it holds an undeveloped
 * monopoly, the one case where borrowed cash has a clear productive use, and
 * requests only the development shortfall.
 * TODO: replace with a real projected-return comparison once events.c and
 * market valuation exist. */
LoanDecision loan_opportunistic(const Player *p, const Square *board, int max_loan, const GameStat *g){
    LoanDecision d = { LOAN_DO_NOTHING, 0 };

    if (p->loan_amount > 0){
        if (p->cash >= p->loan_amount) d.action = LOAN_REPAY_FULL;
        return d;
    }

    if (max_loan <= 0) return d;

    for (int grp = BROWN; grp <= DARK_BLUE; grp++){
        if (!has_monopoly(p->owner_id, board, (Group)grp)) continue;

        int needed = cost_to_develop(board, (Group)grp, g);
        if (needed <= 0) continue;
        if (p->cash >= needed) continue;

        d.action = LOAN_OBTAIN;
        d.amount = needed - p->cash;
        return d;
    }
    return d;
}

/* Rule 3.1 - no renovation rule is stated for this player.
 * Thus implemented as it renovates at the first sign of decay since he focuses on maximizing
 * rental income. */
bool renovate_aggressive(const Player *p, const Square *s){
    (void)p;
    return s->data.property.depreciation > 0
        || s->data.property.structural_damage;
}

// Rule 3.2 - "renovates depreciated properties immediately once depreciation exceeds 10%"
bool renovate_conservative(const Player *p, const Square *s){
    (void)p;
    return s->data.property.depreciation > 10
        || s->data.property.structural_damage;
}

// Rule 3.3 - "ignores property depreciation until repair becomes unavoidable"
bool renovate_risky(const Player *p, const Square *s){
    (void)p;
    return s->data.property.depreciation >= MAX_DEPRECIATION
        || s->data.property.structural_damage;
}

/* Rule 3.4 - "renovates properties once depreciation exceeds 15%". Stated
 * verbatim. */
bool renovate_opportunistic(const Player *p, const Square *s){
    (void)p;
    return s->data.property.depreciation > 15
        || s->data.property.structural_damage;
}

void initialize_players(Player* players){
	Player temp_players[NO_OF_PLAYERS] = {
		[0] = {
			.id = AGGRESSIVE_INVESTOR,
            .owner_id = PLAYER_1,
			.cash = 30000,
            .net_worth = 30000,
            .position = START_SQUARE,
            .player_rounds = 0,
            .in_jail = false,
            .jail_turns = 0,
            .loan_amount = 0,
            .loan_round = 0,
            .loan_duration = 0,
            .bankrupt = false,
            .buy_property = aggressive_buy,
            .build_property = aggressive_build,
            .bid = bid_aggressive,
            .loan_action = loan_aggressive,   
            .should_renovate = renovate_aggressive,
	},
		
		[1] = {
			.id = CONSERVATIVE_BANKER,
            .owner_id = PLAYER_2,
			.cash = 30000,
            .net_worth = 30000,
            .position = START_SQUARE,
            .player_rounds = 0,
            .in_jail = false,
            .jail_turns = 0,
            .loan_amount = 0,
            .loan_round = 0,
            .loan_duration = 0,
            .bankrupt = false,
            .buy_property = conservative_buy,
            .build_property = conservative_build,
            .bid = bid_conservative,
            .loan_action = loan_conservative,   
            .should_renovate = renovate_conservative,
		},
		
		[2] = {
			.id = RISK_TAKER,
            .owner_id = PLAYER_3,
			.cash = 30000,
            .net_worth = 30000,
            .position = START_SQUARE,
            .player_rounds = 0,
            .in_jail = false,
            .jail_turns = 0,
            .loan_amount = 0,
            .loan_round = 0,
            .loan_duration = 0,
            .bankrupt = false,
            .buy_property = risky_buy,
            .build_property = risky_build,
            .bid = bid_risky,
            .loan_action = loan_risky,   
            .should_renovate = renovate_risky,
		},

		[3] = {
			.id = OPPORTUNISTIC_TRADER,
            .owner_id = PLAYER_4,
			.cash = 30000,
            .net_worth = 30000,
            .position = START_SQUARE,
            .player_rounds = 0,
            .in_jail = false,
            .jail_turns = 0,
            .loan_amount = 0,
            .loan_round = 0,
            .loan_duration = 0,
            .bankrupt = false,
            .buy_property = opportunistic_buy,
            .build_property = opportunistic_build,
            .bid = bid_opportunistic,
            .loan_action = loan_opportunistic,   
            .should_renovate = renovate_opportunistic,
		},
	};
    
    memcpy(players, temp_players, sizeof(Player) * NO_OF_PLAYERS);
} 

bool has_monopoly(Owner owner_id, const Square *board, Group target_group) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        const Square *s = &board[i];
        if (s->type != PROPERTY) continue;
        if (s->data.property.group != target_group) continue;

        if (s->data.property.owner != owner_id) {
            return false;   // found one property in this group not owned by p
        }
    }
    return true;   
}

/* Owner runs OG_BANK, PLAYER_1..PLAYER_4 while players[] is indexed 0..3, 
 * so the mapping is off by one. */ 
Player *find_player(Player *players, Owner id){
    if (id == OG_BANK) return NULL;

    int idx = (int)id - 1;
    if (idx < 0 || idx >= NO_OF_PLAYERS) return NULL;

    return &players[idx];
}

/* Display names exactly as Section 5 prints them. */
const char *player_name(PlayerType id){
    switch (id){
        case AGGRESSIVE_INVESTOR:   return "Aggressive Investor";
        case CONSERVATIVE_BANKER:   return "Conservative Banker";
        case RISK_TAKER:            return "Risk Taker";
        case OPPORTUNISTIC_TRADER:  return "Opportunistic Trader";
        default:                    return "Unknown Player";
    }
}

/* Thousands-separated amount, e.g. 30000 -> "30,000".
 * A ring of buffers lets several calls appear in a single printf. */
const char *lkr(int amount){
    #define LKR_BUFFERS 6
    static char buf[LKR_BUFFERS][32];
    static int  next = 0;

    char *out = buf[next];
    next = (next + 1) % LKR_BUFFERS;

    char digits[32];
    int negative = amount < 0;
    long value = negative ? -(long)amount : (long)amount;

    int n = snprintf(digits, sizeof(digits), "%ld", value);

    int o = 0;
    if (negative) out[o++] = '-';

    for (int i = 0; i < n; i++){
        if (i > 0 && (n - i) % 3 == 0) out[o++] = ',';
        out[o++] = digits[i];
    }
    out[o] = '\0';

    return out;
}

/* Number of purchasable squares of every kind owned by this player. */
int count_properties(const Player *p, const Square *board){
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        if (!board[i].purchasable) continue;
        if (get_owner(&board[i]) == p->owner_id) count++;
    }
    return count;
}

int count_hotels(const Player *p, const Square *board){
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type != PROPERTY) continue;
        if (board[i].data.property.owner != p->owner_id) continue;
        if (board[i].data.property.has_hotel) count++;
    }
    return count;
}

// Purchase value of every square the player owns - properties, railways and utilities.
int total_property_value(const Player *p, const Square *board, const GameStat *g){
    int total = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        if (!board[i].purchasable) continue;
        if (get_owner(&board[i]) != p->owner_id) continue;
        total += get_purchase_price(g, &board[i]);
    }
    return total;
}

// A developed property contributes either houses or a hotel, never both. 
static int total_building_value(const Player *p, const Square *board, const GameStat *g){
    int total = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type != PROPERTY) continue;

        const Property *prop = &board[i].data.property;
        if (prop->owner != p->owner_id) continue;

        if (prop->has_hotel) total += prop->hotel_const_cost;
        else                 total += prop->no_of_houses * prop->house_const_cost;
    }
    return total;
}

int calculate_net_worth(const Player *p, const Square *board, const GameStat *g){
    int worth = p->cash;

    worth += total_property_value(p, board, g);   // property + railway + utility 
    worth += total_building_value(p, board, g);
    worth -= p->loan_amount;

    /* TODO: + Insurance Claims Receivable   (needs finance.c)
     * TODO: - Accrued Interest              (needs finance.c)
     * TODO: - Taxes Due                     (needs debt recovery) */

    return worth;
}
