/*

   Pong watch for Pebble

 */

// Standard includes
#include "pebble.h"
#include "ball.h"
#include "court.h"
#include "player.h"


#include "constants.h"

  /*
  PBL_APP_INFO(MY_UUID,
             "Pong Watch", "GTank",
             1, 0,
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);
  */
bool initialWipeFlag;

// App-specific data
Window *window; // All apps must have at least one window
Layer *courtLayer; // The courtLayer/background
Layer *playersLayer; // The player markers
TextLayer *timeLayer; // The clock/"scoreboard"
TextLayer *scoreLayer; // The clock/"scoreboard"

int player1Score;
int player2Score;



// This is called whenever the `courtLayer` layer needs to be redrawn.
void courtLayer_update_callback(Layer *me, GContext* ctx) 
{
  draw_court (me, ctx);
}

void update_score_text(int player1, int player2)
{
  static char scoreText[] = "00 00";
  snprintf(scoreText, sizeof(scoreText), "%02d %02d", player1,player2);
  text_layer_set_text(scoreLayer, scoreText);

}

// This is called whenever the `playersLayer` layer needs to be redrawn.
// TODO: Can we get this called without the layer being cleared?
void playersLayer_update_callback(Layer *me, GContext* ctx) {

#ifdef INVERT_COLORS
  graphics_context_set_compositing_mode(ctx, GCompOpAssignInverted);
#endif

  if (initialWipeFlag)
  {
      // Erase screen on first run
      graphics_context_set_fill_color(ctx, GColorBlack);
      GRect rect = layer_get_bounds(window_get_root_layer(window));// GRect(0,0,144,168);
      graphics_fill_rect(ctx, rect, 0, GCornerNone);
      initialWipeFlag = false;
  }
  
  
  draw_ball(ctx);
  int point = tick_ball();
  
  if(point == 1)
  {
     player1Score ++;
  }
  else if (point == 2)
  {
    player2Score++;
  }

  if(player1Score > 10 || player2Score > 10)
  {
    player1Score = 0;
    player2Score = 0;
    psleep(2000);
  }
  update_score_text(player1Score, player2Score);
  
  draw_player1(ctx);
  tick_player1();
  
  draw_player2(ctx);
  tick_player2();
  
  
  check_for_ball_collision (player2, true);
  check_for_ball_collision (player1, false);
  
  /*
  draw_player(ctx, player2);
  tick_player(player2);
  */
  
  // Reset timer
  //timerHandle = app_timer_send_event(ctx, 100, 1);
  // Tell layer to redraw
  layer_mark_dirty(playersLayer);
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
  //       content you must have it on a different layer. e.g. court vs player layers.
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

  // Init the layer that shows the court
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  courtLayer = layer_create(bounds); // Associate with layer object and set dimensions
  layer_set_update_proc(courtLayer, courtLayer_update_callback); // Set the drawing callback function for the layer.
  layer_add_child(window_layer, courtLayer); // Add the child to the app's base window

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
  
  scoreLayer = text_layer_create(GRect(0, 110, 144 /* width */, 42 /* height */));
  text_layer_set_text_alignment(scoreLayer, GTextAlignmentCenter);
  text_layer_set_text_color(scoreLayer, COLOR_FOREGROUND);
  text_layer_set_background_color(scoreLayer, GColorClear);
  text_layer_set_font(scoreLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(scoreLayer));
  
  player1Score = 0;
  player2Score = 0;
  update_score_text(player1Score, player2Score);
  
  init_ball();
  init_players();

}

// Clean up
void deinit(void) {
  window_destroy(window);
  layer_destroy(courtLayer);
  layer_destroy(playersLayer);
  text_layer_destroy(timeLayer);
  text_layer_destroy(scoreLayer);
  
}

// The main event/run loop for our app
int main(void) {
  init();
  app_event_loop();
  deinit();
}
