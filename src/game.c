#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include "players.h"
#include "board.h"

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

void pass_start(Player* p){
    p->cash += 2000;
}

// Pointer to the player
// Board array`
int move(Player* p, Square* b, int roll){
    int old_position = p->position;
    p->position = (old_position + roll) % BOARD_SIZE;

    if (p->position < old_position){
        pass_start(p);
    }
    return b[p->position].type;
}

void start_simulation(void){
    srand((unsigned int)time(NULL));

    Square board[BOARD_SIZE];
    draw_board(board);

    Player players[4];
    initialize_players(players);
    
    int play_order[NO_OF_PLAYERS] = {0, 1, 2, 3};
    int sum[NO_OF_PLAYERS] = {0, 0, 0, 0};
    find_roll_order(play_order, sum, NO_OF_PLAYERS);

    // TODO: The 500 rounds loop
    int game_rounds = 500;
    while (game_rounds >= 0){
        for(int i = 0; i < NO_OF_PLAYERS; i++){
            int val = roll();
            int square = move(&players[play_order[i]], board, val);
            printf("%d \t", square);
        }
        putc(10, stdout);
        game_rounds--;
    }
}
