import socket
import time
import math

def receive_initial_message(s):
    # Receive the initial datetime string from the server
    datetime_string = s.recv(1024).decode('utf-8')
    print('Received datetime from server:', datetime_string)

def send_and_receive(s, data):
    # Send the data and receive the PID output
    s.sendall(str(data).encode('utf-8'))
    response = s.recv(1024)
    return float(response.decode('utf-8'))

# Define the server address and port
server_address = ('localhost', 12000)  # replace with your server's IP and port

# Temperature simulation parameters
ambient_temperature = 25.0  # Ambient temperature in degrees Celsius
current_temperature = ambient_temperature
heating_power = 0.0

# Create a TCP/IP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(server_address)

try:
    # Receive the initial datetime string from the server
    receive_initial_message(s)

    while True:
        # Simulate the effect of heating power on the temperature
        current_temperature += (heating_power - (current_temperature - ambient_temperature) * 0.01) * 0.001

        # Use the common function to send the current temperature and receive PID output
        heating_power = send_and_receive(s, current_temperature)
        print(f'Current Temperature: {current_temperature:.2f} °C, PID Output (Heating Power): {heating_power:.2f}')

finally:
    s.close()
    print("Client script finished.")
