from bluetooth import *

device = '00:12:6F:25:B5:2C'

port = 1

socket = BluetoothSocket(RFCOMM)
print "Connecting.."
socket.connect((device, port))

data = ''
try:
    print "Receiving data.."
    while True:
        try:
            socket.write(0xFF)
            # data = socket.recv(255)
        except bluetooth.BluetoothError, b:
            print "Bluetooth Error: ", b

        if len(data) > 0:
            print data

except KeyboardInterrupt:
    print "Closing socket...",
    socket.close()
    #print "done."


# class rfcomm_client():

#     def __init__(self):
#         """
#         Initialization of rfcomm client
#         """


# class rfcomm_server():

#     def __init__(self):
#         """
#         Initialization
#         """
#         self.uuid = '4201B1B2-C470-47F1-9437-52EAD247ED20'
#         self.server_sock = BluetoothSocket(RFCOMM)
#         self.server_sock.bind(("", PORT_ANY))
#         self.server_sock.listen(1)
#         self.port = self.server_sock.getsockname()[1]

#     def search(self):
#         """
#         Searching for the devices, which has `patric`
#         name
#         """
#         self.nearly_devices = discover_devices(
#             lookup_names=True
#         )
#         if len(self.nearly_devices) > 0:
#             for bdaddr, name in self.nearly_devices:
#                 if name == 'patric':
#                     self.to_address = bdaddr
#                     return True

#         return False

#     def connect(self):
#         """
#         Connecting to the device
#         """
#         if self.search:
#             advertise_service(
#                 self.server_sock,
#                 "SampleServer",
#                 service_id=self.uuid,
#                 service_classes=[self.uuid, SERIAL_PORT_CLASS],
#                 profiles=[SERIAL_PORT_PROFILE]
#             )
#             print "Waiting for connection on RCOMM channel %d" % self.port
#             self.client_sock, self.client_info = self.server_sock.accept()
#             print "Connection accepted from ", self.client_info
#             try:
#                 while True:
#                     data = self.client_sock.recv(1024)
#                     if len(data) == 0:
#                         break
#                     print "received [%s]" % data
#             except IOError:
#                 print "IOError"
#             print "Disconnected"
#             self.client_sock.close()
#             self.server_sock.close()
#             return True
#         else:
#             return False

# if __name__ == '__main__':
#     rc = rfcomm_server()
#     rc.connect()
