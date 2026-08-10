#include <string.h>
#include "players.h"
#include "board.h"

int get_next_highest_rent(int curr_pos, const Square *s){
    int highest_rent = 0;

    for(int offset = 2; offset <= 12; offset++){
        int next_pos = resolve_out_of_bounds(curr_pos, offset);
        const Square *next = &s[next_pos];

        // should fail if no owner, cz bank is 0 in the enum
        if (next->purchasable && get_owner(next)){ 
            int rent = get_rent(next);
            highest_rent = highest_rent < rent ? rent : highest_rent;  
        }
    }
    return highest_rent;
}

/* @param p A pointer to the current player
 * @param board A pointer to the board to pass to the get_next_highest_rent function
 * */
void aggressive_buy(Player *p, Square *board){
    Square *s = &board[p->position];    // the current square the player in
    if (!s->purchasable || get_owner(s)) return;

    int price = get_purchase_price(s);
    // only save upto highest rent in the next roll
    int has_enough_cash = (p->cash - price) > 
        get_next_highest_rent(p->position, board) ? 1 : 0;
    
    if (has_enough_cash){
        set_owner(s, p->owner_id);
        p->cash -= price;
    }
}

void conservative_buy(Player *p, Square *board){
    Square *s = &board[p->position];    
    if (!s->purchasable || get_owner(s)) return;

    int price = get_purchase_price(s);
    int calculated_remaining = p->cash - price; 

    if (calculated_remaining >= (p->cash / 2)){
        set_owner(s, p->owner_id);
        p->cash -= price;
    }
}

void risky_buy(Player *p, Square *board){
    Square *s = &board[p->position];    
    if (!s->purchasable || get_owner(s)) return;

    int price = get_purchase_price(s);
    if (p->cash >= price){
        set_owner(s, p->owner_id);
        p->cash -= price;
    }
}

void opportunistic_buy(Player *p, Square *board){
    Square *s = &board[p->position];
    if (!s->purchasable || get_owner(s)) return;

    int price = get_purchase_price(s);
    if (p->cash < price) return;

    // TODO: replace with real "projected appreciation vs construction cost"
    // once inflation / market boom-decline / regional development cards exist.
    // For now: treat rent-to-price ratio as a stand-in for "good return."
    int rent = get_rent(s);
    int good_return = (rent * 100 >= price * 8);

    if (good_return){
        set_owner(s, p->owner_id);
        p->cash -= price;
    }
}

// TODO: Paused the build functions till the required extensions are added.
void aggressive_build(Player *p, Square *board, Group target_group){
    int built_something;

    // Repeat full passes until a pass builds nothing. Even development means
    // one pass can only add one house per property, so reaching 4 houses
    // (and then hotels) needs several passes.

    // This loop keep running till the cash runs out.
    do {
        built_something = 0;

        // loop through the board to find other properties in the group
        for (int i = 1; i < BOARD_SIZE; i++){
            Square *s = &board[i];

            // skips other types and groups
            if (s->type != PROPERTY) continue;
            if (s->data.property.group != target_group) continue;

            int house_cost = s->data.property.house_const_cost;
            int hotel_cost = s->data.property.hotel_const_cost;

            if (p->cash >= house_cost && can_build_house(board, s, p->owner_id)){
                p->cash -= house_cost;
                s->data.property.no_of_houses++;
                built_something = 1;
                continue;
            }

            if (p->cash >= hotel_cost && can_build_hotel(board, s, p->owner_id)){
                p->cash -= hotel_cost;
                s->data.property.no_of_houses = 0;   // Rule 10: hotel replaces houses
                s->data.property.has_hotel = true;
                built_something = 1;
            }
        }
    } while (built_something);
}

void conservative_build(Player *p, Square *board, Group target_group){
    Square* s = &board[p->position];

    int house_cost = s->data.property.house_const_cost;
    int next_rent = get_next_highest_rent(p->position, board);
    int hotel_cost = s->data.property.hotel_const_cost;

    // just build one house if 50% of the money remains after the build
    if((p->cash - house_cost) >= (p->cash / 2) && can_build_house(board, s, p->owner_id)){
        p->cash -= house_cost;
        s->data.property.no_of_houses++;
    }
        // TODO: Hotel building needs more data added.
/*        if((p->cash - hotel_cost) < next_rent) && can_build_hotel(board, s, p->owner_id){
            p->cash - hotel_cost;
            s->data.property.has_hotel = true;
        }
*/
}

void risky_build(Player *p, Square *board, Group target_group){
    // They are the same. Build Hotels asap.
    aggressive_build(p, board, target_group);
}

void opportunistic_build(Player *p, Square *board, Group target_group){
    // TODO: Have to wait till the events and all
}

void initialize_players(Player* players){
	Player temp_players[NO_OF_PLAYERS] = {
		[0] = {
			.id = AGGRESSIVE_INVESTOR,
            .owner_id = PLAYER_1,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = aggressive_buy,
            .build_property = aggressive_build
		},
		
		[1] = {
			.id = CONSERVATIVE_BANKER,
            .owner_id = PLAYER_2,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = conservative_buy,
            .build_property = conservative_build
		},
		
		[2] = {
			.id = RISK_TAKER,
            .owner_id = PLAYER_3,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = risky_buy,
            .build_property = risky_build
		},

		[3] = {
			.id = OPPORTUNISTIC_TRADER,
            .owner_id = PLAYER_4,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = opportunistic_buy,
            .build_property = opportunistic_build
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
