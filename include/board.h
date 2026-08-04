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
} Utility;

typedef struct {
    int award;

    void (*pass_start)(Player *p);
} Start;

typedef struct {
    Group group;
    int purchase_price;
    int mortgage_value;
    int base_rental;
    int house_const_cost;
    int hotel_const_cost;
    Owner owner;
    bool mortgage_stat;
    bool insurance_stat;
    int num_of_buildings;
} Property;

typedef struct {
    Owner owner;
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

typedef struct {
    SquareType type;
    char *name;

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

#endif /* BOARD_H */
