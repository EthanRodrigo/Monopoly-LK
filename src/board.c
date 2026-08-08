#include <string.h>
#include "board.h"

void draw_board(Square* board){
	Square temp_board[40] = {
		[0] = {
			.type = START,
			.name = "GO",
			.purchasable = false,
			.data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[1] = {
			.type = PROPERTY,
			.name = "Pettah",
			.purchasable = true,
			.data.property = {
				.group = BROWN,
				.purchase_price = 1500,
				.mortgage_value = 750,
				.base_rental = 100,
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
			.name = "Community Development Fund",
			.purchasable = false
		},

		[3] = {
			.type = PROPERTY,
			.name = "Maradana",
			.purchasable = true,
			.data.property = {
				.group = BROWN,
				.purchase_price = 1800,
				.mortgage_value = 900,
				.base_rental = 120,
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
			.name = "Income Tax",
			.purchasable = false
		},

		[5] = {
			.type = RAILWAY,
			.name = "Colombo Fort Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 10 // TODO: find the railway purchase price
			}
		},

		[6] = {
			.type = PROPERTY,
			.name = "Bambalapitiya",
			.purchasable = true,
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 2500,
				.mortgage_value = 1250,
				.base_rental = 180,
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
			.name = "National Event Card",
			.purchasable = false
		},

		[8] = {
			.type = PROPERTY,
			.name = "Wellawatte",
			.purchasable = true,
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 2700,
				.mortgage_value = 1350,
				.base_rental = 200,
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
			.purchasable = true,
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 3000,
				.mortgage_value = 1500,
				.base_rental = 220,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[10] = {
			.type = START,
			.name = "Jail / Just Visiting",
			.purchasable = false,
            .data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[11] = {
			.type = PROPERTY,
			.name = "Nugegoda",
			.purchasable = true,
			.data.property = {
				.group = PINK,
				.purchase_price = 3500,
				.mortgage_value = 1750,
				.base_rental = 260,
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
			.purchasable = true,
			.data.utility = {
				.owner = OG_BANK,
                .purchase_price = 1500,
                .base_rental = 0
			}
		},

		[13] = {
			.type = PROPERTY,
			.name = "Maharagama",
			.purchasable = true,
			.data.property = {
				.group = PINK,
				.purchase_price = 3800,
				.mortgage_value = 1900,
				.base_rental = 280,
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
			.purchasable = true,
			.data.property = {
				.group = PINK,
				.purchase_price = 4000,
				.mortgage_value = 2000,
				.base_rental = 300,
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
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 10 // TODO: find the railway purchase price
			}
		},

		[16] = {
			.type = PROPERTY,
			.name = "Negombo",
			.purchasable = true,
			.data.property = {
				.group = ORANGE,
				.purchase_price = 4500,
				.mortgage_value = 2250,
				.base_rental = 350,
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
			.purchasable = false,
			.data.insurance = {
				.name = "Sri Lanka Insurance",
				.type = NULL
			}
		},

		[18] = {
			.type = PROPERTY,
			.name = "Katunayake",
			.purchasable = true,
			.data.property = {
				.group = ORANGE,
				.purchase_price = 4700,
				.mortgage_value = 2350,
				.base_rental = 370,
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
			.purchasable = true,
			.data.property = {
				.group = ORANGE,
				.purchase_price = 5000,
				.mortgage_value = 2500,
				.base_rental = 400,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[20] = {
			.type = START,
			.name = "Free Parking",
			.purchasable = false,
            .data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[21] = {
			.type = PROPERTY,
			.name = "Kandy City",
			.purchasable = true,
			.data.property = {
				.group = RED,
				.purchase_price = 5500,
				.mortgage_value = 2750,
				.base_rental = 450,
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
			.name = "National Event Card",
			.purchasable = false
		},

		[23] = {
			.type = PROPERTY,
			.name = "Peradeniya",
			.purchasable = true,
			.data.property = {
				.group = RED,
				.purchase_price = 5800,
				.mortgage_value = 2900,
				.base_rental = 480,
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
			.purchasable = true,
			.data.property = {
				.group = RED,
				.purchase_price = 6000,
				.mortgage_value = 3000,
				.base_rental = 500,
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
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 10 // TODO: find the railway purchase price
			}
		},

		[26] = {
			.type = PROPERTY,
			.name = "Galle Fort",
			.purchasable = true,
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6500,
				.mortgage_value = 3250,
				.base_rental = 600,
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
			.purchasable = true,
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6800,
				.mortgage_value = 3400,
				.base_rental = 620,
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
			.purchasable = true,
			.data.utility = {
				.owner = OG_BANK,
                .purchase_price = 1500,
                .base_rental = 0
			}
		},

		[29] = {
			.type = PROPERTY,
			.name = "Hikkaduwa",
			.purchasable = true,
			.data.property = {
				.group = YELLOW,
				.purchase_price = 7000,
				.mortgage_value = 3500,
				.base_rental = 650,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.num_of_buildings = 0
			}
		},

		[30] = {
			.type = START,
			.name = "Go To Jail",
			.purchasable = false,
            .data.start = {
				.award = 2000,
                .pass_start = NULL,
			}
		},

		[31] = {
			.type = PROPERTY,
			.name = "Jaffna Town",
			.purchasable = true,
			.data.property = {
				.group = GREEN,
				.purchase_price = 8000,
				.mortgage_value = 4000,
				.base_rental = 750,
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
			.purchasable = true,
			.data.property = {
				.group = GREEN,
				.purchase_price = 8300,
				.mortgage_value = 4150,
				.base_rental = 780,
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
			.purchasable = false,
			.data.insurance = {
				.type = NULL
			}
		},

		[34] = {
			.type = PROPERTY,
			.name = "Trincomalee",
			.purchasable = true,
			.data.property = {
				.group = GREEN,
				.purchase_price = 8500,
				.mortgage_value = 4250,
				.base_rental = 800,
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
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 10 // TODO: find the railway purchase price
			}
		},

		[36] = {
			.type = EVENT,
			.name = "National Event Card",
			.purchasable = false
		},

		[37] = {
			.type = PROPERTY,
			.name = "Nuwara Eliya",
			.purchasable = true,
			.data.property = {
				.group = DARK_BLUE,
				.purchase_price = 10000,
				.mortgage_value = 5000,
				.base_rental = 1000,
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
			.name = "Bank of Ceylon",
			.purchasable = false
		},

		[39] = {
			.type = PROPERTY,
			.name = "Galle Face",
			.purchasable = true,
			.data.property = {
				.group = DARK_BLUE,
				.purchase_price = 12000,
				.mortgage_value = 6000,
				.base_rental = 1200,
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

// resolve the roll getting a 40+
int resolve_out_of_bounds(int curr_pos, int offset){
    return (curr_pos + offset) % 40;
}

Owner get_owner(const Square *s) {
    if (!s->purchasable) return OG_BANK;
    switch (s->type) {
        case PROPERTY: return s->data.property.owner;
        case RAILWAY:  return s->data.railway.owner;
        case UTILITY:  return s->data.utility.owner;
        default:       return OG_BANK;
    }
}

int get_purchase_price(const Square *s) {
    if (!s->purchasable) return 0;
    switch (s->type) {
        case PROPERTY: return s->data.property.purchase_price;
        case RAILWAY:  return s->data.railway.purchase_price;
        case UTILITY:  return s->data.utility.purchase_price;
        default:       return 0;
    }
}

int get_rent(const Square *s) {
    if (!s->purchasable) return 0;
    switch (s->type) {
        case PROPERTY: return s->data.property.base_rental;
        case RAILWAY:  return s->data.railway.base_rental;
        case UTILITY:  return s->data.utility.base_rental;
        default:       return 0;
    }
}

void set_owner(Square *s, Owner new_owner) {
    if (!s->purchasable) return;
    switch (s->type) {
        case PROPERTY: s->data.property.owner = new_owner; break;
        case RAILWAY:  s->data.railway.owner  = new_owner; break;
        case UTILITY:  s->data.utility.owner  = new_owner; break;
        default: break;
    }
}


