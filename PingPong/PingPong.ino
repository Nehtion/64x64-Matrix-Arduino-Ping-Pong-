
#include "RGBmatrixPanel.h"
#include "bit_bmp.h"
#include "fonts.h"
#include <string.h>
#include <stdlib.h>
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/FreeSerifBoldItalic9pt7b.h"
#include "Fonts/RobotoMono_Thin7pt7b.h"
#include "Fonts/FreeSans9pt7b.h"

#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define E   A4

//PingPong Variables
#define P1UP    2  //Left Up
#define P1DOWN  3  //Left Down
#define P2UP    4  //Right Up
#define P2DOWN  5  //Right Down
int posP1X = 1;
int posP1Y = 27;
int posP2X = 61;
int posP2Y = 27;
int bWidth = 2;      
int bHeight = 11;
int prePos1 = 31;
int prePos2 = 31;
int cx = 31;  //Circle x
int cy = 31;  //Circle y
float dx;   //Direction x
float dy;   //Direction y
float cs = 1;   //Circle Size
int precx = 31;
int precy = 31;
float ballSpeed = 1.5;
int P1Score = 0;
int P2Score = 0;
int directions[] = {-1,1};
int dirsize = 2;
int diry;
int dirx;
int moves = -1;



RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, false, 64);

void setup() {
  Serial.begin(115200);
  matrix.begin();             //Initialize matrix
  pinMode(P1UP, INPUT_PULLUP);
  pinMode(P1DOWN, INPUT_PULLUP);
  pinMode(P2UP, INPUT_PULLUP);
  pinMode(P2DOWN, INPUT_PULLUP);
  randomSeed(analogRead(A9));
  int randomIndex = random(0, dirsize);
  dx = directions[randomIndex];
  randomIndex = random(0, dirsize);
  dy = directions[randomIndex];
  }

void loop() {
PingPong();
}

void PingPong() {
  if(moves == 0) {
  dirx = (matrix.width()/2)+(dx*10);
  diry = (matrix.height()/2)+(dy*10);
  matrix.drawLine(32,32,dirx,diry,matrix.Color333(7,0,0));
  } 
  if(moves < 32 ) {
    ballSpeed = 0;
  }
  else {
    ballSpeed = 2;
    matrix.drawLine(32,32,dirx,diry,matrix.Color333(0,0,0));
  }
  if(digitalRead(P1UP) == 0 && posP1Y > 0){
    posP1Y--;
  }
  if(digitalRead(P1DOWN) == 0 && posP1Y < matrix.width()-bHeight) {
    posP1Y++;
  }
  if(digitalRead(P2UP) == 0 && posP2Y > 0) {
    posP2Y--;
  }
  if(digitalRead(P2DOWN) == 0 && posP2Y < matrix.width()-bHeight) {
    posP2Y++;
  }

  cx += round(dx * ballSpeed);
  cy += round(dy * ballSpeed);

  if (cy - cs < 0 || cy + cs > matrix.height()) {
    dy = -dy;
  }
  if (cx - cs <= posP1X + bWidth && cy >= posP1Y && cy <= posP1Y + (bHeight/3)) {
    dx = 1.5;
  }

  if (cx - cs <= posP1X + bWidth && cy >= posP1Y + (bHeight/3) && cy <= posP1Y + ((bHeight/3)*2)) {
    dx = 1;
  }
  if (cx - cs <= posP1X + bWidth && cy >= posP1Y + ((bHeight/3)*2) && cy <= posP1Y + bHeight) {
    dx = 0.5;
  }

  if (cx + cs >= posP2X && cy >= posP2Y && cy <= posP2Y + (bHeight/3)) {
    dx = -1.5;
  }
  if (cx + cs >= posP2X && cy >= posP2Y + (bHeight/3) && cy <= posP2Y + ((bHeight/3)*2)) {
    dx = -1;
  }
  if (cx + cs >= posP2X && cy >= posP2Y + ((bHeight/3)*2) && cy <= posP2Y + bHeight) {
    dx = -0.5;
  }
  if (cx - cs < 0) {
    P2Score++;
    printScore();
  }
  if (cx + cs > matrix.width()) {
    P1Score++;
    printScore();
  }
  matrix.fillCircle(precx, precy, cs, matrix.Color333(0,0,0));
  matrix.fillCircle(cx, cy, cs, matrix.Color333(7,7,7));
  matrix.fillRect(posP1X,prePos1,bWidth,bHeight,matrix.Color333(0,0,0));
  matrix.fillRect(posP2X,prePos2,bWidth,bHeight,matrix.Color333(0,0,0));
  matrix.fillRect(posP1X,posP1Y,bWidth,bHeight,matrix.Color333(7,7,7));
  matrix.fillRect(posP2X,posP2Y,bWidth,bHeight,matrix.Color333(7,7,7));
  prePos1 = posP1Y;
  prePos2 = posP2Y;
  precx = cx;
  precy = cy;
  moves++;
  delay(75);
}

void printScore() {
  matrix.setTextSize(1);
  matrix.setTextWrap(true);
  matrix.setCursor(5, 0); 
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print(P1Score);
  matrix.setCursor(50, 0); 
  matrix.print(P2Score);
  delay(3000);
  screen_clear();
  int posP1X = 1;
  posP1Y = 27;
  posP2X = 60;
  posP2Y = 27;
  prePos1 = 31;
  prePos2 = 31;
  cx = 31;  
  cy = 31;   
  precx = 31;
  precy = 31;
  int randomIndex = random(0, dirsize);
  dx = directions[randomIndex];
  randomIndex = random(0, dirsize);
  dy = directions[randomIndex];
  moves = -1;
}

void screen_clear()
{
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
}
