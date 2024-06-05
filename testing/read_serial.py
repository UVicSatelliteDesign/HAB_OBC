import serial

def receive():
    try:
        ser=serial.Serial(port="COM3", baudrate=9600)
        if ser.in_waiting!=0:
            print(ser.readline().decode())
    except:
        print(Exception)
    finally:
        ser.close()
def send():
    try:
        ser=serial.Serial(port="COM3", baudrate=9600)
        inp=input("Enter string to send")
        ser.write(inp.encode())
    except:
        print(Exception)
    finally:
        ser.close()
