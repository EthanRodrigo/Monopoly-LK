#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "players.h"

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
