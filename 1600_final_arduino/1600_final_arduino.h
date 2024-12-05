// Defining all input pins
#define JOYS_VRX_DIO A0 // Joystick X-Axis
#define JOYS_VRY_DIO A1 // Joystick Y-Axis
#define JOYS_SW_DIO 2 // Joystick Push Button
#define AUDIO_IN 5 // Mic Input
#define CACTUS_BUT 3 // Cactus Button

// Variables to keep track of inputs
float JOY_X;
float JOY_Y;
double AUD_VOLTS;
bool GAME_ENDED;
bool ADD_CACTUS;

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
  sUNPAUSE_SENT = 6,
  sGAME_ENDED = 7
} state;

void updateInputs();
