// Standard includes
#include "player.h"
#include "constants.h"
  
int width;
int height;
  
void init_players() 
{
  width = PADDLE_WIDTH;
  height = PADDLE_LENGTH;
  
  player1.w = width;
  player1.h = height;
  player1.x = MIN_X;
  player1.y = (MAX_Y>>1) - (player1.h>>1);;
  player1.dy = -VELOCITY;
  player1.oy= 0;
  

  player2.w = width;
  player2.h = height;
  player2.x = MAX_X - player2.w;
  player2.y = (MAX_Y>>1) - (player2.h>>1);
  player2.dy = VELOCITY;
  player2.oy= 0;
}

void draw_player1(GContext* ctx)
{
    // Wipe old Rect
    GPoint point = GPoint(player1.x, player1.oy);
    graphics_context_set_fill_color(ctx, COLOR_BACKGROUND);
    GRect rect = GRect (point.x, point.y, width, height);
    graphics_fill_rect(ctx, rect, 1, 0);
    
    // Draw new Rect
    point = GPoint(player1.x, player1.y);
    graphics_context_set_fill_color(ctx, COLOR_FOREGROUND);
    rect = GRect (point.x, point.y, width, height);
    graphics_fill_rect(ctx, rect, 1, 0);
}

void draw_player2(GContext* ctx)
{
    // Wipe old Rect
    GPoint point = GPoint(player2.x, player2.oy);
    graphics_context_set_fill_color(ctx, COLOR_BACKGROUND);
    GRect rect = GRect (point.x, point.y, width, height);
    graphics_fill_rect(ctx, rect, 1, 0);
    
    // Draw new Rect
    point = GPoint(player2.x, player2.y);
    graphics_context_set_fill_color(ctx, COLOR_FOREGROUND);
    rect = GRect (point.x, point.y, width, height);
    graphics_fill_rect(ctx, rect, 1, 0);
}



void tick_player1()
{
  player1.oy = player1.y;
  player1.y = player1.y + player1.dy;

  if (player1.y > (MAX_Y - player1.h))
  {
     player1.y = MAX_Y - player1.h;
     player1.dy = -VELOCITY;
  }
    
  if (player1.y < MIN_Y)
  {
      player1.y = MIN_Y;
      player1.dy = VELOCITY;
  }
} 

void tick_player2()
{
  player2.oy = player2.y;
  player2.y = player2.y + player2.dy;

  if (player2.y > (MAX_Y - player2.h))
  {
     player2.y = MAX_Y - player2.h;
     player2.dy = -VELOCITY;
  }
    
  if (player2.y < MIN_Y)
  {
      player2.y = MIN_Y;
      player2.dy = VELOCITY;
  }
} 


