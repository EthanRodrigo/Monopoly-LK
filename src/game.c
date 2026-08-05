#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h> 
#include <time.h>
#include "players.h"
#include "board.h"
#include "game.h"
/* roll both dice for a player
 * @return The sum of the both rolls
 * */
int roll(){
    return ((rand() % 6) + 1) + ((rand() %6) + 1);
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
int move(Player* p, Square* b, int roll, GameStat* game){
    int old_position = p->position;
    p->position = (old_position + roll) % BOARD_SIZE;   // can't be 40+

    if (p->position < old_position){
        pass_start(p, game);
    }
    return b[p->position].type;
}

void mark_game_round(uint8_t* bitmap, int player_id){
    *bitmap |= (1 << player_id);
}

void reset_game_round(uint8_t* bitmap){
    *bitmap = 0;
}

void start_simulation(void){
    srand((unsigned int)time(NULL));
    GameStat game;
    game.game_round = 0;
    game.players_passed_go = 0;
    game.mark_player_game_rounds = mark_game_round;
    game.reset_player_game_rounds = reset_game_round;

    Square board[BOARD_SIZE];
    draw_board(board);

    Player players[4];
    initialize_players(players);
    
    int play_order[NO_OF_PLAYERS] = {0, 1, 2, 3};
    int sum[NO_OF_PLAYERS] = {0, 0, 0, 0};
    find_roll_order(play_order, sum, NO_OF_PLAYERS);

    // TODO: The 500 rounds loop
    while (game.game_round < 500){
        // Player turn; roll, move, action
        for(int i = 0; i < NO_OF_PLAYERS; i++){
            int val = roll();
            int square = move(&players[play_order[i]], board, val, &game);
            printf("%s \t", board[square].name);
        }
        printf("Game Round: %d\n", game.game_round);
        if (game.players_passed_go == 15) {
            game.game_round++;
            game.reset_player_game_rounds(&(game.players_passed_go));
        }
    }
}
