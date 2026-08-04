#include <string.h>
#include "players.h"
#include "board.h"

void initialize_players(Player* players){
	Player temp_players[NO_OF_PLAYERS] = {
		[0] = {
			.type = AGGRESSIVE_INVESTOR,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},
		
		[1] = {
			.type = CONSERVATIVE_BANKER,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},
		
		[2] = {
			.type = RISK_TAKER,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},

		[3] = {
			.type = OPPORTUNISTIC_TRADER,
			.cash = 30000,
            .net_worth = 30000,
            .position = START
		},
	};
    
    memcpy(players, temp_players, sizeof(Player) * NO_OF_PLAYERS);
} 
