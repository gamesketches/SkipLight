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

enum blinkState {CLEAR, GOAL, RED, BLUE, SUCCESS, PURPLE, GAMEOVER, YELLOW, RESET, ORANGE, GREEN}; 

blinkState myColor = RED;

uint8_t propagationTime = 2000;

void setup() {
  setButtonCallback(propagate);
  setLongButtonCallback(resetGame, 4000);
  setMicOff();
  returnToNeutral();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void propagate() {
  setState(myColor);
  setTimerCallback(stopPropagating, propagationTime);
}

void stopPropagating() {
  setState(CLEAR);
}

void resetGame() {
  setState(RESET);
  setTimerCallback(returnToNeutral, 3000);
}

void returnToNeutral() {
    setColor(myColor);
    setState(CLEAR);
}

