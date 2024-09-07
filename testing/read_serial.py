import serial

from signal import signal, SIGINT
from sys import exit
ser=serial.Serial(port="/dev/ttyACM0", baudrate=115200)
read_value=""
def handler(signal_received, frame):
    # Handle any cleanup here
    print('SIGINT or CTRL-C detected. Exiting gracefully')
    
    ser.close()
    exit(0)

def receive():
    
        
    print(ser.read().decode(), end="")

def send():
    return read_value    
        
    



signal(SIGINT, handler)   
while not read_value:
    receive()

