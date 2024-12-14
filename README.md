# cs1600-final

dinosaur game from harsitbaral

# Wiring Instructions
PLAYER JOYSTICK:
- VRX to A4
- VRY to A1
- SW to D2

MIC to A0

CACTUS BUTTON to D3

SABOTAGE JOYSTICK:
- VRX to A2
- VRY to A3
- SW to D3

# Device-Specific Error Debugging Info
- DO NOT open the Serial Monitor. If you get "resource busy" error on the port at any point, it's probably because you opened serial monitor. Arduino communicates with Python through the Serial Monitor, so if you also open in Arduino IDE, the serial monitor will be taken up and can't talk to Python.
- If you get an error that the port isn't found, update the port string on line 17 of main.py to your Arduino's port number. Get this by going to the Arduino IDE > Tools > Port.
