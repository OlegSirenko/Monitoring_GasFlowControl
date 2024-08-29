import socket
import time
import subprocess
import os
import random

def receive_initial_message(s):
    # Receive the initial datetime string from the server
    datetime_string = s.recv(1024).decode('utf-8')
    print('Received datetime from server:', datetime_string)

def send_and_receive(s, data):
    # Send the data and receive the response
    s.sendall(str(data).encode('utf-8'))
    response = s.recv(1024)
    return float(response.decode('utf-8'))

# Define the server address and port
server_address = ('localhost', 12000)  # replace with your server's IP and port

# Start the server executable
if os.name == 'nt':  # Windows
    server_process = subprocess.Popen(['GasFlowControlMonitoringApp.exe'], creationflags=subprocess.CREATE_NEW_CONSOLE)
else:  # Unix-based systems
    server_process = subprocess.Popen(['./GasFlowControlMonitoringApp'])

# Give the server some time to start
time.sleep(5)

# Create a TCP/IP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(server_address)

try:
    # Receive and ignore the initial datetime string from the server
    receive_initial_message(s)

    # Generate a random value to send
    test_value = random.randint(1, 100)  # Example range: 1 to 100
    response = send_and_receive(s, test_value)
    print(f'Sent: {test_value}, Received: {response}')

    # Check if the sent value is the same as the received value
    assert test_value == response, f"Test failed: Sent {test_value}, but received {response}"
    print("Test passed: Sent value is the same as received value.")
finally:
    s.close()
    server_process.terminate()
    print("Client script finished.")