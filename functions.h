
long previousMillis = 0;
long badTime = 0;

void play_welcome() {
  for(int w=0; w<5; w++) {
    initStrips();
  }
}

void gatherAccelData() {
  static byte source;

  if (digitalRead(int1Pin)==1) {

    readAccelData(accelCount); 
    
    float valXSum=0;
    float valYSum=0;
    float valZSum=0;
    
    for(int i=0;i<89;i++) {
      for (int a=0; a<3; a++) {
        accelG[a] = (float) accelCount[a]/((1<<12)/(2*SCALE));
      }

      if(accelG[2] < 1.0) {
        valXSum = (accelG[0]+valXSum);
        valYSum = (accelG[1]+valYSum);
        valZSum = (accelG[2]+valZSum);
      }
    }

    accelG[0] = valXSum/90;
    accelG[1] = valYSum/90;
    accelG[2] = valZSum/90;

  }
}

void checkPlayerLose(long currentMillis, int ACTIVE_PIXEL, int ACTIVE_PIXEL_L) {
      // smooth the reading a bit ... give the player a change to get out of the red before we bork them
      if(ACTIVE_PIXEL >= 10 || ACTIVE_PIXEL_L >= 10) {
      } else {
          previousMillis = currentMillis;
      }
      if((ACTIVE_PIXEL >= 10 || ACTIVE_PIXEL_L >= 10) && (currentMillis - previousMillis > GAME_LEVEL) ) {
          GAME_OVER = 1;
      } else {
          //Serial.println("Game on!");
          GAME_OVER = 0; 
      }

        if(GAME_OVER == 1) {
          // give player x time to "fix" the errors
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
          resetStrips(ACTIVE_PIXEL, ACTIVE_PIXEL_L);
        }

}


void gameChoiceChecker() {
  static byte source;
  // If int2 goes high, either p/l has changed or there's been a single/double tap
  if (digitalRead(int2Pin)==1)
  {
    source = readRegister(0x0C);  // Read the interrupt source reg.
	if ((source & 0x08)==0x08) {  // Otherwise, if tap register is set go check that
      tapHandler();
	}
  }
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkStrip(void) {
  gatherAccelData();

  /////////////////////////////////////////////////////////////
  //                       red(STRIP_F)(1)
  //       green(STRIP_L)(4)                yellow(STRIP_R)(8)
  //                       blue(STRIP_B)(4)
  /////////////////////////////////////////////////////////////
  PIXEL_LEVEL_RED    = (accelG[0]*100);
  PIXEL_LEVEL_BLUE   = (accelG[0]*100);
  PIXEL_LEVEL_GREEN  = (accelG[1]*100);
  PIXEL_LEVEL_YELLOW = (accelG[1]*100);

  int ACTIVE_PIXEL_RED    = map(PIXEL_LEVEL_RED, 0, 25, 0, 9);
  int ACTIVE_PIXEL_BLUE   = map(PIXEL_LEVEL_BLUE, 0, 25, 0, 9);
  int ACTIVE_PIXEL_GREEN  = map(PIXEL_LEVEL_GREEN, 0, 25, 0, 9);
  int ACTIVE_PIXEL_YELLOW = map(PIXEL_LEVEL_YELLOW, 0, 25, 0, 9);

  if(ACTIVE_PIXEL_RED    > 9)  { return(CHOICE_RED); }
  if(ACTIVE_PIXEL_BLUE   < -9) { return(CHOICE_BLUE); }
  if(ACTIVE_PIXEL_GREEN  > 9)  { return(CHOICE_GREEN); }
  if(ACTIVE_PIXEL_YELLOW < -9) { return(CHOICE_YELLOW); }

  // default response
  return(CHOICE_NONE); // If no button is pressed, return none
}



