#include <stdint.h>
#include <stdio.h>
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

    printf("\nAuction Started.\n\n");
    printf("Property :\n%s\n\n", s->name);
    printf("Opening Bid :\nLKR %s.\n\n", lkr(current_bid));

    // bitmap to mark the active players
    uint8_t active = 0;
    for (int i = 0; i < NO_OF_PLAYERS; i++){
        if (!players[i].bankrupt) active |= (1 << i);
    }

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
                printf("%s withdraws.\n\n", player_name(p->id));
                withdraw(&active, i);
                continue;
            }

            current_bid = bid;
            winner = i;
            bids_this_round++;
            printf("%s bids LKR %s.\n\n", player_name(p->id), lkr(current_bid));
        }

        if (bids_this_round == 0) break;      // nobody raised
    }

    if (winner >= 0){
        set_owner(s, players[winner].owner_id);
        players[winner].cash -= current_bid;
        printf("%s wins the auction.\n\n", player_name(players[winner].id));
    } else {
        // Rule-LK 23, ownership remains with the Bank
        printf("No player bids. Ownership remains with the Bank.\n\n");
    }

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

int pay_income_tax(Player *p, Square *board, const Tax *t){
    int tax = calculate_income_tax(t, p->cash);
    return mortgage_and_pay(p, board, tax);
}

static int mortgage_square(Player *p, Square *s){
    int value = get_mortgage_value(s);

    if(is_developed(s)){
        demolish_buildings(s);
        printf("Buildings on %s were demolised.\n", s->name);
    }

    set_mortgaged(s, true);
    p->cash += value;

    printf("%s mortgaged %s.\n", player_name(p->id), s->name);
    printf("Received : LKR %s.\n\n", lkr(value));

    return value;
}

/* Lift a mortgage, charging 110% of the mortgage value. */
int unmortgage_square(Player *p, Square *s){
    if (!is_mortgaged(s)) return 0;

    int cost = get_mortgage_value(s) * UNMORTGAGE_PERCENT / 100;
    if (p->cash < cost) return 0;

    set_mortgaged(s, false);
    p->cash -= cost;

    printf("%s lifted the mortgage on %s.\n", player_name(p->id), s->name);
    printf("Cost : LKR %s.\n\n", lkr(cost));

    return cost;
}

/* Raise cash by mortgaging owned squares until `needed` is covered.
 * Cheapest mortgage value first, so the player gives up as little borrowing
 * capacity as possible per transaction.
 * @return true if the player can now cover `needed`.
 */
bool raise_cash(Player *p, Square *board, int needed){
    while (p->cash < needed){
        int best = -1;
        int best_value = 0;

        for (int i = 0; i < BOARD_SIZE; i++){
            Square *s = &board[i];

            if (!s->purchasable) continue;
            if (get_owner(s) != p->owner_id) continue;
            if (is_mortgaged(s)) continue;

            int value = get_mortgage_value(s);
            if (value <= 0) continue;

            if (best < 0 || value < best_value){
                best = i;
                best_value = value;
            }
        }

        if (best < 0) return false;   /* nothing left to mortgage */

        mortgage_square(p, &board[best]);
    }
    return true;
}

/* Make a payment, mortgaging property first if cash alone will not cover it.
 * Cash never goes below zero: a player who cannot meet the full amount pays
 * everything they have and is declared bankrupt under Rule 14, since
 * liabilities then exceed available assets.
 *
 * @return the amount ACTUALLY paid. Callers transferring money to another
 *         player must credit this, not the amount requested - crediting the
 *         full amount would create money that the payer never had.
 */
int mortgage_and_pay(Player *p, Square *board, int amount){
    if (amount <= 0) return 0;

    if (p->cash < amount){
        raise_cash(p, board, amount);
    }

    if (p->cash >= amount){
        p->cash -= amount;
        return amount;
    }

    /* Rule 14: everything the player has is paid over, and what remains
     * unpaid means liabilities exceed available assets. */
    int paid = p->cash;
    p->cash = 0;

    declare_bankrupt(p);

    return paid;
}

/* ---- Loans -------------------------------------------------------------
 * Rule-LK 1: eligible collateral is properties, railway stations and utility
 * companies. Buildings are explicitly excluded.
 * Rule-LK 2: the maximum loan is 75% of the total mortgage value of all
 * eligible collateral.
 *
 * INTERPRETATION: already-mortgaged squares are excluded from collateral. A
 * square whose mortgage value has already been drawn as cash cannot back a
 * second advance of the same value, and Rule-LK 3 forbids additionally
 * mortgaging loan locked property.
 */
int max_loan_amount(const Player *p, const Square *board){
    int total = 0;

    for (int i = 0; i < BOARD_SIZE; i++){
        const Square *s = &board[i];

        if (!s->purchasable) continue;
        if (get_owner(s) != p->owner_id) continue;
        if (is_mortgaged(s)) continue;

        total += get_mortgage_value(s);
    }

    return total * MAX_LOAN_PERCENT / 100;
}

/* Pledge every eligible square as collateral. Rule-LK 3: pledged squares
 * become Loan Locked - they keep earning rent and may still be developed,
 * but cannot be sold, traded, auctioned or further mortgaged. */
static void lock_collateral(Player *p, Square *board){
    printf("Collateral :\n");

    for (int i = 0; i < BOARD_SIZE; i++){
        Square *s = &board[i];

        if (!s->purchasable) continue;
        if (get_owner(s) != p->owner_id) continue;
        if (is_mortgaged(s)) continue;

        set_loan_locked(s, true);
        printf("%s\n", s->name);
    }
    printf("\n");
}

static void release_collateral(Player *p, Square *board){
    for (int i = 0; i < BOARD_SIZE; i++){
        Square *s = &board[i];
        if (get_owner(s) == p->owner_id) set_loan_locked(s, false);
    }
}

static void obtain_loan(Player *p, Square *board, int amount, int game_round){
    if (amount <= 0) return;

    p->loan_amount   = amount;
    p->loan_round    = game_round;
    p->loan_duration = LOAN_DURATION;
    p->cash += amount;              /* Rule-LK 3: credited immediately */

    printf("%s obtained a secured loan.\n\n", player_name(p->id));
    printf("Loan Amount : LKR %s.\n\n", lkr(amount));
    lock_collateral(p, board);
    printf("Interest Rate : %d%%\n", LOAN_INTEREST_PERCENT);
    printf("Duration : %d Rounds\n\n", LOAN_DURATION);
}

static void repay_loan(Player *p, Square *board, int amount){
    if (p->loan_amount <= 0 || amount <= 0) return;
    if (amount > p->loan_amount) amount = p->loan_amount;
    if (amount > p->cash)        amount = p->cash;
    if (amount <= 0) return;

    p->cash        -= amount;
    p->loan_amount -= amount;

    printf("%s repaid LKR %s.\n\n", player_name(p->id), lkr(amount));

    if (p->loan_amount == 0){
        release_collateral(p, board);
        printf("Outstanding Balance :\nLKR 0.\n\n");
    } else {
        printf("Outstanding Balance :\nLKR %s.\n\n", lkr(p->loan_amount));
    }
}

/* Rule-LK 6: failure to repay within the loan duration causes default. All
 * pledged assets transfer to the Bank, buildings are demolished, insurance is
 * cancelled, the outstanding debt is cleared, and the player continues using
 * whatever assets remain. */
static void default_loan(Player *p, Square *board){
    printf("%s has defaulted.\n\n", player_name(p->id));

    for (int i = 0; i < BOARD_SIZE; i++){
        Square *s = &board[i];

        if (!is_loan_locked(s)) continue;
        if (get_owner(s) != p->owner_id) continue;

        demolish_buildings(s);
        set_mortgaged(s, false);
        set_loan_locked(s, false);
        set_owner(s, OG_BANK);
        /* TODO: Rule-LK 6 also cancels insurance on foreclosed squares.
         * Insurance is not implemented yet. */
    }

    p->loan_amount   = 0;
    p->loan_duration = 0;

    printf("Collateral has been foreclosed.\n\n");
    printf("Outstanding debt cleared.\n\n");

    /* TODO: Rule-LK 7 - if the player has no remaining assets at all they
     * are declared bankrupt. Needs Rule 14 bankruptcy. */
}

/* Rule-LK 4: interest compounds at the end of every complete round -
 * Current Loan x Current Interest Rate - and the accrued interest becomes
 * part of the outstanding loan. */
void accrue_loan_interest(Player *p){
    if (p->loan_amount <= 0) return;
    p->loan_amount += p->loan_amount * LOAN_INTEREST_PERCENT / 100;
}

/* Rule-LK 4: maturity is measured in complete rounds, so this is checked at
 * the same point as interest accrual. */
void check_loan_default(Player *p, Square *board, int game_round){
    if (p->loan_amount <= 0) return;
    if (game_round - p->loan_round < p->loan_duration) return;
    default_loan(p, board);
}

int projected_income_tax(const Square *board, int cash){
    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type == TAX){
            return calculate_income_tax(&board[i].data.tax, cash);
        }
    }
    return 0;
}

void resolve_bank_visit(Player *p, Square *board, int game_round){
    int max_loan = max_loan_amount(p, board);
    LoanDecision d = p->loan_action(p, board, max_loan);

    /* Rule-LK 2: never advance more than the collateral supports. */
    if (d.amount > max_loan) d.amount = max_loan;
    if (d.amount < 0)        d.amount = 0;

    switch (d.action){
        case LOAN_OBTAIN:
            obtain_loan(p, board, d.amount, game_round);
            break;

        case LOAN_REPAY_FULL:
            repay_loan(p, board, p->loan_amount);
            break;

        case LOAN_REPAY_PART:
            repay_loan(p, board, d.amount);
            break;

        case LOAN_EXTEND:
            p->loan_duration += LOAN_DURATION;
            printf("%s extended the loan period by %d rounds.\n\n",
                   player_name(p->id), LOAN_DURATION);
            break;

        case LOAN_INCREASE: {
            int headroom = max_loan - p->loan_amount;
            int take = d.amount < headroom ? d.amount : headroom;
            if (take > 0){
                p->loan_amount += take;
                p->cash        += take;
                printf("%s increased the loan by LKR %s.\n\n",
                       player_name(p->id), lkr(take));
                printf("Outstanding Balance :\nLKR %s.\n\n", lkr(p->loan_amount));
            }
            break;
        }

        default: break;
    }
}

/* ---- Bankruptcy (Rule 14) ----------------------------------------------
 * Rule 14: a player becomes bankrupt when liabilities exceed available
 * assets. All buildings are removed, insurance policies expire, loans become
 * immediately due, and remaining assets transfer according to the bankruptcy
 * rules - which Rule-LK 19 defines as liquidation by auction.
 *
 * The flag is set separately from the liquidation because bankruptcy is
 * detected inside a payment, while liquidation runs auctions that themselves
 * take payments. Setting the flag and liquidating in one step would re-enter
 * the payment path with the board mid-transaction, so the turn loop
 * liquidates afterwards instead.
 */
void declare_bankrupt(Player *p){
    if (p->bankrupt) return;

    p->bankrupt = true;
    printf("%s has been declared bankrupt.\n\n", player_name(p->id));
}

/* Rule-LK 19: a bankrupt player's assets are liquidated by auction.
 * Rule 14: buildings are removed and loans become immediately due. */
void liquidate_assets(Player *p, Player *players, Square *board){
    /* Rule 14: loans become immediately due. With no assets left to settle
     * them the debt is written off, as in Rule-LK 6 foreclosure. */
    p->loan_amount   = 0;
    p->loan_duration = 0;

    for (int i = 0; i < BOARD_SIZE; i++){
        Square *s = &board[i];

        if (!s->purchasable) continue;
        if (get_owner(s) != p->owner_id) continue;

        demolish_buildings(s);          /* Rule 14: all buildings removed */
        set_mortgaged(s, false);
        set_loan_locked(s, false);

        /* start_auction only accepts unowned squares, so ownership is
         * released to the Bank before the square goes under the hammer.
         * Rule-LK 23: if nobody bids, it simply stays with the Bank. */
        set_owner(s, OG_BANK);
        start_auction(players, board, i);
    }

    /* TODO: Rule 14 also expires insurance policies. Not implemented. */

    printf("Remaining assets transferred to the Bank.\n\n");
}

/* Rule 15: the game concludes when only one player remains solvent. */
int solvent_count(const Player *players){
    int n = 0;
    for (int i = 0; i < NO_OF_PLAYERS; i++){
        if (!players[i].bankrupt) n++;
    }
    return n;
}
