/*

   Pong watch for Pebble

 */

// Standard includes
#include "pebble.h"

//
// My understanding of graphics is that there are two approaches but only
// one is public currently:
//
//  * Create a "drawing" at initialisation time with GPath etc (with a
//    PathLayer--which isn't public yet?).
//
//  * Draw at screen update time in a layer update function using
//    `graphics_*` functions.
//
// So, for the moment we have to use the latter--hopefully this will change. :)
//

#define INVERT_COLORS

#ifndef INVERT_COLORS
#define COLOR_FOREGROUND GColorBlack
#define COLOR_BACKGROUND GColorWhite
#else
#define COLOR_FOREGROUND GColorWhite
#define COLOR_BACKGROUND GColorBlack
#endif
  
// Globals
#define BALL_SIZE 6
#define MIN_Y 5
#define MAX_Y 142
#define MIN_X (BALL_SIZE>>1)
#define MAX_X (144 - (BALL_SIZE>>1))

int pos_x, pos_y, delta_x, delta_y, old_x, old_y;
bool initialWipeFlag;

// App-specific data
Window *window; // All apps must have at least one window

Layer *boardLayer; // The board/grid
Layer *playersLayer; // The player markers
TextLayer *timeLayer; // The clock/"scoreboard"


// This is called whenever the `boardLayer` layer needs to be redrawn.
void boardLayer_update_callback(Layer *me, GContext* ctx) {

  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

  // Draw playing board grid
  // TODO: Change this to allow for a nicely centred circle in each square.

  GRect rect = layer_get_bounds(me);
  int centerX = rect.size.w>>1;
  int centerY = rect.size.h>>1;

  // Vertical lines  dotted line
  for (int y = MIN_Y; y <= MAX_Y-5; y+=10)
  {
    graphics_draw_line(ctx, GPoint(centerX, y), GPoint(centerX, y+5));
  }
  
  // Horizontal lines
  graphics_draw_line(ctx, GPoint((MIN_X), MIN_Y), GPoint((MAX_X), MIN_Y));
  graphics_draw_line(ctx, GPoint((MIN_X), MAX_Y), GPoint((MAX_X), MAX_Y));
}


void graphics_draw_line_wide(GContext* ctx, GPoint p0, GPoint p1) {
  // TODO: Replace this when lines can have width
  // NOTE: This fails for horizontal lines as it only "widens" along the x axis.

  for (int xOffset = -1; xOffset < 2; xOffset++) {
    graphics_draw_line(ctx, GPoint(p0.x+xOffset, p0.y), GPoint(p1.x+xOffset, p1.y));
  }

}


void drawCirclePlayer(GContext* ctx, GPoint center) {

  graphics_context_set_fill_color(ctx, COLOR_FOREGROUND);
  graphics_fill_circle(ctx, center, 3);
  graphics_context_set_fill_color(ctx, COLOR_FOREGROUND);
  graphics_fill_circle(ctx, center, 1);
}


void drawCrossPlayer(GContext* ctx, GPoint center) {

  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

  graphics_draw_line_wide(ctx, GPoint(center.x-12, center.y-12), GPoint(center.x+12, center.y+12));
  graphics_draw_line_wide(ctx, GPoint(center.x-12, center.y+12), GPoint(center.x+12, center.y-12));

}


GPoint getCellCenter(unsigned int cellOffset) {
  // Returns the centre coordinates of a cell location on the playing board.
  const unsigned short NUM_COLUMNS = 3;
  const unsigned short OFFSET_X = 37;
  const unsigned short OFFSET_Y = 37;
  const unsigned short CELL_WIDTH = 35;
  const unsigned short CELL_HEIGHT = 35;

  return GPoint( ((cellOffset % NUM_COLUMNS) * CELL_WIDTH) + OFFSET_X,
		 ((cellOffset / NUM_COLUMNS) * CELL_HEIGHT) + OFFSET_Y);
}


typedef struct {
  const unsigned short *SEQUENCE;
  unsigned short sequenceOffset;
} DemoGameState;


void retrieveCurrentGameState(DemoGameState *gameState) {

  time_t now = time(NULL);
  struct tm *currentTime = localtime(&now);
  

  
  
/*
  // The current location in the move sequence is synced to the current time
  // so that games end when the clock digits change.
  gameState->sequenceOffset = (currentTime->tm_sec + 8) % 10;

*/
}

void drawGameState(GContext* ctx, const DemoGameState gameState) {

    if (initialWipeFlag)
    {
        // Erase screen on first run
        
        graphics_context_set_fill_color(ctx, GColorBlack);
        GRect rect = GRect(0,0,144,168);
        graphics_fill_rect(ctx, rect, 0, GCornerNone);
        initialWipeFlag = false;
    }
    
    // Wipe old circle
    
    GPoint point = GPoint(old_x, old_y);
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, point, BALL_SIZE);
    
    // Draw new circle
    
    point = GPoint(pos_x, pos_y);
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, point, BALL_SIZE);

}

void pulse_on_collision(bool pulse)
{
  if(pulse)
    vibes_short_pulse();
}

void handle_timer(GContext* ctx, uint32_t cookie)
{
    bool usePulse = false;
  
    old_x = pos_x;
    old_y = pos_y;
    
    pos_x = pos_x + delta_x;
    pos_y = pos_y + delta_y;
    
    if (pos_x > MAX_X)
    {
        pos_x = MAX_X -BALL_SIZE;
        delta_x = -BALL_SIZE;
        pulse_on_collision(usePulse);
    }
    
    if (pos_x < MIN_X)
    {
        pos_x = MIN_X + BALL_SIZE;
        delta_x = BALL_SIZE;
        pulse_on_collision(usePulse);
    }
    
    if (pos_y > MAX_Y)
    {
        pos_y = MAX_Y - BALL_SIZE;
        delta_y = -BALL_SIZE;
        pulse_on_collision(usePulse);
    }
    
    if (pos_y < MIN_Y)
    {
        pos_y = MIN_Y + BALL_SIZE;
        delta_y = BALL_SIZE;
        pulse_on_collision(usePulse);
    }
    
    // Reset timer
    
    //timerHandle = app_timer_send_event(ctx, time_duration, 1);
    
   // Tell layer to redraw

    layer_mark_dirty(playersLayer);
}




/*
void handle_tick(AppContextRef ctxt, PebbleTickEvent *event)
{
    pos_x = pos_x + delta_x;
    pos_y = pos_y + delta_y;
    
    if (pos_x > 140)
    {
        pos_x = 132;
        delta_x = -8;
    }
    
    if (pos_x < 4)
    {
        pos_x = 12;
        delta_x = 8;
    }
    
    if (pos_y > 162)
    {
        pos_y = 154;
        delta_y = -8;
    }
    
    if (pos_y < 4)
    {
        pos_y = 12;
        delta_y = 8;
    }
    
    Layer *root = window_get_root_layer(window);
    layer_mark_dirty(root);
}
*/

// This is called whenever the `playersLayer` layer needs to be redrawn.
// TODO: Can we get this called without the layer being cleared?
void playersLayer_update_callback(Layer *me, GContext* ctx) {

  DemoGameState gameState;

#ifdef INVERT_COLORS
  graphics_context_set_compositing_mode(ctx, GCompOpAssignInverted);
#endif

  retrieveCurrentGameState(&gameState);

  drawGameState(ctx, gameState);
  
  handle_timer(ctx, 0);
  
  

}


void update_time_text() {
  static char timeText[] = "00:00";

  char *timeFormat;

  time_t now = time(NULL);
  const struct tm *currentTime = localtime(&now);

  if (clock_is_24h_style()) {
    timeFormat = "%R";
  } else {
    timeFormat = "%I:%M";
  }

  strftime(timeText, sizeof(timeText), timeFormat, currentTime);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (timeText[0] == '0')) {
    memmove(timeText, &timeText[1], sizeof(timeText) - 1);
  }

  text_layer_set_text(timeLayer, timeText);

  // Causes a redraw of the layer (via the
  // associated layer update callback)
  // Note: This will cause the entire layer to be cleared first so it needs
  //       to be redrawn in its entirety--if you want to preserve drawn
  //       content you must have it on a different layer. e.g. board vs player layers.
  layer_mark_dirty(playersLayer);
}


// Called once per second
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time_text();
}


// Handle the start-up of the app
void init(void) {

  // Create our app's base window
  window = window_create();
  window_stack_push(window, true);

  window_set_background_color(window, COLOR_BACKGROUND);

  // Init the layer that shows the board
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  boardLayer = layer_create(bounds); // Associate with layer object and set dimensions
  layer_set_update_proc(boardLayer, boardLayer_update_callback); // Set the drawing callback function for the layer.
  layer_add_child(window_layer, boardLayer); // Add the child to the app's base window

  // Init the layer that shows the player marks
  playersLayer = layer_create(bounds);
  layer_set_update_proc(playersLayer, playersLayer_update_callback);
  layer_add_child(window_layer, playersLayer);
  

  // Init the text layer used to show the time
  timeLayer = text_layer_create(GRect(0, 0, 144 /* width */, 42 /* height */));
  text_layer_set_text_alignment(timeLayer, GTextAlignmentCenter);
  text_layer_set_text_color(timeLayer, COLOR_FOREGROUND);
  text_layer_set_background_color(timeLayer, GColorClear);
  text_layer_set_font(timeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(timeLayer));

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);

  update_time_text();
  
  srand(time(NULL));
    
  pos_x = 68 + rand() % 8;
  pos_y = 80 + rand() % 8;
    
  delta_x = BALL_SIZE;
  delta_y = BALL_SIZE;
    
  old_x = 0;
  old_y = 0;
    
}

// Clean up
void deinit(void) {
  window_destroy(window);
  layer_destroy(boardLayer);
  layer_destroy(playersLayer);
  text_layer_destroy(timeLayer);
}

// The main event/run loop for our app
int main(void) {
  init();
  app_event_loop();
  deinit();
}
