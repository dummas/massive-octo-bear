#/usr/bin/env python2
import serial

# Configuration
serial_port = '/dev/ttyACM0'
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
        out = 0x00
        while ser.inWaiting() > 0:
            out = ser.read(1)
            print ">> " + out.encode('hex')


except KeyboardInterrupt:
    print "KeyboardInterrupt, closing port"
    ser.close()
    exit()
