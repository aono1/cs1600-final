const int buttonPin = 2;
int buttonState = 0;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.begin(9600);
    delay(1000);
    Serial.println("Prepped");
}

void loop() {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) { 
        Serial.println("JUMP");
        delay(200); // Debounce delay to prevent multiple triggers
    }
}