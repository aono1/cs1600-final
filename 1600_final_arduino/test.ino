
// A struct to keep state inputs in one place
typedef struct {
  float aud_volts_fsm;
  float joy_x_fsm;
  long mils;
} state_inputs;

//  A struct to keep state variables in one place
typedef struct {
  bool PAUSE_PRESSED;
  bool RESTART_PRESSED;
  int SAVED_CLOCK;
  int LAST_OBSTACLE;
} state_vars;

// declaration
bool testTransition(state startState,
                     state endState,
                     state_inputs testStateInputs, 
                     state_vars startStateVars,
                     state_vars endStateVars);

// Helper function for printing states
char* s2str(state s) {
  switch(s) {
    case sSTATIC:
    return "(1) sSTATIC";
    case sJUMP_SENT:
    return "(2) sJUMP_SENT";
    case sDUCK_SENT:
    return "(3) sDUCK_SENT";
    case sUNDUCK_SENT:
    return "(4) sUNDUCK_SENT";
    case sPAUSE_SENT:
    return "(5) sPAUSE_SENT";
    case sRESTARTING:
    return "(6) sRESTARTING";
    default:
    return "ayowhatsgoingon???";
  }
}


/*
 * Given a start state, inputs, and starting values for state variables, tests that
 * updateFSM returns the correct end state and updates the state variables correctly
 * returns true if this is the case (test passed) and false otherwise (test failed)
 * 
 */
bool testTransition(state startState,
                     state endState,
                     state_inputs testStateInputs, 
                     state_vars startStateVars,
                     state_vars endStateVars) {

  state resultState = updateFSM(startState, testStateInputs.mils, testStateInputs.joy_x_fsm, testStateInputs.aud_volts_fsm);
  bool passedTest = (endState == resultState and
                      startStateVars.PAUSE_PRESSED == endStateVars.PAUSE_PRESSED and
                      startStateVars.RESTART_PRESSED == endStateVars.RESTART_PRESSED and
                      startStateVars.SAVED_CLOCK == endStateVars.SAVED_CLOCK and
                      startStateVars.LAST_OBSTACLE == endStateVars.LAST_OBSTACLE);

    char sToPrint[200];
    sprintf(sToPrint, "Testing from %s to %s", s2str(startState), s2str(endState));
    Serial.println(sToPrint);

    sprintf(sToPrint, "End state expected: %s | actual: %s", s2str(endState), s2str(resultState));
    Serial.println(sToPrint);

    sprintf(sToPrint, "Inputs: mils %ld | joy_x_fsm %.2f | aud_volts_fsm %.2f", testStateInputs.mils, testStateInputs.joy_x_fsm, testStateInputs.aud_volts_fsm);
    Serial.println(sToPrint);


    if (startStateVars.PAUSE_PRESSED) {
      Serial.println("pause is pressed");
    } else {
      Serial.println("pause is not pressed");
    }

    return passedTest;
}

// TEST CASES
// 1-5, 

const state testStatesIn[1] = {(state) 1};
const state testStatesOut[1] = {(state) 5};

// state: aud_volts_fsm, joy_x_fsm, mils
const state_inputs testInputs[1] = {
  {0.0, 0.0, 15}
  };

// state_vars: PAUSE_PRESSED, RESTART_PRESSED, SAVED_CLOCK, LAST_OBSTACLE
const state_vars testVarsIn[1] = {
  {true, false, 10, 10}
  };
const state_vars testVarsOut[1] = {
  {false, false, 10, 10}
  };

const int numTests = 1;


/*
 * Runs through all the test cases defined above
 */
bool testAllTests() {
  Serial.println("runnning testalltests!");
  for (int i = 0; i < numTests; i++) {
    Serial.print("Running test ");
    Serial.println(i);
    if (!testTransition(testStatesIn[i], testStatesOut[i], testInputs[i], testVarsIn[i], testVarsOut[i])) {
      Serial.print("Test ");
      Serial.print(i);
      Serial.print(" failed!");
      return false;
    }
  }
  Serial.println("All tests passed!");
  return true;
}

