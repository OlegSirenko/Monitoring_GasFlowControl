import socket
import time

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
ambient_temperature = 24  # Ambient temperature in degrees Celsius
current_temperature = 24 
heating_power = 0.0
thermal_inertia = 0.025  # Time constant for thermal inertia

# Create a TCP/IP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(server_address)

try:
    # Receive the initial datetime string from the server
    receive_initial_message(s)

    while True:
        # Simulate the effect of heating power on the temperature with thermal inertia
        current_temperature += (heating_power - (current_temperature - ambient_temperature) * 0.1) * 0.1
        current_temperature += (heating_power - current_temperature) * thermal_inertia
        
        if current_temperature < ambient_temperature:
            current_temperature = ambient_temperature
        
        # Use the common function to send the current temperature and receive PID output
        heating_power = send_and_receive(s, current_temperature)
        if heating_power < 0:
            heating_power = 0
        elif heating_power > 256:
            heating_power = 256
        print(f'Current Temperature: {current_temperature:.2f} °C, PID Output (Heating Power): {heating_power:.2f}')

        # Add a small delay to simulate real-time processing
        #time.sleep(1)

finally:
    s.close()
    print("Client script finished.")

