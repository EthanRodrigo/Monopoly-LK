#include <stdint.h>
#include "board.h"
#include "players.h"

static int is_active(uint8_t bitmap, int idx){
    return (bitmap >> idx) & 1;
}

static void withdraw(uint8_t *bitmap, int idx){
    *bitmap &= ~(1 << idx);
}

static int count_active(uint8_t bitmap){
    int n = 0;
    for (int i = 0; i < NO_OF_PLAYERS; i++) n += is_active(bitmap, i);
    return n;
}

/* The auction starts and choose which players will be participating according to their
 * behaviors.
 * @param players The array of all players
 * @param board The board itself
 * @param square_index The index of the square being auctioned
 * @return  an integer specifying the player who won the auction
 * */
int start_auction(Player *players, Square *board, int square_index){
    Square *s = &board[square_index];
    if (!s->purchasable || get_owner(s)) return -1;

    int market_value = get_purchase_price(s);
    int current_bid  = market_value / 2;          // Rule-LK 19 
    int winner       = -1;

    // bitmap to mark the active players
    uint8_t active = (1 << NO_OF_PLAYERS) -1;  // Rule-LK 6 

    // bidding rounds
    while (count_active(active) > 1 || (count_active(active) == 1 && winner < 0)){
        int bids_this_round = 0;

        // players bidding
        for (int i = 0; i < NO_OF_PLAYERS; i++){
            if (!is_active(active, i)) continue;
            if (count_active(active) == 1 && winner == i) break;  // uncontested 

            Player *p = &players[i];
            int bid = p->bid(p, current_bid, market_value);

            if (bid <= current_bid){          
                withdraw(&active, i);
                continue;
            }

            current_bid = bid;
            winner = i; 
            bids_this_round++;
        }

        if (bids_this_round == 0) break;      // nobody raised 
    }

    if (winner >= 0){
        set_owner(s, players[winner].owner_id);
        players[winner].cash -= current_bid;
    }
    // else: Rule-LK 23, ownership remains with the Bank */
    
    return winner;
}

/* Calculaltes income tax based on the current holding cash value. The rates are same as 
 * Sri Lankan tax rates. But the thresholds starts from 15k and each 10k will get the 
 * rates, as that is the practical implementation in the game. 
 * @param cash Amount of money player currently hold
 * @return The amount of tax the player should be paying
 * */
static int calculate_income_tax(const Tax *t, int cash){
    if (cash <= t->free_allowance) return 0;

    int taxable = cash - t->free_allowance;
    int tax = 0;

    for (int band = 0; band < TAX_BAND_COUNT && taxable > 0; band++){
        int in_band;    // the taxable margin

        if (band == TAX_BAND_COUNT - 1){
            in_band = taxable;              // top rate: everything remaining 
        } else {
            in_band = taxable < t->band_width ? taxable : t->band_width;
        }

        tax += in_band * t->rates[band] / 100;
        taxable -= in_band;
    }

    return tax;
}

int pay_income_tax(Player *p, const Tax *t){
    int tax = calculate_income_tax(t, p->cash);
    p->cash -= tax;
    return tax;
}
