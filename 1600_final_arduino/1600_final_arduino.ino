#include "1600_final_arduino.h"

// FSM Variables
static bool PAUSE_PRESSED;
static bool RESTART_PRESSED;
static int SAVED_CLOCK;

// FSM capstone variables
static int LAST_OBSTACLE;

// Some file-global information
// Edit this to make the game harder, especially OBSTACLE_TIMESTEP
// Also edit for audio and joystick sensitivity
const int JUMP_TIMESTEP = 600; 
const int OBSTACLE_TIMESTEP = 750;
const int PAUSE_TIMESTEP = 300;
const int DUCK_TIMESTEP = 100;
const float AUDIO_SENSITIVITY = 1.0; // increase in case of noisy environment 
const int DUCK_SENSITIVITY = 600;


void setup() {
  // Take in audio input
  pinMode(AUDIO_IN, INPUT);      

  // Start serial
  Serial.begin(9600);                      
  delay(1000);
  Serial.println("Started"); // For debugging, Python code ignores this

  // Joystick buttons must be set with pullup
  pinMode(JOYS_SW_DIO, INPUT_PULLUP);
  pinMode(JOY_SW_CAP, INPUT_PULLUP);

  // Attach interrupts for pausing and restarting
  attachInterrupt(digitalPinToInterrupt(JOYS_SW_DIO), pauseISR, LOW); 
  attachInterrupt(digitalPinToInterrupt(JOY_SW_CAP), startISR, LOW); 

  // Define starting state of variables
  // Technically not needed, but paranoia is always smart
  RESTART_PRESSED = false;
  PAUSE_PRESSED = false;

  // Initalize 
  SAVED_CLOCK = 0;
  LAST_OBSTACLE = -OBSTACLE_TIMESTEP; 


  // get the next available cpu interrupt for WDT 
  WDT_INT = getNextCPUINT(1);

  // initiate the WDT
  initWDT();
  petWDT();
}

// Updates variables with input from hardware
void updateInputs() {
  // Read joystick axes
  JOY_X = analogRead(JOYS_VRX_DIO);
  CAP_JOY_X = analogRead(JOY_X_CAP);

  // Process audio input
  unsigned long start = millis();  
  unsigned int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  while (millis() - start < sampleWindow) {
    aud_signal = analogRead(AUDIO_IN);
    if (aud_signal < 1024)  {
        if (aud_signal > signalMax) {
          signalMax = aud_signal;  
        }
        else if (aud_signal < signalMin) {
          signalMin = aud_signal;  
        }
    }
  }
  peakToPeak = signalMax - signalMin; 
  AUD_VOLTS = (peakToPeak * 3.3) / 1024;  
}

// ISRs just set flags
// Based on online information, this seems to be best practice 
// instead of printing in ISRs – which encounters the same priority bug from lab
void pauseISR() {
  PAUSE_PRESSED = true;               
}
void startISR() {
  RESTART_PRESSED = true;               
}

// Code for the sabotage player to add obstacles
// Not in the main FSM because the sabotage player should be able to add obstacles
// independent of the state of the dinosaur
void addObstacle() {
  if ((CAP_JOY_X > 600) && ((millis() - LAST_OBSTACLE) >=  OBSTACLE_TIMESTEP)) {
    Serial.println("CACTUS");
    LAST_OBSTACLE = millis();
  } 
  else if ((CAP_JOY_X < 400) && ((millis() - LAST_OBSTACLE) >=  OBSTACLE_TIMESTEP)) {
    Serial.println("BIRD");
    LAST_OBSTACLE = millis();
  } 
}

// Main loop
void loop() {
  for (int i = 0; i<100; i++){
    static state CURRENT_STATE = sSTATIC;
    updateInputs();
    addObstacle();
    CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), JOY_X, AUD_VOLTS);
    petWDT(); // pet watchdog here 
  }
  while(1);
}

state updateFSM(state curState, long mils, float joy_x_fsm, float aud_volts_fsm) {
  state nextState;
  switch(curState) {
    case sSTATIC:
      if (PAUSE_PRESSED) { // 1-5
        Serial.println("PAUSE");
        PAUSE_PRESSED = false;
        nextState = sPAUSE_SENT;
      }
      else if (joy_x_fsm > DUCK_SENSITIVITY && !PAUSE_PRESSED) { // 1-3
        Serial.println("DUCK");
        nextState = sDUCK_SENT;
      }
      else if (aud_volts_fsm >= AUDIO_SENSITIVITY && !PAUSE_PRESSED && !(joy_x_fsm > DUCK_SENSITIVITY)) { // 1-2
        Serial.println("JUMP");
        SAVED_CLOCK = mils;
        nextState = sJUMP_SENT;
      } 
      else if (RESTART_PRESSED && !PAUSE_PRESSED && !(joy_x_fsm > DUCK_SENSITIVITY) && !(aud_volts_fsm >= AUDIO_SENSITIVITY)) { // 1-6
        Serial.println("START");
        RESTART_PRESSED = false;
        nextState = sRESTARTING;
      }
      else { 
        nextState = sSTATIC; // 1-1
      }
      break;
    case sJUMP_SENT:
      if ((mils - SAVED_CLOCK) >= JUMP_TIMESTEP) { // 2-1
        nextState = sSTATIC;
      } 
      else if (RESTART_PRESSED && !((mils - SAVED_CLOCK) >= JUMP_TIMESTEP)) { // 2-6
        Serial.println("START");
        RESTART_PRESSED = false;
        nextState = sRESTARTING;
      }
      else { // 2-2
        nextState = sJUMP_SENT;
      }
      break;
    case sDUCK_SENT:
      if (!(joy_x_fsm > DUCK_SENSITIVITY)) { // 3-4
        Serial.println("UNDUCK");
        nextState = sUNDUCK_SENT;
      }
      else if (RESTART_PRESSED && (joy_x_fsm > DUCK_SENSITIVITY)) { // 3-6
        Serial.println("START");
        RESTART_PRESSED = false;
        nextState = sRESTARTING;
      }
      else { // 3-3
        nextState = sDUCK_SENT;
      }
      break;
    case sUNDUCK_SENT:
      if ((mils - SAVED_CLOCK) >= DUCK_TIMESTEP) { // 4-1
        nextState = sSTATIC;
      }
      else if (RESTART_PRESSED && !(mils - SAVED_CLOCK) >= DUCK_TIMESTEP){ // 4-6
        Serial.println("START");
        RESTART_PRESSED = false;
        nextState = sRESTARTING;
      }
      else { // 4-4
        nextState = sUNDUCK_SENT;
      }
      break;
    case sPAUSE_SENT:
      if (PAUSE_PRESSED && ((mils - SAVED_CLOCK) >= PAUSE_TIMESTEP)) { // 5-1
        Serial.println("UNPAUSE");
        PAUSE_PRESSED = false;
        nextState = sSTATIC;
      }
      else if (RESTART_PRESSED && !(PAUSE_PRESSED && ((mils - SAVED_CLOCK) >= PAUSE_TIMESTEP))) { // 5-6
        Serial.println("START");
        RESTART_PRESSED = false;
        nextState = sRESTARTING;
      }
      else { // 5-5
        nextState = sPAUSE_SENT;
      }
      break;
    case sRESTARTING:
      nextState = sSTATIC; // 6-1
      break;
  }
  return nextState;
}