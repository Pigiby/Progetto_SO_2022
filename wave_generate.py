import serial
serialport = serial.Serial('/dev/ttyACM0',baudrate=19200,timeout=2)
while(1):
    arduinodata = serialport.readline()
    print(arduinodata)