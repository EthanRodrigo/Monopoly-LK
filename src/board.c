#include <string.h>
#include "board.h"
#include "events.h"
#include "types.h"

void draw_board(Square* board){
	Square temp_board[BOARD_SIZE] = {
 
		[0] = {
			.type = START,
			.name = "GO",
			.purchasable = false,
            .data.start = {
                .award = 2000
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
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
				.mortgage_value = 750,
				.base_rental = 120,
				.house_const_cost = 500,
				.hotel_const_cost = 2000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[4] = {
			.type = TAX,
			.name = "Income Tax",
			.purchasable = false,
            .data.tax = {
                .base_rate = 15
            }
		},
 
		[5] = {
			.type = RAILWAY,
			.name = "Colombo Fort Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
				.purchase_price = 8000,
				.base_rental = 250,
				.mortgage_value = 4000,
				.mortgage_stat = false,
				.loan_locked = false
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
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
				.mortgage_value = 1250,
				.base_rental = 200,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[9] = {
			.type = PROPERTY,
			.name = "Mount Lavinia",
			.purchasable = true,
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 3000,
				.mortgage_value = 1250,
				.base_rental = 220,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[10] = {
			.type = SPECIAL,
			.name = "Jail / Just Visiting",
			.purchasable = false,
            .data.special = {
                .kind = JAIL_VISITING
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[12] = {
			.type = UTILITY,
			.name = "Ceylon Electricity Board",
			.purchasable = true,
			.data.utility = {
				.owner = OG_BANK,
				.purchase_price = 1500,
				.base_rental = 250,
				.mortgage_value = 750,
				.mortgage_stat = false,
				.loan_locked = false
			}
		},
 
		[13] = {
			.type = PROPERTY,
			.name = "Maharagama",
			.purchasable = true,
			.data.property = {
				.group = PINK,
				.purchase_price = 3800,
				.mortgage_value = 1750,
				.base_rental = 280,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[14] = {
			.type = PROPERTY,
			.name = "Kottawa",
			.purchasable = true,
			.data.property = {
				.group = PINK,
				.purchase_price = 4000,
				.mortgage_value = 1750,
				.base_rental = 300,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[15] = {
			.type = RAILWAY,
			.name = "Kandy Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
				.purchase_price = 8000,
				.base_rental = 250,
				.mortgage_value = 4000,
				.mortgage_stat = false,
				.loan_locked = false
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
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
				.mortgage_value = 2250,
				.base_rental = 370,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[19] = {
			.type = PROPERTY,
			.name = "Ja-Ela",
			.purchasable = true,
			.data.property = {
				.group = ORANGE,
				.purchase_price = 5000,
				.mortgage_value = 2250,
				.base_rental = 400,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[20] = {
			.type = SPECIAL,
			.name = "Free Parking",
			.purchasable = false,
            .data.special = {
                .kind = FREE_PARKING
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
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
				.mortgage_value = 2750,
				.base_rental = 480,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[24] = {
			.type = PROPERTY,
			.name = "Katugastota",
			.purchasable = true,
			.data.property = {
				.group = RED,
				.purchase_price = 6000,
				.mortgage_value = 2750,
				.base_rental = 500,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[25] = {
			.type = RAILWAY,
			.name = "Galle Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
				.purchase_price = 8000,
				.base_rental = 250,
				.mortgage_value = 4000,
				.mortgage_stat = false,
				.loan_locked = false
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[27] = {
			.type = PROPERTY,
			.name = "Unawatuna",
			.purchasable = true,
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6800,
				.mortgage_value = 3250,
				.base_rental = 620,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[28] = {
			.type = UTILITY,
			.name = "National Water Supply and Drainage Board",
			.purchasable = true,
			.data.utility = {
				.owner = OG_BANK,
				.purchase_price = 1500,
				.base_rental = 250,
				.mortgage_value = 750,
				.mortgage_stat = false,
				.loan_locked = false
			}
		},
 
		[29] = {
			.type = PROPERTY,
			.name = "Hikkaduwa",
			.purchasable = true,
			.data.property = {
				.group = YELLOW,
				.purchase_price = 7000,
				.mortgage_value = 3250,
				.base_rental = 650,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[30] = {
			.type = SPECIAL,
			.name = "Go To Jail",
			.purchasable = false,
            .data.special = {
                .kind = GO_TO_JAIL
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[32] = {
			.type = PROPERTY,
			.name = "Nallur",
			.purchasable = true,
			.data.property = {
				.group = GREEN,
				.purchase_price = 8300,
				.mortgage_value = 4000,
				.base_rental = 780,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[33] = {
			.type = INSURANCE,
			.name = "Ceylinco Insurance",
			.purchasable = false,
            .data.insurance = {
                .name = "Ceylinco Insurance",
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
				.mortgage_value = 4000,
				.base_rental = 800,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		},
 
		[35] = {
			.type = RAILWAY,
			.name = "Jaffna Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
				.purchase_price = 8000,
				.base_rental = 250,
				.mortgage_value = 4000,
				.mortgage_stat = false,
				.loan_locked = false
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
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
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
				.mortgage_value = 5000,
				.base_rental = 1200,
				.house_const_cost = 3000,
				.hotel_const_cost = 12000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false,
				.loan_locked = false,
				.age = 0,
				.depreciation = 0,
				.condition = 100,
				.rounds_since_maintenance = 0,
				.structural_damage = false
			}
		}
 
	};
 
    memcpy(board, temp_board, sizeof(Square) * BOARD_SIZE);
}

int resolve_out_of_bounds(int curr_pos, int offset){
    return (curr_pos + offset) % BOARD_SIZE;
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

int get_purchase_price(const GameStat *g, const Square *s) {
    if (!s->purchasable) return 0;

    int price;
    switch (s->type) {
        case PROPERTY: price = s->data.property.purchase_price; break;
        case RAILWAY:  price = s->data.railway.purchase_price;  break;
        case UTILITY:  price = s->data.utility.purchase_price;  break;
        default:       return 0;
    }

    /* Rule-LK 31: a Market Boom raises purchase prices 15%. */
    return (price * effect_pct(g, s, 0, EFF_PURCHASE_PRICE, get_owner(s)) + 50) / 100;
}

int get_rent(const GameStat *g, const Square *s) {
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

int min_houses_in_group(const Square *board, Group target_group) {
    int group_min = 5;  // higher than any real value (max is 4)
    for (int i = 0; i < BOARD_SIZE; i++) {
        const Square *s = &board[i];
        if (s->type != PROPERTY) continue;
        if (s->data.property.group != target_group) continue;

        int houses = s->data.property.has_hotel ? 4 : s->data.property.no_of_houses;
        if (houses < group_min) group_min = houses;
    }
    return group_min;
}

bool can_build_house(const Square *board, const Square *target, Owner owner) {
    if (target->type != PROPERTY) return false;
    if (target->data.property.owner != owner) return false;
    if (!has_monopoly(owner, board, target->data.property.group)) return false;
    if (target->data.property.has_hotel) return false;             
    if (target->data.property.no_of_houses >= 4) return false; 

    // only gain a house if it's tied for the fewest houses inits own group right now.
    int group_min = min_houses_in_group(board, target->data.property.group);
    return target->data.property.no_of_houses <= group_min;
}

bool can_build_hotel(const Square *board, const Square *target, Owner owner) {
    if (target->type != PROPERTY) return false;
    if (target->data.property.owner != owner) return false;
    if (!has_monopoly(owner, board, target->data.property.group)) return false;
    if (target->data.property.has_hotel) return false;             // already a hotel
    if (target->data.property.no_of_houses != 4) return false; // must have 4 houses first

    // Extending the even-development rule to the hotel step: every property
    // in the group should already be at 4 houses (or already a hotel) before
    // any single one converts — otherwise the group stops being "even."
    int group_min = min_houses_in_group(board, target->data.property.group);
    return group_min == 4;
}

/* Count squares of a given type owned by one player. Used by the railway and
 * utility rent tables, both of which scale with how many the owner holds. */
int count_owned_by_type(const Square *board, Owner owner, SquareType type){
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type != type) continue;
        if (get_owner(&board[i]) == owner) count++;
    }
    return count;
}

int property_rent(const GameStat *g, const Square *s){
    if (s->type != PROPERTY) return 0;

    const Property *prop = &s->data.property;
    int rent;

    // Table 6 
    if (prop->has_hotel){
        rent = prop->base_rental * 10;
    } else {
        static const int multiplier[] = { 1, 2, 3, 5, 7 };
        int houses = prop->no_of_houses;
        if (houses < 0) houses = 0;
        if (houses > 4) houses = 4;
        rent = prop->base_rental * multiplier[houses];
    }

    // Rule-LK 28: structural damage reduces maximum rent by 25%. 
    if (prop->structural_damage) rent = rent * 75 / 100;

    // Rule-LK 26: condition applies only where buildings exist 
    if (prop->has_hotel || prop->no_of_houses > 0){
        rent = rent * condition_rent_percent(prop->condition) / 100;
    }

    // There are events that affect the rent where owner is the viewer
    rent = (rent * effect_pct(g, s, 0, EFF_RENT, prop->owner) + 50) / 100;

    return rent;
}

int railway_rent(const GameStat *g, const Square *board, Owner owner){
    static const int rent_by_count[] = { 0, 250, 500, 1000, 2000 };

    int owned = count_owned_by_type(board, owner, RAILWAY);
    if (owned < 0) owned = 0;
    if (owned > 4) owned = 4;

    int rent = rent_by_count[owned];

    /* Fuel Shortage / Fuel Crisis / Railway Modernization all target railways
     * as a square type. The square itself is needed for scope matching. */
    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type == RAILWAY){
            rent = (rent * effect_pct(g, &board[i], i, EFF_RENT, owner) + 50) / 100;
            break;
        }
    }

    return rent;
}

int utility_rent(const GameStat *g, const Square *board, Owner owner, int dice){
    int owned = count_owned_by_type(board, owner, UTILITY);

    int rent = 0;
    if (owned >= 2)      rent = dice * 10;
    else if (owned == 1) rent = dice * 4;
    else                 return 0;

    // Power Failure / Electricity Tariff Revision target utilities. 
    for (int i = 0; i < BOARD_SIZE; i++){
        if (board[i].type == UTILITY){
            rent = (rent * effect_pct(g, &board[i], i, EFF_RENT, owner) + 50) / 100;
            break;
        }
    }

    return rent;
}

bool is_mortgaged(const Square *s){
    if (!s->purchasable) return false;
    switch (s->type){
        case PROPERTY: return s->data.property.mortgage_stat;
        case RAILWAY:  return s->data.railway.mortgage_stat;
        case UTILITY:  return s->data.utility.mortgage_stat;
        default:       return false;
    }
}

int get_mortgage_value(const GameStat *g, const Square *s){
    if (!s->purchasable) return 0;

    int value;
    switch (s->type){
        case PROPERTY: value = s->data.property.mortgage_value; break;
        case RAILWAY:  value = s->data.railway.mortgage_value;  break;
        case UTILITY:  value = s->data.utility.mortgage_value;  break;
        default:       return 0;
    }

    // Rules-LK 31/32: booms and declines move mortgage values. 
    return (value * effect_pct(g, s, 0, EFF_MORTGAGE_VALUE, get_owner(s)) + 50) / 100;
}

void set_mortgaged(Square *s, bool state){
    if (!s->purchasable) return;
    switch (s->type){
        case PROPERTY: s->data.property.mortgage_stat = state; break;
        case RAILWAY:  s->data.railway.mortgage_stat  = state; break;
        case UTILITY:  s->data.utility.mortgage_stat  = state; break;
        default: break;
    }
}

// Check if a square carries any buildings 
bool is_developed(const Square *s){
    if (s->type != PROPERTY) return false;
    return s->data.property.has_hotel || s->data.property.no_of_houses > 0;
}

// Rule-LK 3: pledged collateral becomes Loan Locked 
bool is_loan_locked(const Square *s){
    if (!s->purchasable) return false;
    switch (s->type){
        case PROPERTY: return s->data.property.loan_locked;
        case RAILWAY:  return s->data.railway.loan_locked;
        case UTILITY:  return s->data.utility.loan_locked;
        default:       return false;
    }
}

void set_loan_locked(Square *s, bool state){
    if (!s->purchasable) return;
    switch (s->type){
        case PROPERTY: s->data.property.loan_locked = state; break;
        case RAILWAY:  s->data.railway.loan_locked  = state; break;
        case UTILITY:  s->data.utility.loan_locked  = state; break;
        default: break;
    }
}

// A mortgaged property carries no buildings, so mortgaging demolishes them.
void demolish_buildings(Square *s){
    if (s->type != PROPERTY) return;
    s->data.property.no_of_houses = 0;
    s->data.property.has_hotel = false;
}

// Table 3: building condition determines the share of rent collected.
int condition_rent_percent(int condition){
    if (condition >= 90) return 100;
    if (condition >= 75) return  90;
    if (condition >= 50) return  75;
    if (condition >= 25) return  50;
    return 0;                       /* building closed */
}
