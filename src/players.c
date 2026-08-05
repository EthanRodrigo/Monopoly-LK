#include <string.h>
#include "players.h"
#include "board.h"

void initialize_players(Player* players){
	Player temp_players[NO_OF_PLAYERS] = {
		[0] = {
			.id = AGGRESSIVE_INVESTOR,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},
		
		[1] = {
			.id = CONSERVATIVE_BANKER,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},
		
		[2] = {
			.id = RISK_TAKER,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},

		[3] = {
			.id = OPPORTUNISTIC_TRADER,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},
	};
    
    memcpy(players, temp_players, sizeof(Player) * NO_OF_PLAYERS);
} 
