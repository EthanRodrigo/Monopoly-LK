#include <string.h>
#include "board.h"
#include "types.h"

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
				.no_of_houses = 0,
				.has_hotel = false
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
				.purchase_price = 1500,
				.mortgage_value = 750,
				.base_rental = 120,
				.house_const_cost = 500,
				.hotel_const_cost = 2000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[4] = {
			.type = TAX,
			.name = "Income Tax",
			.purchasable = false,
            .data.tax = {
                .free_allowance = 15000,
                .band_width = 10000,
                .rates = {6, 18, 24, 30}
            }
		},

		[5] = {
			.type = RAILWAY,
			.name = "Colombo Fort Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 8000 
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
				.has_hotel = false
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
				.purchase_price = 2500,
				.mortgage_value = 1250,
				.base_rental = 200,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[9] = {
			.type = PROPERTY,
			.name = "Mount Lavinia",
			.purchasable = true,
			.data.property = {
				.group = LIGHT_BLUE,
				.purchase_price = 2500,
				.mortgage_value = 1250,
				.base_rental = 220,
				.house_const_cost = 750,
				.hotel_const_cost = 3000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
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
				.has_hotel = false
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
				.purchase_price = 3500,
				.mortgage_value = 1750,
				.base_rental = 280,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[14] = {
			.type = PROPERTY,
			.name = "Kottawa",
			.purchasable = true,
			.data.property = {
				.group = PINK,
				.purchase_price = 3500,
				.mortgage_value = 1750,
				.base_rental = 300,
				.house_const_cost = 1000,
				.hotel_const_cost = 4000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[15] = {
			.type = RAILWAY,
			.name = "Kandy Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 8000
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
				.has_hotel = false
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
				.purchase_price = 4500,
				.mortgage_value = 2250,
				.base_rental = 370,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[19] = {
			.type = PROPERTY,
			.name = "Ja-Ela",
			.purchasable = true,
			.data.property = {
				.group = ORANGE,
				.purchase_price = 4500,
				.mortgage_value = 2250,
				.base_rental = 400,
				.house_const_cost = 1250,
				.hotel_const_cost = 5000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
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
				.has_hotel = false
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
				.purchase_price = 5500,
				.mortgage_value = 2750,
				.base_rental = 480,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[24] = {
			.type = PROPERTY,
			.name = "Katugastota",
			.purchasable = true,
			.data.property = {
				.group = RED,
				.purchase_price = 5500,
				.mortgage_value = 2750,
				.base_rental = 500,
				.house_const_cost = 1500,
				.hotel_const_cost = 6000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[25] = {
			.type = RAILWAY,
			.name = "Galle Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 8000
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
				.has_hotel = false
			}
		},

		[27] = {
			.type = PROPERTY,
			.name = "Unawatuna",
			.purchasable = true,
			.data.property = {
				.group = YELLOW,
				.purchase_price = 6500,
				.mortgage_value = 3250,
				.base_rental = 620,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK, 
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
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
				.purchase_price = 6500,
				.mortgage_value = 3250,
				.base_rental = 650,
				.house_const_cost = 2000,
				.hotel_const_cost = 8000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
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
				.has_hotel = false
			}
		},

		[32] = {
			.type = PROPERTY,
			.name = "Nallur",
			.purchasable = true,
			.data.property = {
				.group = GREEN,
				.purchase_price = 8000,
				.mortgage_value = 4000,
				.base_rental = 780,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
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
				.purchase_price = 8000,
				.mortgage_value = 4000,
				.base_rental = 800,
				.house_const_cost = 2500,
				.hotel_const_cost = 10000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		},

		[35] = {
			.type = RAILWAY,
			.name = "Jaffna Railway Station",
			.purchasable = true,
			.data.railway = {
				.owner = OG_BANK,
                .base_rental = 0,
                .purchase_price = 8000
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
				.has_hotel = false
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
				.purchase_price = 10000,
				.mortgage_value = 5000,
				.base_rental = 1200,
				.house_const_cost = 3000,
				.hotel_const_cost = 12000,
				.owner = OG_BANK,
				.mortgage_stat = false,
				.insurance_stat = false,
				.no_of_houses = 0,
				.has_hotel = false
			}
		}
	};

    memcpy(board, temp_board, sizeof(Square) * 40);
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

/* Returns the minimum house count currently built among all developable properties 
 * in `target_group`. A hotel counts as 4 for this comparison.
 * */
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

/* Table 6: residential rent is base rent times a development multiplier.
 * The multipliers are not linear - 3 houses jumps by 2x and the hotel by 3x -
 * so this is a lookup table, not arithmetic.
 *
 * INTERPRETATION: standard Monopoly doubles base rent on an undeveloped
 * monopoly. Table 6 lists 1x for "No Buildings" and says nothing about
 * monopolies, so the literal reading is applied: no doubling.
 *
 * Does NOT check ownership or mortgage status - the caller owns those
 * conditions, so this stays usable for hypothetical rent lookups.
 */
int property_rent(const Square *s){
    if (s->type != PROPERTY) return 0;

    const Property *prop = &s->data.property;

    if (prop->has_hotel) return prop->base_rental * 10;

    /* index = number of houses, 0..4 */
    static const int multiplier[] = { 1, 2, 3, 5, 7 };

    int houses = prop->no_of_houses;
    if (houses < 0) houses = 0;
    if (houses > 4) houses = 4;

    return prop->base_rental * multiplier[houses];
}

/* Table 7: railway rent depends only on how many of the four stations the
 * owner holds - 250 / 500 / 1000 / 2000. Base rental is not used. */
int railway_rent(const Square *board, Owner owner){
    static const int rent_by_count[] = { 0, 250, 500, 1000, 2000 };

    int owned = count_owned_by_type(board, owner, RAILWAY);
    if (owned < 0) owned = 0;
    if (owned > 4) owned = 4;

    return rent_by_count[owned];
}

/* Table 8: utility rent is 4x the dice value for one utility, 10x for both. */
int utility_rent(const Square *board, Owner owner, int dice){
    int owned = count_owned_by_type(board, owner, UTILITY);

    if (owned >= 2) return dice * 10;
    if (owned == 1) return dice * 4;
    return 0;
}
