#include "court.h"
#include "constants.h"

  void draw_court(Layer *me, GContext* ctx)
{
    graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

    // Draw court lines
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