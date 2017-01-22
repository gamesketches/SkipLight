uint8_t neighbors[6];
uint8_t colors[11][3] = {{0, 0, 0},    // Clear
                         {0, 0, 0}, //empty color
                          {255,0,0},   // Red
                         {0,0,255},    // Blue
                         {128, 0, 128}, // rando
                         {128, 0, 128}, // Purple
                         {255,255,255},  // Game Over Color
                         {255,255,0},  // Yellow
                         {0, 0, 0}, // Reset color
                         {255, 165, 0}, // Orange
                         {0, 255, 0} // Green
                         }; 

//enum blinkState {CLEAR, GOAL, RED, BLUE, SUCCESS, PURPLE, GAMEOVER, YELLOW, RESET, ORANGE, GREEN, FAILURE}; 
/* Clear = 0
 *  Goal = 1
 *  Red = 2
 *  Blue = 3
 *  Success = 4
 *  Purple = 5
 *  GameOver = 6
 *  Yellow = 7
 *  Reset = 8
 *  Orange = 9
 *  Green = 10
 *  Failure = 11
 */
//blinkState storedColor;
//blinkState targetColor;

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
}

void loop() {
  if(blinking) {
     blinkTimer++;
     if(blinkTimer > 600) {
       if(blinkState) {
          blinkState = false;
          setColor(colors[0]);//CLEAR]);
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
      if(neighbors[i] == 8){//RESET) {
        setColor(colors[8]);//RESET]);
        setState(8);//RESET);
        setTimerCallback(becomeClear, 5000);
      }
      else if(neighbors[i] == 6){//GAMEOVER) {
        setColor(colors[6]);//GAMEOVER]);
        setState(6);//GAMEOVER);
      }
      else if(isPrimaryColor(neighbors[i])) {
        if(getState() == 1){//GOAL) {
          if(storedColor == 0){//CLEAR) {
            setStoredColor(neighbors[i]);
            setTimerCallback(unStoreColor, propagationTime);
          }
          else if(storedColor + neighbors[i] == targetColor) {
             setState(4);//SUCCESS);
             setColor(colors[2]);//colors[storedColor + neighbors[i]]);
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
  setColor(colors[0]);//CLEAR]);
  setState(0);//CLEAR);
}

bool isPrimaryColor(uint8_t color) {
  if(color == 2 || color == 3 || color == 5) {
      return true;
  }
  return false;
}

void becomeGoal() {
  if(getState() != 1 && getState() != 4){//GOAL && getState() != SUCCESS) {
    uint32_t diceRoll = getTimer() % 3;
    switch(diceRoll) {
      case 0: 
        targetColor = 5;//PURPLE;
        break;
      case 1:
        targetColor = 9;//ORANGE;
        break;
      case 2:
        targetColor = 10;//GREEN;
        break;
  };

  setColor(colors[targetColor]);
  setState(1);//GOAL);
  }
}

void unStoreColor() {
  storedColor = 0;//CLEAR;
}

void setStoredColor(uint8_t state) {
  switch(state) {
    case 2: 
      storedColor = 2;//RED;
      break;
    case 3:
      storedColor = 3;//BLUE;
      break;
    case 7:
      storedColor = 7;//YELLOW;
      break;
    default:
      storedColor = 0;//CLEAR;
      break;
  }
}

