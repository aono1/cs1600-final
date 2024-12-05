# cs1600-final

dinosaur game from harsitbaral

# Wiring Instructions
JOYSTICK:
- VRX to A0
- VRY to A1
- SW to D2
- GND to ground, 5V to 5V
MIC to A5
CACTUS BUTTON to D3

# Instructions for using ButtonExpt 
(This makes the dino jump when you hit the button. It should also work for ducking, I just didn't bother to make it because this is enough to prove it works.)
- pip install pyserial
- Wire up a button to D2. You can use the circuit from Lab 1 except with D2
- Upload code in arduino_buttonExpt to Arduino IDE
    - DO NOT open the Serial Monitor. If you get "resource busy" error on the port at any point, it's probably because you opened serial monitor. Arduino communicates with Python through the Serial Monitor, so if you also open in Arduino IDE, the serial monitor will be taken up and can't talk to Python.
- Run buttonExpt.py. Jump by hitting the button.
    - If you get an error that the port isn't found, update the port string on line 15 of buttonExpt.py to your Arduino's port number. Get this by going to the Arduino IDE > Tools > Port.