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

void initialize_players(Player* players){
	Player temp_players[NO_OF_PLAYERS] = {
		[0] = {
			.id = AGGRESSIVE_INVESTOR,
            .owner_id = PLAYER_1,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = aggressive_buy
		},
		
		[1] = {
			.id = CONSERVATIVE_BANKER,
            .owner_id = PLAYER_2,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = conservative_buy
		},
		
		[2] = {
			.id = RISK_TAKER,
            .owner_id = PLAYER_3,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = risky_buy
		},

		[3] = {
			.id = OPPORTUNISTIC_TRADER,
            .owner_id = PLAYER_4,
			.cash = 30000,
            .net_worth = 30000,
            .position = START,
            .player_rounds = 0,
            .buy_property = opportunistic_buy
		},
	};
    
    memcpy(players, temp_players, sizeof(Player) * NO_OF_PLAYERS);
} 
