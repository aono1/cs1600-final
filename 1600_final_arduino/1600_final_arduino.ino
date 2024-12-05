#include "1600_final_arduino.h"
#include <ezButton.h>
// #include "1600_final_arduino_utils.ino"    

// FSM Variables
static bool PAUSE_PRESSED;
static bool GAME_PAUSED;
static int SAVED_CLOCK;
static int TIMESTEP;
static int LAST_OBSTACLE;
static int OBSTACLE_TIMESTEP;


void setup() {
  pinMode(AUDIO_IN, INPUT);    
  // pinMode(PAUSE_BUT, INPUT);  
  pinMode(CACTUS_BUT, INPUT);    

  Serial.begin(9600);                      
  delay(1000);
  Serial.println("Started");

  pinMode(JOYS_SW_DIO, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(JOYS_SW_DIO), pauseISR, LOW); 
  attachInterrupt(digitalPinToInterrupt(CACTUS_BUT), cactusISR, RISING); 

  TIMESTEP = 100;
  OBSTACLE_TIMESTEP = 750;

  GAME_ENDED = false;
  ADD_CACTUS = false;
}

void updateInputs() {
  JOY_X = analogRead(JOYS_VRX_DIO);
  JOY_Y = analogRead(JOYS_VRY_DIO);


  unsigned long start= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

 // collect data for 250 miliseconds
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

  // Detect gameover
  // String commStr  = Serial.readString();
  // if (commStr == "OVER") {
  //   GAME_ENDED = true;
  // }
  // else if (commStr == "START") {
  //   GAME_ENDED = false;
  // }
}

void cactusISR() {
  if ((millis() - LAST_OBSTACLE) >=  OBSTACLE_TIMESTEP) {
    ADD_CACTUS = true;       
  }
}

void pauseISR() {
  PAUSE_PRESSED = true;               
}

void loop() {
  static state CURRENT_STATE = sSTATIC;
  updateInputs();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), JOY_X, JOY_Y, AUD_VOLTS);


  if (ADD_CACTUS) {
    Serial.println("CACTUS");
    ADD_CACTUS = false;
    LAST_OBSTACLE = millis();
  }

  // if (PAUSE_PRESSED) {
  //   Serial.println("PAUSE");
  //   PAUSE_PRESSED = false;
  // }

  // if (!digitalRead(JOYS_SW_DIO)) {
  //   Serial.println("JOY BUTTON");
  // }
}

state updateFSM(state curState, long mils, float joy_x_fsm, float joy_y_fsm, float aud_volts_fsm) {
  state nextState;

  switch(curState) {
    case sSTATIC:
      if (PAUSE_PRESSED) { // 1-5
        Serial.println("PAUSE");
        nextState = sPAUSE_SENT;
        PAUSE_PRESSED = false;
      }
      else if (aud_volts_fsm >= 1.0) { // 1-2
        Serial.println("JUMP");
        SAVED_CLOCK = mils;
        nextState = sJUMP_SENT;
      } 
      else if (joy_x_fsm > 600) { // 1-3
        Serial.println("DUCK");
        nextState = sDUCK_SENT;
      }
      else { 
        nextState = sSTATIC; // 1-1
      }
      break;
    case sJUMP_SENT:
      if ((mils - SAVED_CLOCK) >= TIMESTEP) { // 2-1
        nextState = sSTATIC;
      } 
      else { // 2-2
        nextState = sJUMP_SENT;
      }
      break;
    case sDUCK_SENT:
      if (joy_x_fsm < 400) { // 3-4
        Serial.println("UNDUCK");
        nextState = sUNDUCK_SENT;
      }
      else { // 3-3
        nextState = sDUCK_SENT;
      }
      break;
    case sUNDUCK_SENT:
      if ((mils - SAVED_CLOCK) >= 100) { // 4-1
        nextState = sSTATIC;
      }
      else { // 4-4
        nextState = sUNDUCK_SENT;
      }
      break;
    case sPAUSE_SENT:
      if (PAUSE_PRESSED && ((mils - SAVED_CLOCK) >= 300)) { // 5-6
        Serial.println("UNPAUSE");
        nextState = sSTATIC;
        PAUSE_PRESSED = false;
      }
      else { // 5-5
        nextState = sPAUSE_SENT;
      }
      break;

    // case sGAME_ENDED:
    //   if (!GAME_ENDED) { // 7-1
    //     nextState = sSTATIC;
    //   } 
    //   else { // 7-7
    //     nextState = sGAME_ENDED;
    //   }
  }

  return nextState;
}


// state updateFSM(state curState, long mils, float joy_x_fsm, float joy_y_fsm, float aud_volts_fsm) {
//   state nextState;

//   switch(curState) {
//     case sSTATIC:
//       if (GAME_ENDED) { // 1-7
//         nextState = sGAME_ENDED;
//       }
//       else if ((aud_volts_fsm >= 1.0) && (!GAME_ENDED)) { // 1-2
//         Serial.println("JUMP");
//         SAVED_CLOCK = mils;
//         nextState = sJUMP_SENT;
//       } 
//       else if ((joy_x_fsm > 600) && (!GAME_ENDED)) { // 1-3
//         Serial.println("DUCK");
//         nextState = sDUCK_SENT;
//       }
//       else { 
//         nextState = sSTATIC; // 1-1
//       }
//       break;
//     case sJUMP_SENT:
//       if (GAME_ENDED) { // 2-7
//         nextState = sGAME_ENDED;
//       }
//       else if (((mils - SAVED_CLOCK) >= TIMESTEP) && (!GAME_ENDED)) { // 2-1
//         nextState = sSTATIC;
//       } 
//       else { // 2-2
//         nextState = sJUMP_SENT;
//       }
//       break;
//     case sDUCK_SENT:
//       if (GAME_ENDED) { // 3-7
//         nextState = sGAME_ENDED;
//       }
//       else if ((joy_x_fsm < 400) && (!GAME_ENDED)) { // 3-4
//         Serial.println("UNDUCK");
//         nextState = sUNDUCK_SENT;
//       }
//       else { // 3-3
//         nextState = sDUCK_SENT;
//       }
//       break;
//     case sUNDUCK_SENT:
//       if (GAME_ENDED) { // 4-7
//         nextState = sGAME_ENDED;
//       }
//       else if (((mils - SAVED_CLOCK) >= 100) && (!GAME_ENDED)) { // 4-1
//         nextState = sSTATIC;
//       }
//       else { // 4-4
//         nextState = sUNDUCK_SENT;
//       }
//       break;
//     case sGAME_ENDED:
//       if (!GAME_ENDED) { // 7-1
//         nextState = sSTATIC;
//       } 
//       else { // 7-7
//         nextState = sGAME_ENDED;
//       }
//   }

//   return nextState;
// }






