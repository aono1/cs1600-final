// Input pins
#define JOYS_VRX_DIO A0 // Player Joystick X-Axis
#define JOYS_VRY_DIO A1 // Player Joystick Y-Axis
#define JOYS_SW_DIO 2 // Player Joystick Push Button
#define AUDIO_IN 5 // Mic Input
#define JOY_X_CAP A2 // Sabotage Joystick X-Axis
#define JOY_Y_CAP A3 // Sabotage Joystick Y-Axis
#define JOY_SW_CAP 3 // Sabotage Joystick Push Button

// Variables to keep track of inputs updated in updateInputs()
float JOY_X; // Player Joystick X-Axis
float JOY_Y; // Player Joystick Y-Axis
double AUD_VOLTS; // Mic Input
float CAP_JOY_X; // Sabotage Joystick X-Axis
float CAP_JOY_Y; // Sabotage Joystick Y-Axis

// Sampling Variables
const int sampleWindow = 100;
unsigned int aud_signal;

// States 
typedef enum {
  sSTATIC = 1,
  sJUMP_SENT = 2,
  sDUCK_SENT = 3,
  sUNDUCK_SENT = 4,
  sPAUSE_SENT = 5,
  sRESTARTING = 6
} state;

// Input update function
void updateInputs();