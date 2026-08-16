#ifndef EVENTS_H
#define EVENTS_H

/* Rule-LK 12: possible inflation rates, drawn every ten rounds.
 * Negative values are deflation. */
#define INFLATION_RATE_COUNT 6

typedef struct {
//    EventKind kind;
    int expires_round;
    int magnitude; // the percentage
    int target_group;
} Event;

/* Forward declarations - events.c operates on the board and on game state,
 * but events.h is included by board.h, so the real types are not visible
 * here yet. */
typedef struct Square Square;

/* Rule-LK 12-14: inflation. Every ten rounds a rate is generated and applied
 * to every value the spec lists in Rule-LK 13. The effect compounds, so the
 * board's stored values are rewritten in place rather than tracked as a
 * temporary modifier. */
int  generate_inflation_rate(void);
void apply_inflation(Square *board, int rate);
int  inflate_value(int value, int rate);

#endif /* EVENTS_H */
