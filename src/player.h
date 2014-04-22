#ifndef PLAYER_H_   /* Include guard */
#define PLAYER_H_
#include "pebble.h"

typedef struct player 
{
    int x;
    int y;
    int w;
    int h;
    int dy; // delta
    int oy; // old y 
    bool loose;
    char name[32];
} PLAYER;

PLAYER player1, player2;

void init_players();
void draw_player1(GContext* ctx);
void tick_player1();

void draw_player2(GContext* ctx);
void tick_player2();

#endif  