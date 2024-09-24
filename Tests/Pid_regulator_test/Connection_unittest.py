import socket
import time
import subprocess
import os
import random
import shlex

def receive_initial_message(s):
    # Receive the initial datetime string from the server
    datetime_string = s.recv(1024).decode('utf-8')
    print('Received datetime from server:', datetime_string)

def send_and_receive(s, data):
    # Send the data and receive the response
    s.sendall(str(data).encode('utf-8'))
    resp = s.recv(1024)
    return float(resp.decode('utf-8'))

# Define the server address and port
server_address = ('localhost', 12000)  # replace with your server's IP and port

# Ensure the executable path is trusted and validated
build_dir = os.path.join(os.getcwd())
if os.name == 'nt':  # Windows
    executable_path = os.path.join(build_dir, 'Release', 'Monitor.exe')
    server_process = subprocess.Popen([executable_path], creationflags=subprocess.CREATE_NEW_CONSOLE)
else:  # Unix-based systems
    executable_path = os.path.join(build_dir, 'Monitor')
    server_process = subprocess.Popen([shlex.quote(executable_path)])

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
    print(f"Sent: {test_value}, Received: {response} 'couse the PID regulation is not enabled by default")

    # Check if the sent value is the same as the received value
    assert 0 == response, f"Test failed: Sent {0}, but received {response}"
    print("Test passed: Sent value is the same as received value.")
finally:
    s.close()
    server_process.terminate()
    print("Client script finished.")
