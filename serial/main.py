#/usr/bin/env python2
import serial

# Configuration
serial_port = '/dev/ttyACM1'
serial_baud = 19200

ser = serial.Serial(
    port=serial_port,
    baudrate=serial_baud,
)

# try:
#     ser.open()
# except serial.serialutil.SerialException:
#     print "Port is taken, stupid, trying to close, run program again"
#     ser.close()
#     exit()

ser.isOpen()

try:
    while 1:
        out = ''
        while ser.inWaiting() > 0:
            out = ser.read(1)
            print bytes(out)


except KeyboardInterrupt:
    print "KeyboardInterrupt, closing port"
    ser.close()
    exit()
