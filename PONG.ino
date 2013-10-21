#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

// Pins for the TFT
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _rst 9
#define _dc 8

// TFT object
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

// Global variables
int ball; // Ball radius

int WIDTH; // Screen width
int HEIGHT; // Screen height

double x; // x position of the ball
double y; // y position of the ball

double ox; // Old x position of the ball
double oy; // Old y position of the ball

double dx; // Delta x for the ball
double dy; // Delta y for the ball

int len; // Length of the paddles

int p1; // Player 1 position
int p2; // Player 2 position

int op1; // Old player 1 position
int op2; // Old player 2 position

int p1Score; // Player 1 score
int p2Score; // Player 2 score

int op1Score; // Old player 1 score
int op2Score; // Old player 2 score

// Main setup function
void setup() {
  pinMode(A0, INPUT); // Paddle 1
  pinMode(A3, INPUT); // Mode pot
  pinMode(A5, INPUT); // Paddle 2
  tft.begin(); // Initialise the display
  tft.setRotation(map(analogRead(A3), 0, 1023, 0, 3)); // Rotate the screen depending on the value of A3
  tft.fillScreen(ILI9340_BLACK); // Clear the screen
  
  // Initialise global variables
  WIDTH = tft.width();
  HEIGHT = tft.height();
  
  ball = 3;
  
  x = WIDTH/2;
  y = (HEIGHT + 18)/2;
  
  ox = !x;
  oy = !y;
  
  dx = 1;
  dy = 1;
  
  len = 50;
  
  p1 = HEIGHT/2;
  p2 = HEIGHT/2;
  
  op1 = !p1;
  op2 = !p2;
  
  p1Score = 48;
  p2Score = 48;
  
  op1Score = !p1Score;
  op2Score = !p2Score;
  
}

// Main loop function
void loop() {
  
  // Draw the board
  tft.drawFastVLine(WIDTH/2, 0, HEIGHT, ILI9340_GREEN);
  tft.drawFastHLine(0, 18, WIDTH, ILI9340_GREEN);
  
  // Draw the score for player 1
  if (p1Score != op1Score) { // If the score has changed...
    tft.drawRect(WIDTH/2 - 10, 5, 5, 8, ILI9340_BLACK); // Then erase the old score...
    op1Score = p1Score; // Set the old score to the current score...
    tft.drawChar(WIDTH/2 - 10, 5, (char)p1Score, ILI9340_GREEN, ILI9340_BLACK, 1); // And draw the new score
  }
  
  // Draw The score for player 2
  if (p2Score != op2Score) { // If the score has changed...
    tft.drawRect(WIDTH/2 + 5, 5, 5, 8, ILI9340_BLACK); // Then erase the old score...
    op2Score = p2Score; // Set the old score to the current score...
    tft.drawChar(WIDTH/2 + 5, 5, (char)p2Score, ILI9340_GREEN, ILI9340_BLACK, 1); // And draw the new score
  }
  
  // Draw the ball
  if (x != ox || y != oy) { tft.fillCircle(ox, oy, ball, ILI9340_BLACK); ox = x; oy = y; } // Erase the old ball if the position changed
  tft.fillCircle(x, y, ball, ILI9340_GREEN); // Draw the new ball
  
  // Draw paddle 1
  if (p1 != op1) { tft.drawFastVLine(10, op1 - len/2, len, ILI9340_BLACK); op1 = p1; } // Erase the old paddle if the position changed
  tft.drawFastVLine(10, p1 - len/2, len, ILI9340_GREEN); // Draw the new paddle
  
  // Draw paddle 2
  if (p2 != op2) { tft.drawFastVLine(WIDTH - 10, op2 - len/2, len, ILI9340_BLACK); op2 = p2; } // Erase the old paddle if the position changed
  tft.drawFastVLine(WIDTH - 10, p2 - len/2, len, ILI9340_GREEN); // Draw the new paddle
  
  // Move the paddles
  p1 = map(analogRead(A0), 0, 1023, HEIGHT - len/2, len/2 + 18);
  p2 = map(analogRead(A5), 0, 1023, HEIGHT - len/2, len/2 + 18);
  
  // Update the ball
  x += dx;
  y += dy;
  
  // Turn off the beep
  noTone(3);
  
  // Check to see if the ball hit paddle 1
  if (x <= 10 + ball) {
    if (y >= p1 - len/2 && y <= p1 + len/2) {
      dx = -dx;
      tone(3, 523);
    } else {
      p2Score++;
      x = WIDTH/2;
      y = HEIGHT/2;
      tone(3, 262);
    }
  }
  
  // Check to see if the ball hit paddle 2
  if (x >= WIDTH - (10 + ball)) {
    if (y >= p2 - len/2 && y <= p2 + len/2) {
      dx = -dx;
      tone(3, 523);
    } else {
      p1Score++;
      x = WIDTH/2;
      y = (HEIGHT + 18)/2;
      tone(3, 262);
    }
  }
  
  // Bounce ball off ceiling and floor
  if (y >= HEIGHT - ball || y <= 0 + (18 + ball)) {
    dy = -dy;
    tone(3, 523);
  }
  
  // Restart game when someone gets 7 points
  if (p1Score == 48 + 7 || p2Score == 48 + 7) {
    setup();
  }
  
}
