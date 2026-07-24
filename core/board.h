#ifndef BOARD_H
#define BOARD_H

#include "player.h"
#include <stdbool.h>

typedef enum {
	BROWN,
	LIGHT_BLUE,
	PINK,
	ORANGE,
	RED,
	YELLOW,
	GREEN, 
	DARK_BLUE,
} Group;

typedef enum {
	BANK,
	PLAYER_1,
	PLAYER_2,
	PLAYER_3,
	PLAYER_4
} Owner;


typedef struct {
	int abc;
} Start;

typedef struct {
	Group group;
	char *name;
	int purchase_price;
	int mortgage_value;
	int base_rental;
	int house_const_cost;
	int hotel_const_cost;
	Owner *owner;
	bool mortgage_stat;
	bool insurance_stat;
	int num_of_buildings;
} Property;

typedef struct {} Event;

typedef struct {} Tax;

typedef struct {
	char *name;
	Owner *owner;
} Railway;

typedef struct {
	Owner *owner;
} Utility;

typedef struct {
	char *protect_against;
	int premium;
	int compensation;
} InsuranceType;

typedef struct {
	char *name;
	InsuranceType *type;
} Insurance;

typedef struct {} Bank;

// to filter out which type we in
typedef enum {
    START,
    PROPERTY,
    EVENT,
    TAX,
    RAILWAY,
    UTILITY,
    INSURANCE,
    BOC 	// since BANK has used above
} SquareType;

typedef struct {
	SquareType type;
	char *name;

	// since we need one of the following at a time
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

#endif
