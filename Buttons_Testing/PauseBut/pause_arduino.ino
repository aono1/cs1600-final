const int jumpButtonPin = 2;          // Jump button connected to pin 2
const int pauseButtonPin = 3;         // Pause/Restart button connected to pin 3 (interrupt pin)
volatile bool pauseButtonPressed = false; // Flag for pause button state
bool gamePaused = false;              // Tracks if the game is currently paused
int jumpButtonState = HIGH;           // Variable to store jump button state

void setup() {
    pinMode(jumpButtonPin, INPUT_PULLUP);    // Set jump button with internal pull-up
    pinMode(pauseButtonPin, INPUT_PULLUP);   // Set pause button with internal pull-up
    Serial.begin(9600);                      // Initialize serial communication
    attachInterrupt(digitalPinToInterrupt(pauseButtonPin), pauseISR, RISING);  // Attach ISR to pause button
}

// ISR function to set the flag when the pause button is pressed
void pauseISR() {
    pauseButtonPressed = true;               // Set flag to indicate pause button was pressed
    // Based on all online searches, it is not good practice to print within an ISR.
    // I was getting the single character printing bug from Lab 3, and decided to just use best practice
    // instead of fixing it as Lab 3 did. 
}

void loop() {
    // Check if pause button was pressed
    if (pauseButtonPressed) {
        // Toggle game paused state
        gamePaused = !gamePaused;
        pauseButtonPressed = false;          // Reset the flag

        // Send appropriate message based on game state
        if (gamePaused) {
            Serial.println("PAUSE");         // Send "PAUSE" when the game is paused
        } else {
            Serial.println("RESTART");       // Send "RESTART" when the game resumes
        }
        delay(50); // Small debounce delay
    }

    // Check jump button state only if the game is not paused
    if (!gamePaused) {
        jumpButtonState = digitalRead(jumpButtonPin);
        if (jumpButtonState == HIGH) {        // Button is pressed (assuming active-low)
            Serial.println("JUMP");          // Send "JUMP" command
            delay(200);                      // Debounce delay to avoid multiple triggers
        }
    }
}
