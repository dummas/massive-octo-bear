#/usr/bin/env python2
import serial
from datetime import datetime

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
word = ''
cr_happend = False
lr_happend = False

try:
    while 1:
        out = ''
        while ser.inWaiting() > 0:
            out = ser.read()
            # print bytes(out)
            # print out.encode('hex')
            if out.encode('hex') != '00':
                if out.encode('hex') == '0d':
                    cr_happend = True
                if out.encode('hex') == '0a':
                    lr_happend = True
                if cr_happend and lr_happend:
                    # print "****************"
                    print datetime.now().isoformat()
                    cr_happend = False
                    lr_happend = False
                    print bytes(word)
                    print word.encode('hex')
                    word = ''
                    # print "****************"
                else:
                    word += out
                # print bytes(out)
                # print out.encode('hex')


except KeyboardInterrupt:
    print "KeyboardInterrupt, closing port"
    ser.close()
    exit()
