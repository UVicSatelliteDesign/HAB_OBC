import serial
ser=serial.Serial(port="COM3", baudrate=9600)
def receive():
    try:
        
        if ser.in_waiting!=0:
            print(ser.readline().decode())
    except:
        print(Exception)
    finally:
        ser.close()
def send():
    try:
        
        inp=input("Enter string to send")
        ser.write(inp.encode())
    except:
        print(Exception)
    finally:
        ser.close()
while True:
    print(ser.readline().decode())
