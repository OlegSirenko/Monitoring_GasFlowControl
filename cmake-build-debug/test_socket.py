import socket
import time
import numpy as np

class TestSystem:
    def __init__(self, K, tau, theta):
        self.K = K
        self.tau = tau
        self.theta = theta
        self.y = 0
        self.prev_t = 0
        self.prev_u = 0

    def fopdt_model(self, t, u):
        dt = t - self.prev_t
        if t >= self.theta:
            dydt = (self.K * (u - self.prev_u) - self.y) / self.tau
            self.y += dydt * dt
            self.prev_u = u
        self.prev_t = t
        return self.y

# Create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the server address and port
server_address = ('localhost', 12000)  # replace with your server's IP and port

# Connect the socket to the server
s.connect(server_address)

# Initialize the FOPDT model parameters
K = 100  # process gain
tau = 1  # time constant
theta = 0.1  # dead time
t = 0  # initial time
u = 0  # initial input

# Initialize the test system
system = TestSystem(K, tau, theta)

try:
    s.sendall(str(0).encode('utf-8'))
    time.sleep(4)
    while True:
        # Get the current output from the FOPDT model
        y = system.fopdt_model(t, u)

        # Send the data
        s.sendall(str(y).encode('utf-8'))

        # Update the input and time
        u += 0.0001  # slower rate of increase
        t += 0.01

        # Wait for 0.01 seconds
        time.sleep(0.01)

finally:
    # Close the socket
    s.close()
