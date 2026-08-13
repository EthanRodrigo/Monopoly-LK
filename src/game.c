#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h> 
#include <time.h>
#include "players.h"
#include "board.h"
#include "game.h"
#include "finance.h"

/* Roll both dice.
 * @param d1, d2 Die values, may be NULL if the caller only wants the sum
 * @return the sum
 */
int roll_dice(int *d1, int *d2){
    int a = (rand() % 6) + 1;
    int b = (rand() % 6) + 1;
    if (d1) *d1 = a;
    if (d2) *d2 = b;
    return a + b;
}

int roll(void){
    return roll_dice(NULL, NULL);
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Sort the play_order array according to the sum array
 * @param order The play_order array which has the order of the players
 * @param sum The sum array containing the sums of the dice rolls
 * @param len Length of both arrays. Should be equal for both arrays
 * */
void swap_sort(int *order, int *sum, int len){
    for(int i = 0; i < len - 1; i++){
        for(int j = 0; j < len - i - 1; j++){
            if (sum[order[j]] < sum[order[j+1]]){
                swap(&order[j], &order[j+1]);
            }
        }    
    }
}

/* Find the order which players roll the dices 
 * @param play_order The play_order array to store the order
 * @param sum The sum array to store the sums
 * @param len Length of both arrays. Should be equal for both arrays
 * */
void find_roll_order(int* play_order, int* sum, int len){
    for(int i = 0; i < len; i++){
        sum[play_order[i]] = roll();
    } 
    swap_sort(play_order, sum, len);

    int i = 0;
    while(i < len){
        int ties = 1;

        // grouping the equal sums to perform re-roll 
        while(i + ties < len && sum[play_order[i]] == sum[play_order[i + ties]]){
            ties++;
        }

        if (ties > 1){
            find_roll_order(&play_order[i], sum, ties);
        }
        i += ties;
    }
}

void pass_start(Player* p, GameStat* game){
    printf("Player round %d finishing... \n", p->player_rounds);
    p->player_rounds += 1;
    p->cash += 2000;

    game->mark_player_game_rounds(&(game->players_passed_go), p->id);
}

/* Move the player to the rolled square
 * @param p The pointer to the current player
 * @param b The array of squares (i.e. the board)
 * @param roll The rolled sum of the two dices
 * @return The type of the square 
 * */
void move(Player* p, int roll, GameStat* game){
    int old_position = p->position;
    p->position = resolve_out_of_bounds(old_position, roll);

    if (p->position < old_position){
        pass_start(p, game);
    }
}

void mark_game_round(uint8_t* bitmap, int player_id){
    *bitmap |= (1 << player_id);
}

void reset_game_round(uint8_t* bitmap){
    *bitmap = 0;
}

/* Rule 12: transfers the player to Jail without collecting GO money.
 * Deliberately does NOT call move() - the backwards jump from square 30 to
 * square 10 would trip move()'s wraparound test and wrongly award LKR 2,000. */
static void send_to_jail(Player *p){
    p->position = JAIL_SQUARE;
    p->in_jail = true;
    p->jail_turns = 0;
}

/* Rule 3 step 1: resolve outstanding penalties.
 *
 * Rule 13 gives three exits from jail: pay LKR 300 bail, roll doubles, or
 * remain imprisoned for three turns.
 *
 * SIMPLIFICATION: every player pays bail when they can afford it, and rolls
 * for doubles otherwise. The spec gives no per-strategy guidance on bail, and
 * at 1% of starting cash the decision is not a meaningful differentiator -
 * inventing a split would be unsupported by Rule 13 or section 3.
 *
 * INTERPRETATION: Rule 13 lists "remaining imprisoned for three turns" as an
 * exit in its own right, so no bail is charged on release after three turns.
 * Standard Monopoly charges it; the spec's wording does not.
 *
 * @return true if the player takes a normal turn (roll and move) this turn.
 */
static bool resolve_jail(Player *p, GameStat *game){
    if (!p->in_jail) return true;

    if (p->cash >= BAIL_AMOUNT){
        p->cash -= BAIL_AMOUNT;
        p->in_jail = false;
        printf("  -> JAIL: player %d paid bail %d. Cash now %d\n",
               p->id, BAIL_AMOUNT, p->cash);
        return true;
    }

    int d1, d2;
    int val = roll_dice(&d1, &d2);

    if (d1 == d2){
        p->in_jail = false;
        printf("  -> JAIL: player %d rolled doubles (%d+%d), released.\n",
               p->id, d1, d2);
        move(p, val, game);   
        return false;         
    }

    p->jail_turns++;

    if (p->jail_turns >= MAX_JAIL_TURNS){
        p->in_jail = false;
        p->jail_turns = 0;
        printf("  -> JAIL: player %d served %d turns, released.\n",
               p->id, MAX_JAIL_TURNS);
        return false;         /* released, but does not move until next turn */
    }

    printf("  -> JAIL: player %d remains (turn %d of %d).\n",
           p->id, p->jail_turns, MAX_JAIL_TURNS);
    return false;
}

/* Rule 3 step 4: resolve landing action.
 * Runs after move() and before buy_property(), matching the turn sequence:
 * roll -> move -> resolve landing -> purchase -> construct.
 *
 * Only TAX is implemented. Every other case is a placeholder - each needs a
 * subsystem that does not exist yet.
 */
static void resolve_landing(Player *p, Square *board, GameStat *game){
    Square *s = &board[p->position];
    (void)game;   /* unused until events / jail need round state */

    switch (s->type){
        case TAX: {
            /* Rule 11: payment is immediate. Paid to the Bank, which has
             * unlimited money, so the cash simply leaves circulation. */
            int paid = pay_income_tax(p, &s->data.tax);
            printf("  -> TAX: player %d paid %d. Cash now %d\n",
                   p->id, paid, p->cash);
            break;
        }

        /* TODO: Rule 7 - landing on an owned property/railway/utility pays
         * rent to the owner. Blocked on rent calculation: Table 6 needs
         * building counts, Table 7 needs a count of stations owned by one
         * player, Table 8 needs the dice value. No rent is collected if the
         * square is mortgaged. */
        case PROPERTY:
        case RAILWAY:
        case UTILITY:
            break;

        /* TODO: Appendix A - draw the top National Event Card, apply it,
         * return it to the bottom of the deck. Needs events.c. */
        case EVENT:
            break;

        /* TODO: Rule-LK 8 / section 1.2 - purchase or renew one of three
         * policies on a single property. Needs finance.c insurance handling
         * and a policy-expiry counter on the player. */
        case INSURANCE:
            break;

        /* TODO: Rule-LK 5 - one financial transaction: obtain, repay,
         * refinance, extend, or increase a loan. Needs finance.c. */
        case BANK:
            break;

       case START:
            break;

        case SPECIAL:
            switch (s->data.special.kind){
                case GO_TO_JAIL:
                    /* Rule 12: immediate transfer, no GO money. */
                    send_to_jail(p);
                    printf("  -> JAIL: player %d sent to jail.\n", p->id);
                    break;

                case JAIL_VISITING:
                    /* Landing here by rolling is "just visiting" - no effect.
                     * Only send_to_jail() sets in_jail. */
                    break;

                case FREE_PARKING:
                    /* The spec assigns Free Parking no effect under
                     * traditional rules, and no extension changes that.
                     * Deliberately a no-op, not an oversight. */
                    break;
            }
            break;

        default:
            break;
    }
}

void start_simulation(void){
    srand((unsigned int)time(NULL));
    GameStat game;
    game.game_round = 1;
    game.players_passed_go = 0;
    game.mark_player_game_rounds = mark_game_round;
    game.reset_player_game_rounds = reset_game_round;

    Square board[BOARD_SIZE];
    draw_board(board);

    Player players[NO_OF_PLAYERS];
    initialize_players(players);

    int play_order[NO_OF_PLAYERS] = {0, 1, 2, 3};
    int sum[NO_OF_PLAYERS] = {0, 0, 0, 0};
    find_roll_order(play_order, sum, NO_OF_PLAYERS);

    for (int i = 0; i < NO_OF_PLAYERS; i++) printf("%d \t", play_order[i]);
    putc('\n', stdout);

    while (game.game_round <= MAX_ROUNDS){
        for (int i = 0; i < NO_OF_PLAYERS; i++){
            Player *player = &players[play_order[i]];

            /* Rule 3 step 1: resolve outstanding penalties */
            if (!resolve_jail(player, &game)) continue;
            
            /* TODO: Rule 3 step 1 - resolve outstanding penalties (jail
             * turns, unpaid debt). Needs jail and debt recovery. */

            /* Rule 3 step 2-3: roll and move */
            int val = roll();
            move(player, val, &game);

            printf("roll: %d\n", val);
            printf("player: %d square: %s\n",
                   player->id, board[player->position].name);

            /* Rule 3 step 4: resolve landing action */
            resolve_landing(player, board, &game);

            /* Rule 3 step 5: purchase property if eligible */
            int cash_before = player->cash;
            int result = player->buy_property(player, board);

            if (result == BUY_BOUGHT){
                printf("  -> BOUGHT for %d. Cash %d -> %d. Owner: %d\n",
                       cash_before - player->cash, cash_before, player->cash,
                       get_owner(&board[player->position]));
            } else if (result == BUY_DECLINED){
                /* Rule 5: a declined property immediately enters auction */
                printf("  -> DECLINED %s. Auction starting.\n",
                       board[player->position].name);
                int winner = start_auction(players, board, player->position);

                if (winner >= 0){
                    printf("  -> AUCTION won by player %d. Cash now %d\n",
                           players[winner].id, players[winner].cash);
                } else {
                    printf("  -> AUCTION: no bids, stays with the Bank.\n");
                }
            }

            /* TODO: Rule 3 step 6 - construct buildings if eligible.
             * build_property takes a Group, so this needs a loop over all
             * eight groups, or the signature change discussed (drop the
             * group, let each strategy pick its own order). */

            /* TODO: Rule 3 step 7 - complete financial transactions.
             * Needs finance.c. */
        }

        /* A game round completes only when all four players have individually
         * passed GO - the professor's definition, tracked by bitmask. */
        if (game.players_passed_go == (1 << NO_OF_PLAYERS) - 1){
            printf("Game Round: %d\n", game.game_round);
            /* TODO: Section 5 "Round N Summary" block - per player: cash,
             * net worth, properties, hotels, outstanding loan. */
            game.game_round++;
            game.reset_player_game_rounds(&(game.players_passed_go));
        }
    }

    /* TODO: Section 5 "End of Game" block - GAME OVER, winner by Rule 15 net
     * worth, total cash, total property value, outstanding loans, net worth.
     * TODO: Rule 15 also ends the game early when only one player remains
     * solvent - needs bankruptcy. */
}
