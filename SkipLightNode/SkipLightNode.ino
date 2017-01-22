uint8_t neighbors[6];
uint8_t colors[11][3] = {{0, 0, 0},    // Clear
                         {0, 0, 0}, //empty color
                          {255,0,0},   // Red
                         {0,0,255},    // Blue
                         {128, 0, 128}, // rando
                         {128, 0, 128}, // Purple
                         {255,255,255},  // Game Over Color
                         {0,255,0},  // Yellow
                         {0, 0, 0}, // Reset color
                         {255, 165, 0}, // Orange
                         {255, 255, 0} // Green
                         }; 

enum blinkState {CLEAR, GOAL, RED, BLUE, SUCCESS, PURPLE, GAMEOVER, GREEN, RESET, ORANGE, YELLOW, FAILURE}; 

blinkState storedColor;
blinkState targetColor;

bool blinking;
bool blinkState;
uint8_t blinkTimer = 0;

uint8_t propagationTime = 2000;

void setMyColor(uint8_t color);
void becomeClear();
bool isPrimaryColor(uint8_t color);
void becomeClear();
void setStoredColor(uint8_t state);

void setup() {
  // put your setup code here, to run once:
  setLongButtonCallback(becomeGoal, 2000);
  setMicOff();
  setState(CLEAR);
  setColor(colors[CLEAR]);
  blinkState = true;
  blinking = false;
  storedColor = CLEAR;
}

void loop() {
  if(blinking) {
     blinkTimer++;
     if(blinkTimer > 600) {
       if(blinkState) {
          blinkState = false;
          setColor(colors[CLEAR]);
       }
       else {
          blinkState = true;
          setColor(colors[targetColor]);
       }
       setColor(colors[BLUE]);
       blinkTimer = 0;
     }
  }
  // put your main code here, to run repeatedly:
  getNeighborStates(neighbors);

  for(uint8_t i = 0; i < 6; i++) {
      if(neighbors[i] == RESET) {
        setColor(colors[RESET]);
        setState(RESET);
        setTimerCallback(becomeClear, 5000);
      }
      else if(neighbors[i] == GAMEOVER) {
        setColor(colors[GAMEOVER]);
        setState(GAMEOVER);
      }
      else if(isPrimaryColor(neighbors[i])) {
        if(getState() == GOAL) {
          if(storedColor == CLEAR) {
            setStoredColor(neighbors[i]);
            setTimerCallback(unStoreColor, propagationTime);
          }
          else if(storedColor + neighbors[i] == targetColor) {
             setState(SUCCESS);
             setColor((uint8_t*)storedColor + neighbors[i]);
             blinking = true;
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
  setColor(colors[CLEAR]);
  setState(CLEAR);
}

bool isPrimaryColor(uint8_t color) {
  if(color == RED || color == BLUE || color == YELLOW) {
      return true;
  }
  return false;
}

void becomeGoal() {
  if(getState() != GOAL && getState() != SUCCESS) {
    uint32_t diceRoll = getTimer() % 3;
    switch(diceRoll) {
      case 0: 
        targetColor = PURPLE;
        break;
      case 1:
        targetColor = ORANGE;
        break;
      case 2:
        targetColor = GREEN;
        break;
  };

  setColor(colors[targetColor]);
  setState(GOAL);
  }
}

void unStoreColor() {
  storedColor = CLEAR;
}

void setStoredColor(uint8_t state) {
  switch(state) {
    case 2: 
      storedColor = RED;
    case 3:
      storedColor = BLUE;
    case 7:
      storedColor = YELLOW;
    default:
      storedColor = CLEAR;
  }
}

