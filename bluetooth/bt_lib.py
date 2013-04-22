from bluetooth import *


class bt_lib():

    def __init__(self):
        """
        Initialization of the class
        """
        # Default configuration
        # Debug device configuration
        self.device = '00:12:6F:25:B5:2C'
        self.port = 1

        self.socket = BluetoothSocket(RFCOMM)

    def connect(self):
        """
        Connecting to the bluetooth device
        """
        return self.socket.connect((
            self.device,
            self.port
        ))

    def disconnect(self):
        """
        Disconnecting from the bluetooth device
        """
        return self.socket.close()

    def configuration_set_device(self, _device=None):
        """
        Configuration matters

        Setting the device
        """
        self.device = _device

    def configuration_set_port(self, _port=None):
        """
        Configuration matters

        Setting the reading port
        """
        self.port = _port

    def data_read(self):
        try:
            data = self.reading_socket.recv(255)
        except BluetoothError, b:
            print "Bluetooth error: ", b
            self.disconnect()
        if len(data) > 0:
            return data

    def data_write(self, _data=None):
        try:
            self.writing_socket.write(_data)
        except BluetoothError, b:
            print "Bluetooth error: ", b
            self.disconnect()
