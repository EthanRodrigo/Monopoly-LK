#include <stdio.h>
#include "players.h"

void initialize_players(){
	Player players[4] = {
		[0] = {
			.type = AGGRESSIVE_INVESTOR,
			.net_worth = 0
		},
		
		[1] = {
			.type = CONSERVATIVE_BANKER,
			.net_worth = 0
		},
		
		[2] = {
			.type = RISK_TAKER,
			.net_worth = 0
		},

		[3] = {
			.type = OPPORTUNISTIC_TRADER,
			.net_worth = 0
		},
	};

	for(int i = 0; i < 4; i++){
		printf("%d\n", players[i].type);
	}
} 
