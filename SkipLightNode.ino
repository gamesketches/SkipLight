uint8_t neighbors[6];
uint8_t colors[6][3] = {{0, 0, 0},    // Clear
                         {0, 0, 0}, //empty color
                          {255,0,0},   // Red
                         {0,0,255},    // Blue
                         {128, 0, 128}, // rando
                         {128, 0, 128}, // Purple
                         {255,255,255},  // Game Over Color
                         {255,255,0},  // Yellow
                         {0, 0, 0}, // Reset color
                         {255, 165, 0}, // Orange
                         {0, 255, 0}, // Green
                         }; 

enum blinkState {CLEAR, GOAL, RED, BLUE, SUCCESS, PURPLE, GAMEOVER, YELLOW, RESET, ORANGE, GREEN, FAILURE}; 

blinkState storedColor;
blinkState targetColor;

bool blinking;
bool blinkState;
uint8_t blinkTimer = 0;

uint8_t propagationTime = 2000;

void setup() {
  // put your setup code here, to run once:
  setLongButtonCallback(becomeGoal, 2000);
  setMicOff();
  setState(CLEAR);
  setColor(colors[CLEAR]);
  blinkState = true;
  blinking = false;
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
       blinkTimer = 0;
     }
  }
  // put your main code here, to run repeatedly:
  getNeighborStates(neighbors);

  for(uint8_t i = 0; i < 6; i++) {
      if(neighbors[i] == RESET) {
        setColor(RESET);
        setState(RESET);
        setTimerCallback(becomeClear, 5000);
      }
      else if(neighbors[i] == GAMEOVER) {
        setColor(colors[GAMEOVER]);
        setState(GAMEOVER);
      }
      else if(isPrimaryColor(neighbors[i])) {
        if(getState() == GOAL) {
          if(!storedColor) {
            storedColor = neighbors[i];
            setTimerCallback(unStoreColor, propagationTime);
          }
          else if(storedColor + neighbors[i] == targetColor) {
             setState(SUCCESS);
             setColor(storedColor + neighbors[i]);
             blinking = true;
              }
          }
        else {
          setMyColor(neighbors[i]);
        }
      }
  }

  
}

void setMyColor(blinkState color) {
  setColor(colors[blinkState]);
  setState(color);
  setTimerCallback(becomeClear, propagationTime);
}

void becomeClear() {
  setColor(colors[CLEAR]);
  setState(CLEAR);
}

bool isPrimaryColor(blinkState color) {
  if(color == RED || color == BLUE || color == YELLOW) {
      return true;
  }
  return false;
}

void becomeGoal() {
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

  setColor(targetColor);
  setState(GOAL);
}

void unStoreColor() {
  storedColor = NULL;
}

