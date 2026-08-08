#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h> 
#include "events.h" 
#include "finance.h" 
#include "players.h"

#define BOARD_SIZE 40

typedef enum {
    BROWN,
    LIGHT_BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE
} Group;

typedef struct {
    Owner owner;
    int purchase_price;
    int base_rental;
} Utility;

typedef struct {
    int award;

    void (*pass_start)(Player *p);
} Start;

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
    int num_of_buildings;
} Property;

typedef struct {
    Owner owner;
    int purchase_price;
    int base_rental;
} Railway;

typedef enum {
    START,
    PROPERTY,
    EVENT,
    TAX,
    RAILWAY,
    UTILITY,
    INSURANCE,
    BANK
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
    } data;
} Square;

void draw_board(Square* board);
int resolve_out_of_bounds(int curr_pos, int offset);

// Getters and Setters are used to avoid long conditional statements 
// getters 
Owner get_owner(const Square *s);
int get_purchase_price(const Square *s);
int get_rent(const Square *s);

// setters
void set_owner(Square *s, Owner new_owner);

#endif /* BOARD_H */
