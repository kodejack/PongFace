// Globals

#define INVERT_COLORS

#ifndef INVERT_COLORS
#define COLOR_FOREGROUND GColorBlack
#define COLOR_BACKGROUND GColorWhite
#else
#define COLOR_FOREGROUND GColorWhite
#define COLOR_BACKGROUND GColorBlack
#endif
  

#define BALL_SIZE 6
#define VELOCITY 24
#define MIN_Y 5
#define MAX_Y 142
#define MIN_X (BALL_SIZE>>1)
#define MAX_X (144 - (BALL_SIZE>>1))