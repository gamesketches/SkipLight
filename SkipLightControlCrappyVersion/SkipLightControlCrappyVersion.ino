uint8_t neighbors[6];
uint8_t colors[9][3] = {{0, 0, 0},    // Clear
                          {255,0,0},   // Red
                         {0,0,255},    // Blue
                         {255,255,0},  // Yellow
                         {128, 0, 128}, // Purple
                         {255, 165, 0}, // Orange
                         {0, 255, 0}, // Green
                         {255,255,255},  // Game Over Color
                         {0, 0, 0} // Reset color
                         };
uint8_t myColor = 1;

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
  setState(0);//CLEAR);
}

void resetGame() {
  setState(10);//RESET);
  setColor(colors[5]);
  setTimerCallback(returnToNeutral, 3000);
}

void returnToNeutral() {
    setColor(colors[myColor]);
    setState(0);//CLEAR);
}

