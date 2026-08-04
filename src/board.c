#include <string.h>
#include "board.h"

void pass_start(Player *p){
   p->net_worth += 2000; 
}

void draw_board(Square* board){
	Square temp_board[40] = {
		[0] = {
			.type = START,
			.name = "GO",
			.data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[1] = {
			.type = PROPERTY,
			.name = "Pettah",
			.data.property = {
				.group = BROWN,
				.purchase_price = 1500,
				.mortgage_value = 750,
				.base_rental = 0,
				.house_const_cost = 500,
				.hotel_const_cost = 2000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[2] = {
			.type = EVENT,
			.name = "Community Development Fund"
		},

		[3] = {
			.type = PROPERTY,
			.name = "Maradana",
			.data.property = {
				.group = BROWN,
				.purchase_price = 1500,
				.mortgage_value = 750,
				.base_rental = 0,
				.house_const_cost = 500,
				.hotel_const_cost = 2000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[4] = {
			.type = TAX,
			.name = "Income Tax"
		},

		[5] = {
			.type = RAILWAY,
			.name = "Colombo Fort Railway Station",
			.data.railway = {
				.owner = OG_BANK
			}
		},

		[6] = {
			.type = PROPERTY,
			.name = "Bambalapitiya",
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 2500,
				.mortgage_value = 1250,
				.base_rental = 0,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[7] = {
			.type = EVENT,
			.name = "National Event Card"
		},

		[8] = {
			.type = PROPERTY,
			.name = "Wellawatte",
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 2500,
				.mortgage_value = 1250,
				.base_rental = 0,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[9] = {
			.type = PROPERTY,
			.name = "Mount Lavinia",
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 2500,
				.mortgage_value = 1250,
				.base_rental = 0,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		// TODO: Change this to Special: Jail / Just Visiting
		[10] = {
			.type = START,
			.name = "Jail / Just Visiting",
            .data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[11] = {
			.type = PROPERTY,
			.name = "Nugegoda",
			.data.property = {
				.group = PINK,
				.purchase_price = 3500,
				.mortgage_value = 1750,
				.base_rental = 0,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[12] = {
			.type = UTILITY,
			.name = "Ceylon Electricity Board",
			.data.utility = {
				.owner = OG_BANK,
			}
		},

		[13] = {
			.type = PROPERTY,
			.name = "Maharagama",
			.data.property = {
				.group = PINK,
				.purchase_price = 3500,
				.mortgage_value = 1750,
				.base_rental = 0,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[14] = {
			.type = PROPERTY,
			.name = "Kottawa",
			.data.property = {
				.group = PINK,
				.purchase_price = 3500,
				.mortgage_value = 1750,
				.base_rental = 0,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[15] = {
			.type = RAILWAY,
			.name = "Kandy Railway Station",
			.data.railway = {
				.owner = OG_BANK,
			}
		},

		[16] = {
			.type = PROPERTY,
			.name = "Negombo",
			.data.property = {
				.group = ORANGE,
				.purchase_price = 4500,
				.mortgage_value = 2250,
				.base_rental = 0,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[17] = {
			.type = INSURANCE,
			.name = "Sri Lanka Insurance",
			.data.insurance = {
				.name = "Sri Lanka Insurance",
				.type = NULL
			}
		},

		[18] = {
			.type = PROPERTY,
			.name = "Katunayake",
			.data.property = {
				.group = ORANGE,
				.purchase_price = 4500,
				.mortgage_value = 2250,
				.base_rental = 0,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[19] = {
			.type = PROPERTY,
			.name = "Ja-Ela",
			.data.property = {
				.group = ORANGE,
				.purchase_price = 4500,
				.mortgage_value = 2250,
				.base_rental = 0,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		// TODO: Change this to Special: Jail / Just Visiting
		[20] = {
			.type = START,
			.name = "Free Parking",
            .data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[21] = {
			.type = PROPERTY,
			.name = "Kandy City",
			.data.property = {
				.group = RED,
				.purchase_price = 5500,
				.mortgage_value = 2750,
				.base_rental = 0,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[22] = {
			.type = EVENT,
			.name = "National Event Card"
		},

		[23] = {
			.type = PROPERTY,
			.name = "Peradeniya",
			.data.property = {
				.group = RED,
				.purchase_price = 5500,
				.mortgage_value = 2750,
				.base_rental = 0,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[24] = {
			.type = PROPERTY,
			.name = "Katugastota",
			.data.property = {
				.group = RED,
				.purchase_price = 5500,
				.mortgage_value = 2750,
				.base_rental = 0,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[25] = {
			.type = RAILWAY,
			.name = "Galle Railway Station",
			.data.railway = {
				.owner = OG_BANK,
			}
		},

		[26] = {
			.type = PROPERTY,
			.name = "Galle Fort",
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6500,
				.mortgage_value = 3250,
				.base_rental = 0,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[27] = {
			.type = PROPERTY,
			.name = "Unawatuna",
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6500,
				.mortgage_value = 3250,
				.base_rental = 0,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK, 
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[28] = {
			.type = UTILITY,
			.name = "National Water Supply and Drainage Board",
			.data.utility = {
				.owner = OG_BANK,
			}
		},

		[29] = {
			.type = PROPERTY,
			.name = "Hikkaduwa",
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6500,
				.mortgage_value = 3250,
				.base_rental = 0,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		// TODO: Change this to Special: Jail / Just Visiting
		[30] = {
			.type = START,
			.name = "Go To Jail",
            .data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[31] = {
			.type = PROPERTY,
			.name = "Jaffna Town",
			.data.property = {
				.group = GREEN,
				.purchase_price = 8000,
				.mortgage_value = 4000,
				.base_rental = 0,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[32] = {
			.type = PROPERTY,
			.name = "Nallur",
			.data.property = {
				.group = GREEN,
				.purchase_price = 8000,
				.mortgage_value = 4000,
				.base_rental = 0,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[33] = {
			.type = INSURANCE,
			.name = "Ceylinco Insurance",
			.data.insurance = {
				.type = NULL
			}
		},

		[34] = {
			.type = PROPERTY,
			.name = "Trincomalee",
			.data.property = {
				.group = GREEN,
				.purchase_price = 8000,
				.mortgage_value = 4000,
				.base_rental = 0,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[35] = {
			.type = RAILWAY,
			.name = "Jaffna Railway Station",
			.data.railway = {
				.owner = OG_BANK,
			}
		},

		[36] = {
			.type = EVENT,
			.name = "National Event Card"
		},

		[37] = {
			.type = PROPERTY,
			.name = "Nuwara Eliya",
			.data.property = {
				.group = DARK_BLUE,
				.purchase_price = 10000,
				.mortgage_value = 5000,
				.base_rental = 0,
				.house_const_cost = 3000,
				.hotel_const_cost = 12000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[38] = {
			.type = BANK,
			.name = "Bank of Ceylon"
		},

		[39] = {
			.type = PROPERTY,
			.name = "Galle Face",
			.data.property = {
				.group = DARK_BLUE,
				.purchase_price = 10000,
				.mortgage_value = 5000,
				.base_rental = 0,
				.house_const_cost = 3000,
				.hotel_const_cost = 12000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		}
	};

    memcpy(board, temp_board, sizeof(Square) * 40);
}
