import socket
import time
import math

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the server address and port
server_address = ('localhost', 12000)  # replace with your server's IP and port

# Connect the socket to the server
s.connect(server_address)

x = 0

try:
    s.sendall(str(0).encode('utf-8'))
    time.sleep(4)
    while True:
        # Get the current output from the FOPDT model
        x += 0.1

        # Send the data
        s.sendall(str(math.sin(x)).encode('utf-8'))

        # Update the input and time

        # Wait for 0.01 seconds
        time.sleep(0.01)

finally:
    # Close the socket
    s.close()
