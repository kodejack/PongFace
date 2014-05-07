#ifndef BALL_H_   /* Include guard */
#define BALL_H_

#include "pebble.h"
#include "player.h"

void init_ball();
void draw_ball(GContext* ctx);
int tick_ball();

void check_for_ball_collision (PLAYER player, bool headingRight);

#endif  