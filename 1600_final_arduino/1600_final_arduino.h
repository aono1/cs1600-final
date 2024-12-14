// Input pins
#define JOYS_VRX_DIO A4 // Player Joystick X-Axis
#define JOYS_SW_DIO 2 // Player Joystick Push Button
#define AUDIO_IN A0 // Mic Input
#define JOY_X_CAP A2 // Sabotage Joystick X-Axis
#define JOY_SW_CAP 3 // Sabotage Joystick Push Button

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
const float AUDIO_SENSITIVITY = 2.0; // increase in case of noisy environment 
const int DUCK_SENSITIVITY = 600;

// Variables to keep track of inputs updated in updateInputs()
float JOY_X; // Player Joystick X-Axis
double AUD_VOLTS; // Mic Input
float CAP_JOY_X; // Sabotage Joystick X-Axis

// Sampling Variables
const int sampleWindow = 100;
unsigned int aud_signal;

// WDT variable
unsigned int WDT_INT;

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

// Watchdog functions
void initWDT();
void petWDT();
void wdtISR();

unsigned int getNextCPUINT(unsigned int start) {
   unsigned int tryInt = start + 1;
      while (tryInt < 32) {
         if (NVIC_GetEnableIRQ((IRQn_Type) tryInt) == 0) {
            return tryInt;
         }
      tryInt++;
   }
}