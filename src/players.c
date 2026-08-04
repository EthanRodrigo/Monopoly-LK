#include <string.h>
#include "players.h"

void initialize_players(Player* players){
	Player temp_players[4] = {
		[0] = {
			.type = AGGRESSIVE_INVESTOR,
			.net_worth = 30000
		},
		
		[1] = {
			.type = CONSERVATIVE_BANKER,
			.net_worth = 30000
		},
		
		[2] = {
			.type = RISK_TAKER,
			.net_worth = 30000
		},

		[3] = {
			.type = OPPORTUNISTIC_TRADER,
			.net_worth = 30000
		},
	};
    
    memcpy(players, temp_players, sizeof(Player) * 4);
} 
