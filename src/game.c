#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "players.h"
#include "board.h"
#include "game.h"
#include "finance.h"
#include "events.h"
#include "events.h"

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

/* Rule 4: passing or landing on GO awards LKR 2,000. */
void pass_start(Player* p, GameStat* game){
    p->player_rounds += 1;
    p->cash += 2000;

    printf("%s passed GO.\n", player_name(p->id));
    printf("Collected LKR 2,000.\n");
    printf("Current Balance : LKR %s.\n\n", lkr(p->cash));

    mark_game_round(&(game->players_passed_go), p->id);
}

/* Move the player to the rolled square
 * @param p The pointer to the current player
 * @param roll The rolled sum of the two dices
 * */
void move(Player* p, int roll, GameStat* game){
    int old_position = p->position;
    p->position = resolve_out_of_bounds(old_position, roll);

    printf("%s moves from Square %d to Square %d.\n\n",
           player_name(p->id), old_position, p->position);

    if (p->position < old_position){
        pass_start(p, game);
    }
}

static bool resolve_jail(Player *p, Square *board, const GameStat *g){
    if (!p->in_jail) return true;

    if (p->cash >= BAIL_AMOUNT || raise_cash(p, board, BAIL_AMOUNT, g)){
        p->cash -= BAIL_AMOUNT;
        p->in_jail = false;
        printf("%s paid bail of LKR %s.\n", player_name(p->id), lkr(BAIL_AMOUNT));
        printf("Current Balance : LKR %s.\n\n", lkr(p->cash));
        return true;
    }

    int d1, d2;
    roll_dice(&d1, &d2);

    if (d1 == d2){
        p->in_jail = false;
        p->jail_turns = 0;
        printf("%s rolled doubles (%d and %d) and is released from Jail.\n\n",
               player_name(p->id), d1, d2);
        // Released only. The player takes a normal turn next round.
        return false;
    }

    p->jail_turns++;

    if (p->jail_turns >= MAX_JAIL_TURNS){
        p->in_jail = false;
        p->jail_turns = 0;
        printf("%s served %d turns and is released from Jail.\n\n",
               player_name(p->id), MAX_JAIL_TURNS);
        return false;         /* released, but does not move until next turn */
    }

    printf("%s remains in Jail (turn %d of %d).\n\n",
           player_name(p->id), p->jail_turns, MAX_JAIL_TURNS);
    return false;
}

static void collect_rent(Player *p, Player *players, Square *board,
                         Square *s, int dice, const GameStat *g){
    Owner owner = get_owner(s);

    if (owner == OG_BANK) return;            /* unowned - purchase path */
    if (owner == p->owner_id){
       /* Rule-LK 17: landing on an owned property lets the owner renovate.
         * No rent is charged on your own square. */
        if (s->type == PROPERTY && p->should_renovate(p, s)){
            renovate_property(p, s, g);
        }
        return;
    }

    if (is_mortgaged(s)) return;   /* Rule 7: no rent on a mortgaged square */

    int rent = 0;
    switch (s->type){
        case PROPERTY: rent = property_rent(g, s);                  break;
        case RAILWAY:  rent = railway_rent(g, board, owner);         break;
        case UTILITY:  rent = utility_rent(g, board, owner, dice);   break;
        default:       return;
    }

    if (rent <= 0) return;

    Player *landlord = find_player(players, owner);
    if (!landlord) return;

    /* Mortgage property first if cash alone cannot cover the rent. */
    int paid = mortgage_and_pay(p, board, rent, g);
    landlord->cash += paid;

    printf("%s landed on %s.\n\n", player_name(p->id), s->name);
    printf("Rent Paid : LKR %s.\n\n", lkr(paid));
    printf("Owner : %s.\n\n", player_name(landlord->id));
}

/* Rule 3 step 4: resolve landing action.
 * Runs after move() and before buy_property(), matching the turn sequence:
 * roll -> move -> resolve landing -> purchase -> construct.
 */
static void resolve_landing(Player *p, Player *players, Square *board, GameStat *game){
    Square *s = &board[p->position];

    switch (s->type){
        case TAX: {
            int paid = pay_income_tax(p, board, &s->data.tax, game);
            printf("%s paid Income Tax of LKR %s.\n", player_name(p->id), lkr(paid));
            printf("Current Balance : LKR %s.\n\n", lkr(p->cash));
            break;
        }

        case PROPERTY:
        case RAILWAY:
        case UTILITY:
            collect_rent(p, players, board, s, game->last_roll, game);
            break;

        case EVENT:
            if (p->position == 2){
                int levy = pay_development_fund(p, board, game);
                printf("%s paid the Community Development Fund.\n",
                       player_name(p->id));
                printf("Levy : LKR %s.\n", lkr(levy));
                printf("Current Balance : LKR %s.\n\n", lkr(p->cash));
                break;
            }
            draw_event_card((GameStat*)game, p, players, board);
            break;

        /* TODO: Rule-LK 8 / section 1.2 - purchase or renew one of three
         * policies on a single property. Needs finance.c insurance handling
         * and a policy-expiry counter on the player. */
        case INSURANCE:
            break;

        case BANK:
            resolve_bank_visit(p, board, game->game_round, game);
            break; case START:
            break;

        case SPECIAL:
            switch (s->data.special.kind){
                case GO_TO_JAIL:
                    /* Rule 12: immediate transfer, no GO money. */
                    send_to_jail(p);
                    printf("%s was sent to Jail.\n", player_name(p->id));
                    printf("No GO money is collected.\n\n");
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

/* Section 5: the block printed before the game begins. */
static void print_opening(void){
    printf("MONOPOLY-LK Simulation\n\n");
    printf("Player 1 : %s\n", player_name(AGGRESSIVE_INVESTOR));
    printf("Player 2 : %s\n", player_name(CONSERVATIVE_BANKER));
    printf("Player 3 : %s\n", player_name(RISK_TAKER));
    printf("Player 4 : %s\n\n", player_name(OPPORTUNISTIC_TRADER));
    printf("Each player begins with LKR 30,000.\n\n");
}

/* Section 5: "Determining the First Player". */
static void print_roll_order(const Player *players, const int *play_order,
                             const int *sum){
    for (int i = 0; i < NO_OF_PLAYERS; i++){
        printf("%s rolls %d.\n", player_name(players[i].id), sum[i]);
    }
    printf("\n");

    printf("%s will begin the game.\n\n", player_name(players[play_order[0]].id));

    printf("Turn order:\n");
    for (int i = 0; i < NO_OF_PLAYERS; i++){
        printf("%s\n", player_name(players[play_order[i]].id));
    }
    printf("\n");
}

/* Section 5: the summary printed at the end of every completed round. */
static void print_round_summary(const Player *players, const Square *board,
                                int round, const GameStat *g){
    printf("=============================================\n");
    printf("Round %d Summary\n", round);
    printf("=============================================\n\n");

    for (int i = 0; i < NO_OF_PLAYERS; i++){
        const Player *p = &players[i];

        if (p->bankrupt){
            printf("%s\n\nBANKRUPT\n\n", player_name(p->id));
            if (i < NO_OF_PLAYERS - 1){
                printf("---------------------------------------------\n\n");
            }
            continue;
        }

        printf("%s\n\n", player_name(p->id));
        printf("Cash : LKR %s\n\n", lkr(p->cash));
        printf("Net Worth : LKR %s\n\n", lkr(calculate_net_worth(p, board, g)));
        printf("Properties : %d\n\n", count_properties(p, board));
        printf("Hotels : %d\n\n", count_hotels(p, board));
        /* Rule-LK 4: accrued interest is folded into loan_amount, so this is
         * the full outstanding balance, not just the principal. */
        if (p->loan_amount > 0){
            printf("Outstanding Loan : LKR %s\n\n", lkr(p->loan_amount));
        } else {
            printf("Outstanding Loan : None\n\n");
        }

        if (i < NO_OF_PLAYERS - 1){
            printf("---------------------------------------------\n\n");
        }
    }
    printf("=============================================\n\n");
}

/* Section 5: "End of Game". Rule 15 declares the player with the highest net
 * worth the winner when the round limit is reached. */
static void print_end_of_game(const Player *players, const Square *board, const GameStat *g){
    int best = -1;
    int best_worth = 0;

    for (int i = 0; i < NO_OF_PLAYERS; i++){
        if (players[i].bankrupt) continue;
        int worth = calculate_net_worth(&players[i], board, g);
        if (best < 0 || worth > best_worth){
            best_worth = worth;
            best = i;
        }
    }

    if (best < 0){
        printf("GAME OVER\n\nNO Solvent players remain.\n\n");
        return;
    }

    const Player *w = &players[best];

    printf("=============================================\n\n");
    printf("GAME OVER\n\n");
    printf("Winner\n\n");
    printf("%s\n\n", player_name(w->id));
    printf("Total Cash\n\n");
    printf("LKR %s\n\n", lkr(w->cash));
    printf("Total Property Value\n\n");
    printf("LKR %s\n\n", lkr(total_property_value(w, board, g)));
    printf("Outstanding Loans\n\n");
    if (w->loan_amount > 0){
        printf("LKR %s\n\n", lkr(w->loan_amount));
    } else {
        printf("None\n\n");
    }
    printf("Net Worth\n\n");
    printf("LKR %s\n\n", lkr(best_worth));
    printf("=============================================\n");
}

void start_simulation(void){
    srand(4484);   // fixed seed for reproducible runs 

    GameStat game;
    game.game_round        = 1;
    game.players_passed_go = 0;
    game.last_roll         = 0;
    game.inflation_rate    = 0;
    game.effect_count      = 0;
    game.construction_blocked_until = 0;
    for (int i = 0; i < 8; i++) game.group_last_event[i] = -100;
    game.effect_count = 0;
    
    init_event_deck(&game.deck);

    Square board[BOARD_SIZE];
    draw_board(board);

    Player players[NO_OF_PLAYERS];
    initialize_players(players);

    print_opening();

    int play_order[NO_OF_PLAYERS] = {0, 1, 2, 3};
    int sum[NO_OF_PLAYERS] = {0, 0, 0, 0};
    find_roll_order(play_order, sum, NO_OF_PLAYERS);

    print_roll_order(players, play_order, sum);

    while (game.game_round <= MAX_ROUNDS && solvent_count(players) > 1){
        for (int i = 0; i < NO_OF_PLAYERS; i++){
            Player *player = &players[play_order[i]];

            if (player->bankrupt){
                if (count_properties(player, board) > 0){
                    liquidate_assets(player, players, board, &game);
                }
                continue;
            }

            /* Rule 3 step 1: resolve outstanding penalties */
            if (!resolve_jail(player, board, &game)) continue;
            
            /* Rule-LK 27: maintenance may be performed only at the beginning
             * of a player's turn. */
            perform_maintenance(player, board, &game);

            /* Rule 3 step 2-3: roll and move */
            int val = roll();
            game.last_roll = val;

            printf("%s rolled %d.\n\n", player_name(player->id), val);

            move(player, val, &game);

            /* Rule 3 step 4: resolve landing action */
            resolve_landing(player, players, board, &game);

            /* Rule 3 step 5: purchase property if eligible */
            int cash_before = player->cash;
            int result = player->buy_property(player, board, &game);

            if (result == BUY_BOUGHT){
                printf("%s purchased %s for LKR %s.\n",
                       player_name(player->id),
                       board[player->position].name,
                       lkr(cash_before - player->cash));
                printf("Remaining Balance : LKR %s.\n\n", lkr(player->cash));
            } else if (result == BUY_DECLINED){
                /* Rule 5: a declined property immediately enters auction */
                printf("%s declined %s.\n\n",
                       player_name(player->id), board[player->position].name);
                start_auction(players, board, player->position, &game);
            }

            /* Rule 3 step 6: construct buildings if eligible. */
            if (construction_allowed(&game)){
                for (int grp = BROWN; grp <= DARK_BLUE; grp++){
                    player->build_property(player, board, (Group)grp, &game);
                }
            }

            /* Rule 14: liabilities exceeding assets also shows up as a
             * negative net worth once a loan outgrows what it secured. */
            if (!player->bankrupt && calculate_net_worth(player, board, &game) <= 0){
                declare_bankrupt(player);
            }
        }

        /* A game round completes when every SOLVENT player has passed GO.
         * Bankrupt players take no turns (Rule 14), so they are excluded from
         * the requirement rather than marked as having passed. */
        uint8_t required = 0;
        for (int n = 0; n < NO_OF_PLAYERS; n++){
            if (!players[n].bankrupt) required |= (1 << players[n].id);
        }

        if (required != 0 && (game.players_passed_go & required) == required){

            /* Rule-LK 4: interest compounds at the end of every complete
             * round, and loan maturity is measured in complete rounds. */
            for (int n = 0; n < NO_OF_PLAYERS; n++){
                accrue_loan_interest(&players[n]);
                check_loan_default(&players[n], board, game.game_round, &game);
            }

            print_round_summary(players, board, game.game_round, &game);

            /* Rule-LK 15: property age increases every complete round.
             * Rule-LK 25: building condition decreases every round. */
            age_properties(board);
            degrade_buildings(board);

            game.game_round++;
            reset_game_round(&(game.players_passed_go));

            /* Expire finished modifiers before new ones are added. */
            purge_expired(&game);

            /* Rule-LK 18: one national economic event every fifteen rounds.
             * Rule-LK 24: one government regulation every twenty rounds.
             * Rule-LK 30: the property market is reviewed every ten. */
            if (game.game_round % 15 == 0) trigger_economic_event(&game);
            if (game.game_round % 20 == 0) trigger_regulation(&game);
            if (game.game_round % 10 == 0) review_property_market(&game);

            /* Rule-LK 36: display the active market conditions each round. */
            print_market_conditions(&game);

            /* Rule-LK 12: an inflation rate is generated every ten rounds.
             * Rule-LK 14 applies it as New Value = Previous Value x (1 + rate),
             * and the effect compounds across the game. */
            if (game.game_round % 10 == 0){
                game.inflation_rate = generate_inflation_rate();

                if (game.inflation_rate != 0){
                    apply_inflation(board, game.inflation_rate);

                    printf("=============================================\n");
                    printf("Inflation\n");
                    printf("------------\n");
                    printf("%+d%%\n", game.inflation_rate);
                    printf("=============================================\n\n");
                    /* TODO: Rule-LK 13 also inflates loan interest rates,
                     * insurance premiums and repair costs. Table 9 ties the
                     * loan rate to economic condition, which needs the rest
                     * of events.c. */
               }
            }
        }
    }
    print_end_of_game(players, board, &game);
    printf("Game Rounds: %d\n", game.game_round);
}
