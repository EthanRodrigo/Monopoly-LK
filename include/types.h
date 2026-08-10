#ifndef TYPES_H
#define TYPES_H

/*
 * This file contains types which are required by multiple files to avoid circular dependencies
 * */

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

typedef enum {
    OG_BANK,
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4
} Owner;

#endif /* TYPES_H */
