def packet_from_serial_into_list():
    import serial

    from signal import signal, SIGINT
    from sys import exit
    ser=serial.Serial(port="/dev/ttyACM0", baudrate=115200)
    read_values=""
    def handler(signal_received, frame):
        # Handle any cleanup here
        print('SIGINT or CTRL-C detected. Exiting gracefully')
        
        ser.close()
        exit(0)

    def receive():       
        print(ser.read().decode(), end="")
        
    def send(read_values):
        read_list=read_values.split("_")
        return read_list



    signal(SIGINT, handler)   
    while not read_values:
        receive()
    send(read_values)

