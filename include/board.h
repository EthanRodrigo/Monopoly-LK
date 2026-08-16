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

#define MAX_DEPRECIATION      30   // Rule-LK 16 
#define DEPRECIATION_START    50   // Rule-LK 16: rounds before it begins 
#define CONDITION_DECAY        2   // Rule-LK 25: percent per round
#define MAX_NEGLECT_ROUNDS    20   // Rule-LK 28 

/* game.h includes this header file, so to avoid circular dependencies pitfall 
 * forward declarion  is used */
typedef struct GameStat GameStat;

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
    Owner owner;
    int purchase_price;
    int base_rental;
    int mortgage_value;
    int house_const_cost;
    int hotel_const_cost;
    bool mortgage_stat;
    bool insurance_stat;
    int no_of_houses;
    bool has_hotel;
    bool loan_locked;
    int  age;                       // complete rounds since purchase/renovation 
    int  depreciation;              // percent, 0-30 (Rule-LK 16 cap) 

    /* Rule-LK 25-28: building condition.
     * one condition per property rather than per building.
     * Rule 9 forces even development, so every building on a property is the
     * same age, and the spec never distinguishes them individually. */
    int  condition;                 // percent, starts at 100 
    int  rounds_since_maintenance;
    bool structural_damage;         // Rule-LK 28: If maintenance is ignored damage happens 
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

int property_rent(const GameStat *g, const Square *s);
int railway_rent(const GameStat *g, const Square *board, Owner owner);
int utility_rent(const GameStat *g, const Square *board, Owner owner, int dice);
int count_owned_by_type(const Square *board, Owner owner, SquareType type);

void demolish_buildings(Square *s);

int condition_rent_percent(int condition);

// Getters and Setters are used to avoid long conditional statements 
// getters 
Owner get_owner(const Square *s);
int get_purchase_price(const GameStat *g, const Square *s);
int get_rent(const GameStat *g, const Square *s);
int  get_mortgage_value(const GameStat *g, const Square *s);
bool is_mortgaged(const Square *s);
bool is_developed(const Square *s);
bool is_loan_locked(const Square *s);

// setters
void set_owner(Square *s, Owner new_owner);
void set_mortgaged(Square *s, bool state);
void set_loan_locked(Square *s, bool state);

#endif /* BOARD_H */
