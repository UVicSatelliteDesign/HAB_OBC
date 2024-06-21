import serial
ser=serial.Serial(port="COM3", baudrate=115200)
def receive():
    try:
        
       while True:
            print(ser.readline().decode()) 
    except:
        print(Exception)
    finally:
        ser.close()
def send():
    try:
        
        inp=input("Enter string to send")
        while True:
            ser.write(inp.encode())
    except:
        print(Exception)
    finally:
        ser.close()


