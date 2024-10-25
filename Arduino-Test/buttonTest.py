import pyfirmata
import time

board = pyfirmata.Arduino('/dev/cu.usbmodemC04E301141D82')

while True:
    board.digital[13].write(1)
    # time.sleep(1)
    # board.digital[13].write(0)
    # time.sleep(1)



# import pyfirmata
# import time

# # Specify the board and port
# board = pyfirmata.Arduino('/dev/cu.usbmodemC04E301141D82')

# # Define the button pin
# BUTTON_PIN = 5

# # Start an iterator to avoid buffer overflow
# it = pyfirmata.util.Iterator(board)
# it.start()

# # Give the board some time to initialize
# time.sleep(1)

# # Enable reporting on the digital pin
# board.digital[BUTTON_PIN].mode = pyfirmata.INPUT
# board.digital[BUTTON_PIN].enable_reporting()

# while True:
#     # Read the button state
#     button_state = board.digital[BUTTON_PIN].read()
    
#     # Print the button state (None means uninitialized; otherwise True/False for HIGH/LOW)
#     print(button_state)
    
#     time.sleep(0.1)  # Small delay to prevent flooding the output