#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h> 
#include "events.h" 
#include "finance.h" 
#include "types.h"

#define BOARD_SIZE 40
#define JAIL_SQUARE 10
#define BAIL_AMOUNT 300
#define MAX_JAIL_TURNS 3
#define START_SQUARE 0

typedef struct {
    int award;

    void (*pass_start)(Player *p);
} Start;

typedef struct {
    Owner owner;
    int purchase_price;
    int base_rental;
    int mortgage_value;
    bool mortgage_stat;
    bool loan_locked;
} Utility;

typedef struct {
    Group group;
    int purchase_price;
    int base_rental;
    int mortgage_value;
    int house_const_cost;
    int hotel_const_cost;
    Owner owner;
    bool mortgage_stat;
    bool insurance_stat;
    int no_of_houses;
    bool has_hotel;
    bool loan_locked;
} Property;

typedef struct {
    Owner owner;
    int purchase_price;
    int base_rental;
    int mortgage_value;
    bool mortgage_stat;
    bool loan_locked;
} Railway;

typedef enum {
    JAIL_VISITING,
    FREE_PARKING,
    GO_TO_JAIL
} SpecialKind;

typedef struct {
    SpecialKind kind;
} Special;

typedef enum {
    START,
    PROPERTY,
    EVENT,
    TAX,
    RAILWAY,
    UTILITY,
    INSURANCE,
    BANK,
    SPECIAL
} SquareType;

typedef struct Square{
    SquareType type;
    char *name;
    bool purchasable;

    union {
        Start start;
        Property property;
        Event event;
        Tax tax;
        Railway railway;
        Utility utility;
        Insurance insurance;
        Bank bank;
        Special special;
    } data;
} Square;

void draw_board(Square* board);
int resolve_out_of_bounds(int curr_pos, int offset);
int min_houses_in_group(const Square *board, Group target_group);
bool can_build_house(const Square *board, const Square *target, Owner owner);
bool can_build_hotel(const Square *board, const Square *target, Owner owner);
int property_rent(const Square *s);
int railway_rent(const Square *board, Owner owner);
int utility_rent(const Square *board, Owner owner, int dice);
int count_owned_by_type(const Square *board, Owner owner, SquareType type);

bool is_mortgaged(const Square *s);
int  get_mortgage_value(const Square *s);
void set_mortgaged(Square *s, bool state);
bool is_developed(const Square *s);

void demolish_buildings(Square *s);

/* Rule-LK 3: pledged collateral becomes Loan Locked - distinct from
 * mortgaged. Loan locked squares cannot be sold, traded, auctioned or further
 * mortgaged, but still earn rent and may still be developed. Rule-LK 1 makes
 * properties, railways and utilities all eligible collateral. */
bool is_loan_locked(const Square *s);
void set_loan_locked(Square *s, bool state);

// Getters and Setters are used to avoid long conditional statements 
// getters 
Owner get_owner(const Square *s);
int get_purchase_price(const Square *s);
int get_rent(const Square *s);

// setters
void set_owner(Square *s, Owner new_owner);

#endif /* BOARD_H */
