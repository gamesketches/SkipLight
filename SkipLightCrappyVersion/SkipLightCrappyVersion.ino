uint8_t neighbors[6];
uint8_t colors[9][3] = {{0, 0, 0},    // Clear
                          {255,0,0},   // Red
                         {0,0,255},    // Blue
                         {255,255,0},  // Yellow
                         {128, 0, 128}, // Purple
                         {255, 165, 0}, // Orange
                         {0, 255, 0}, // Green
                         {255,255,255},  // Goal Color
                         {255, 255, 255} // Success color
                         }; 

/* Clear = 0
 *  Red = 1
 *  Blue = 2
 *  Yellow = 3
 *  Purple = 4
 *  Orange = 5
 *  Green = 6
 *  Goal = 7
 *  Success = 8
 *  GameOver = 9
 *  Reset = 10
 */

uint8_t storedColor;
uint8_t targetColor;

bool blinking;
bool blinkState;
uint8_t blinkTimer = 0;

uint16_t propagationTime = 2000;

void setMyColor(uint8_t color);
void becomeClear();
bool isPrimaryColor(uint8_t color);
void becomeClear();
void setStoredColor(uint8_t state);

void setup() {
  // put your setup code here, to run once:
  setLongButtonCallback(becomeGoal, 2000);
  setMicOff();
  setState(0); // Clear
  setColor(colors[0]); // Clear
  blinkState = true;
  blinking = false;
  storedColor = 0; // Clear
  setTimeout(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  getNeighborStates(neighbors);

  for(uint8_t i = 0; i < 6; i++) {
      if(neighbors[i] == 10){//RESET) {
        setColor(colors[0]);//RESET]);
        setState(10);//RESET);
        setTimerCallback(becomeClear, 5000);
      }
      else if(neighbors[i] == 9){//GAMEOVER) {
        setColor(colors[9]);//GAMEOVER]);
        setState(9);//GAMEOVER);
      }
      else if(getState() < 8 && neighbors[i] > 0 && neighbors[i] < 4) {
        if(getState() == 7){//GOAL) {
          if(storedColor == 0){//CLEAR) {
            storedColor = neighbors[i];
            setTimerCallback(unStoreColor, propagationTime);
          }
          else if(neededColor(neighbors[i])) {
             setState(8);//SUCCESS);
             setColor(colors[8]);
             blinkLight();
              }
           else {
            setColor(colors[targetColor]);
           }
          }
        else {
          setMyColor(neighbors[i]);
        }
      }
  }

  
}

void setMyColor(uint8_t color) {
  setColor(colors[color]);
  setState(color);
  setTimerCallback(becomeClear, propagationTime);
}

void becomeClear() {
  setColor(colors[0]);//CLEAR]);
  setState(0);//CLEAR);
  blinking = false;
}

void becomeGoal() {
   blinking = true;
  if(getState() != 1 && getState() != 4){//GOAL && getState() != SUCCESS) {
    uint32_t diceRoll = getTimer() % 3;
    targetColor = diceRoll + 4;

  setColor(colors[targetColor]);
  setState(7);//GOAL);
  }
}

void unStoreColor() {
  storedColor = 0;//CLEAR;
}

bool neededColor(uint8_t state) {
  switch(storedColor) {
    case 1: // RED
      if(state == 2 && targetColor == 4) { // BLUE = PURPLE
        return true; // Purple
      }
      else if(state == 3 && targetColor == 5) { // YELLOW = ORANGE
        return true;
      }
      break;
    case 2: // BLUE
      if(state == 1 && targetColor == 4) { // RED = PURPLE
        return true; // Purple
      }
      else if(state == 3 && targetColor == 6) { // YELLOW = GREEN
        return true;
      }
      break;
    case 3: // YELLOW
      if(state == 1 && targetColor == 5) { // RED = ORANGE
        return true;
      }
      else if(state == 2 && targetColor == 6) { // BLUE = GREEN
        return true;
      }
      break;
      };
      return false;
}

void blinkLight() {
  if(blinkState) {
    blinkState = false;
    setColor(colors[0]);
  }
  else {
    blinkState = true;
    setColor(colors[targetColor]);
  }

  if(blinking) {
    setTimerCallback(blinkLight, 600);
  }
}

