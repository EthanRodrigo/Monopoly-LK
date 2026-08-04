#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "players.h"
#include "game.h"

int main(){
//	draw_board();
//	initialize_players();	
    srand((unsigned int)time(NULL));

    int order[4] = {0, 1, 2, 3};
    int sum[4]   = {0, 0, 0, 0};

    // Main entry point
    find_roll_order(order, sum, 4);

    // Output final results
    printf("\n=============================\n");
    printf("FINAL TURN ORDER:\n");
    for (int i = 0; i < 4; i++) {
        int player_id = order[i];
        printf("Position %d: Player %d (Latest Sum: %d)\n", i + 1, player_id, sum[player_id]);
    }
    return 0;

}
