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
  
#define PADDLE_LENGTH 30  //Down-wards length of the paddle
#define PADDLE_WIDTH 10  //How thick the paddle is

#define BALLSPEED_X = -0.03
#define BALLSPEED_Y -0.01
  
  
#define MY_UUID { 0xBD, 0x46, 0x1D, 0x5E, 0xCA, 0x85, 0x40, 0xF3, 0xB3, 0x63, 0x15, 0x99, 0xA1, 0x42, 0x29, 0x8F }
