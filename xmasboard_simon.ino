//###########################################################
// Simon-Says 
// Arduino Fio V3 + Accelerometer + NeoPixels = Wobble Board
// Daniel Simser
 
/*
	Portions of the code was derived from Simaon-Says
	Spark Fun Electronics
	Nathan Seidle
    https://github.com/sparkfun/Simon-Says/blob/master/Firmware/Simon_Says/Simon_Says.ino
	
	AND

    Uses the Adafruit NeoPixel library
	https://github.com/adafruit/Adafruit_NeoPixel
*/

//###########################################################
// Game Board Parameters
//###########################################################
#define MODE_MEMORY   0
#define MODE_BALANCE  1

#define ROUNDS_TO_WIN      7 //Number of rounds to succesfully remember before you win. 13 is do-able.
#define ENTRY_TIME_LIMIT   5000 //Amount of time to press a button before game times out. 3000ms = 3 sec

/////////////////////////////////////////////////////////////
//                       red(STRIP_B)(1)
//       green(STRIP_R)(4)                yellow(STRIP_L)(8)
//                       blue(STRIP_F)(4)
/////////////////////////////////////////////////////////////
#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check strip "buttons"
#define CHOICE_RED	    1 // STRIP_F
#define CHOICE_GREEN	2 // STRIP_L
#define CHOICE_BLUE	    4 // STRIP_B
#define CHOICE_YELLOW	8 // STRIP_R

// Game state variables
byte gameMode = MODE_MEMORY; //By default, let's play the memory game

// Game LEVEL
int GAME_LEVEL =  500; 
int GAME_OVER = 0;


//###########################################################
// libraries and externals
//###########################################################
#include <Adafruit_NeoPixel.h>
#include "pixels.h"
#include "accel.h"
#include "functions.h"
#include "games.h"

void setup() {
  // initialize LED pixel (a little motion to catch the eye)
  play_welcome();

  // initialize Accel logic
  initAccel();

  setLEDs(CHOICE_OFF); // Turn off all LEDs
}

void loop() {  
  // Game Options
  if (gameMode == MODE_MEMORY) {
    // Play memory game and handle result
    if (play_memory() == true) {
      play_winner(); // Player won, play winner tones
    } else {
      play_loser(); // Player lost, play loser tones
    }
  } else {
    showStrips();
    play_balance();
  }

}

