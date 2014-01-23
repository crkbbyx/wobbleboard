//####################################################
// SIMON game and Balance board related games

byte gameBoard[32]; //Contains the combination of buttons as we advance
byte gameRound = 0; //Counts the number of succesful rounds the player has made it through

void play_balance() {
    unsigned long currentMillis = millis();

    gatherAccelData();

    if(accelG[2] < 1.0) {

      PIXEL_LEVEL_F = (accelG[0]*100);
      
      int ACTIVE_PIXEL = map(PIXEL_LEVEL_F, 0, 25, 0, 9);
  
      PIXEL_LEVEL_L = (accelG[1]*100);
      int ACTIVE_PIXEL_L = map(PIXEL_LEVEL_L, 0, 30, 0, 10);

      if(ACTIVE_PIXEL < 0) {
        ACTIVE_PIXEL = ACTIVE_PIXEL*-1;
        STRIP_F.setPixelColor(ACTIVE_PIXEL-1,STRIP_F.Color(255,0,0));  
        STRIP_B.setPixelColor(NUM_PIXELS-ACTIVE_PIXEL,STRIP_B.Color(0,255,0));  
      } else {
        STRIP_F.setPixelColor(NUM_PIXELS-ACTIVE_PIXEL,STRIP_F.Color(0,255,0));  
        STRIP_B.setPixelColor(ACTIVE_PIXEL-2,STRIP_B.Color(255,0,0));  
      }
      STRIP_F.show(); 
      STRIP_B.show(); 
  
      if(ACTIVE_PIXEL_L < 0) {
        //Serial.println("L");
        ACTIVE_PIXEL_L = ACTIVE_PIXEL_L*-1;
        STRIP_L.setPixelColor(ACTIVE_PIXEL_L-2,STRIP_L.Color(255,0,0));  
        STRIP_R.setPixelColor(NUM_PIXELS-ACTIVE_PIXEL_L,STRIP_R.Color(0,255,0));  
      } else {
        //Serial.println("R");
        STRIP_L.setPixelColor(NUM_PIXELS-ACTIVE_PIXEL_L,STRIP_L.Color(0,255,0));  
        STRIP_R.setPixelColor(ACTIVE_PIXEL_L-1,STRIP_R.Color(255,0,0));  
      }
      STRIP_L.show(); 
      STRIP_R.show(); 
      checkPlayerLose(currentMillis,ACTIVE_PIXEL, ACTIVE_PIXEL_L);
    }

}

// Adds a new random button to the game sequence, by sampling the timer
void add_to_moves(void)
{
  byte newButton = random(0, 4); //min (included), max (exluded)

  // We have to convert this number, 0 to 3, to CHOICEs
  if(newButton == 0) newButton = CHOICE_RED;
  else if(newButton == 1) newButton = CHOICE_GREEN;
  else if(newButton == 2) newButton = CHOICE_BLUE;
  else if(newButton == 3) newButton = CHOICE_YELLOW;
  gameBoard[gameRound++] = newButton; // Add this new button to the game array
}

void play_winner() {
  initStrips();
  for(int l=0; l<5; l++) {
	  for(int i=0; i < NUM_PIXELS; i++) {
		  STRIP_F.setPixelColor(i, STRIP_F.Color(75, 0, 130)); // blink  
		  STRIP_B.setPixelColor(i, STRIP_B.Color(75, 0, 130)); // blink  
	  	  STRIP_L.setPixelColor(i, STRIP_L.Color(75, 0, 130)); // blink  
	  	  STRIP_R.setPixelColor(i, STRIP_R.Color(75, 0, 130)); // blink  
	  	  showStrips();
	  }
	  delay(200);
	  for(int i=0; i < NUM_PIXELS; i++) {
		  STRIP_F.setPixelColor(i, STRIP_F.Color(127, 255, 0)); // blink  
		  STRIP_B.setPixelColor(i, STRIP_B.Color(127, 255, 0)); // blink  
		  STRIP_L.setPixelColor(i, STRIP_L.Color(127, 255, 0)); // blink  
		  STRIP_R.setPixelColor(i, STRIP_R.Color(127, 255, 0)); // blink  
		  showStrips();
	  }
	  delay(200);
  }
  delay(500);
  initStrips();
}

void play_loser() {
  for(int r=0; r<5; r++) {
	  for(int i=0; i < NUM_PIXELS; i++) {
		  STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // blink  
		  STRIP_B.setPixelColor(i, STRIP_B.Color(0, 0, 0)); // blink  
		  STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // blink  
		  STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // blink  
		  showStrips();
	  }
	  delay(200);
	  for(int i=0; i < NUM_PIXELS; i++) {
		  STRIP_F.setPixelColor(i, STRIP_F.Color(255, 0, 0)); // blink  
		  STRIP_B.setPixelColor(i, STRIP_B.Color(255, 0, 0)); // blink  
		  STRIP_L.setPixelColor(i, STRIP_L.Color(255, 0, 0)); // blink  
		  STRIP_R.setPixelColor(i, STRIP_R.Color(255, 0, 0)); // blink  
		  showStrips();
	  }
	  delay(200);
  }
}

void play_between() {
          for(int i=0; i < NUM_PIXELS; i++) {
              STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // blink  
              STRIP_B.setPixelColor(i, STRIP_B.Color(0, 0, 0)); // blink  
              STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // blink  
              STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // blink  
              showStrips();
          }
          delay(550);
}

void setLEDs(byte leds) {
  /////////////////////////////////////////////////////////////
  //                       red(STRIP_B)(1)
  //       green(STRIP_R)(4)                yellow(STRIP_L)(8)
  //                       blue(STRIP_F)(4)
  /////////////////////////////////////////////////////////////
  int i = 0;

  if ((leds & CHOICE_OFF) != 0) {
    for(i=0; i < 10; i++) {
      STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // OFF 
      STRIP_B.setPixelColor(i, STRIP_B.Color(0, 0, 0)); // OFF 
      STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // OFF 
      STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // OFF 
      showStrips();  
      delay(500);
    }
  }

  if ((leds & CHOICE_RED) != 0) {
    for(i=0; i < 10; i++) {
      STRIP_B.setPixelColor(i, STRIP_B.Color(255, 0, 0)); // red  
      STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // OFF 
      STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // OFF 
      STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // OFF 
      showStrips();  
    }
  } else {
    for(i=0; i < 10; i++) {
      STRIP_B.setPixelColor(i, STRIP_B.Color(0, 0, 0)); // OFF 
      showStrips();
    }  
  }

  if ((leds & CHOICE_BLUE) != 0) {
    for(i=0; i < 10; i++) {
      STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 255)); // blue  
      STRIP_B.setPixelColor(i, STRIP_B.Color(0, 0, 0)); // OFF 
      STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // OFF 
      STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // OFF 
      showStrips();  
    }
  } else {
    for(i=0; i < 10; i++) {
      STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // OFF 
      showStrips();
    }  
  }

  if ((leds & CHOICE_GREEN) != 0) {
    for(i=0; i < 10; i++) {
      STRIP_R.setPixelColor(i, STRIP_R.Color(0, 100, 0)); // green  
      STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // OFF 
      STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // OFF 
      STRIP_B.setPixelColor(i, STRIP_B.Color(0, 0, 0)); // OFF 
      showStrips();  
    }
  } else {
    for(i=0; i < 10; i++) {
      STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // OFF 
      showStrips();
    }  
  }

  if ((leds & CHOICE_YELLOW) != 0) {
    for(i=0; i < 10; i++) {
      STRIP_L.setPixelColor(i, STRIP_L.Color(255, 255, 0)); // yellow  
      STRIP_F.setPixelColor(i, STRIP_F.Color(0, 0, 0)); // OFF 
      STRIP_B.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // OFF 
      STRIP_R.setPixelColor(i, STRIP_R.Color(0, 0, 0)); // OFF 
      showStrips();
    }
  } else {

    for(i=0; i < 10; i++) {
      STRIP_L.setPixelColor(i, STRIP_L.Color(0, 0, 0)); // OFF 
      showStrips();
    }  
  }

}

// Light an LED
void toner(byte which, int buzz_length_ms)
{
  setLEDs(CHOICE_OFF); // Turn off all LEDs
  setLEDs(which); //Turn on a given LED
}

// Plays the current contents of the game moves
void playMoves(void) {
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) {

    toner(gameBoard[currentMove], 1000);
    
    // Shorten this to make game harder
    delay(1000); // 1000 works well. // it is still very hard
    setLEDs(CHOICE_OFF); // Turn off all LEDs
  }
}

// Wait for a selection to be activated. 
// Returns which way the accel is selected if successful, 0 if timed out
byte wait_for_button(void) {
  long startTime = millis(); // Remember the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {

  byte button = checkStrip();

  if (button != CHOICE_NONE) { 
      toner(button, 500); // Play the button the user just pressed

      while(checkStrip() != CHOICE_NONE) ;  // Now let's wait for user to release button
      delay(10);
      return button;
    }
  }

  return CHOICE_NONE; // If we get here, we've timed out!
}

// Play the regular memory game
// Returns 0 if player loses, or 1 if player wins
boolean play_memory(void)
{
  randomSeed(millis()); // Seed the random generator with random amount of millis()
  gameRound = 0; // Reset the game to the beginning

  while (gameRound < ROUNDS_TO_WIN) {

    play_between();

    add_to_moves(); // Add a button to the current moves, then play them back

    playMoves(); // Play back the current game board

    // Then require the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) {
      byte choice = wait_for_button(); // See what button the user presses
	  delay(500);
      if (choice == 0) return false; // If wait timed out, player loses
      if (choice != gameBoard[currentMove]) return false; // If the choice is incorect, player loses
    }
    delay(1000); // Player was correct, delay before playing moves
  }
  return true; // Player made it through all the rounds to win!
}
