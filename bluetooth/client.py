#!/usr/bin/env python
from gi.repository import Gtk
from gi.repository import GObject
from bt_lib import bt_lib
from threading import Thread

GObject.threads_init()


class bt_client():

    def __init__(self):
        """
        Initialization of bluetooth client
        """
        self.bt_guy = bt_lib()

    def run(self):
        """
        Tun the thread
        """
        while True:
            return self.bt_guy.data_read()

    def connect(self):
        """
        This is the connection
        """
        if self.bt_guy.connect():
            return True
        else:
            return False

    def disconnect(self):
        """
        This is the disconnect happens
        """
        if self.bt_guy.disconnect():
            return True
        else:
            return False


class bluetooth_gtk_client(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(
            self,
            title="Bluetooth client"
        )
        self.set_size_request(600, 400)
        self.bt_guy = bt_client()

        # Grid
        self.grid = Gtk.Grid()
        self.add(self.grid)

        self.create_buttons()
        self.create_textview()

    def create_textview(self):
        """
        Text view to log the data transfer
        """
        scrolledwindow = Gtk.ScrolledWindow()
        scrolledwindow.set_hexpand(True)
        scrolledwindow.set_vexpand(True)
        self.grid.attach(
            scrolledwindow,
            2, 0,
            4, 10
        )

        self.textview = Gtk.TextView()
        self.textbuffer = self.textview.get_buffer()
        scrolledwindow.add(self.textview)

    def create_buttons(self):
        """
        Buttons matters
        """
        self.button_connect = Gtk.Button(
            label="Connect"
        )
        self.button_disconnect = Gtk.Button(
            label="Disconnect"
        )
        self.button_start_session = Gtk.Button(
            label="Start session"
        )
        self.button_end_session = Gtk.Button(
            label="End session"
        )
        self.button_retrieve_data = Gtk.Button(
            label="Receive data"
        )
        # Describing objects
        self.button_connect.set_sensitive(True)
        self.button_connect.set_size_request(100, 10)
        self.button_disconnect.set_sensitive(False)
        self.button_start_session.set_sensitive(False)
        self.button_end_session.set_sensitive(False)
        self.button_retrieve_data.set_sensitive(False)

        # Connecting events
        self.button_connect.connect(
            "clicked",
            self.on_button_connect_clicked
        )
        self.button_disconnect.connect(
            "clicked",
            self.on_button_disconnect_clicked
        )
        self.button_start_session.connect(
            "clicked",
            self.on_button_start_session_clicked
        )
        self.button_end_session.connect(
            "clicked",
            self.on_button_end_session_clicked
        )
        self.button_retrieve_data.connect(
            "clicked",
            self.on_button_retrieve_data_clicked
        )

        # Attaching objects to the grid
        self.grid.add(self.button_connect)
        self.grid.attach_next_to(
            self.button_disconnect,
            self.button_connect,
            Gtk.PositionType.BOTTOM,
            2,
            1
        )
        self.grid.attach_next_to(
            self.button_start_session,
            self.button_disconnect,
            Gtk.PositionType.BOTTOM,
            2,
            1
        )
        self.grid.attach_next_to(
            self.button_end_session,
            self.button_start_session,
            Gtk.PositionType.BOTTOM,
            2,
            1
        )
        self.grid.attach_next_to(
            self.button_retrieve_data,
            self.button_end_session,
            Gtk.PositionType.BOTTOM,
            2,
            1
        )

    def on_button_connect_clicked(self, widget):
        """
        Starting connection with bluetooth device
        """
        Thread(target=self.bt_guy.connect()).start()
        self.button_disconnect.set_sensitive(True)
        self.button_start_session.set_sensitive(True)
        self.button_connect.set_sensitive(False)

    def on_button_disconnect_clicked(self, widget):
        """
        Ending connection with bluetooth device
        """
        Thread(target=self.bt_guy.disconnect()).start()
        self.button_connect.set_sensitive(True)
        self.button_disconnect.set_sensitive(False)
        self.button_start_session.set_sensitive(False)

    def on_button_start_session_clicked(self, widget):
        """
        Starting session with bluetooth device
        """
        print "Start session button clicked"
        self.button_end_session.set_sensitive(True)
        self.button_start_session.set_sensitive(False)
        self.button_disconnect.set_sensitive(False)

    def on_button_end_session_clicked(self, widget):
        """
        Ending session with bluetooth device
        """
        print "End session button clicked"
        self.button_start_session.set_sensitive(True)
        self.button_disconnect.set_sensitive(True)
        self.button_retrieve_data.set_sensitive(True)
        self.button_end_session.set_sensitive(False)

    def on_button_retrieve_data_clicked(self, widget):
        """
        Getting all the data from the memory
        """
        print "Getting all the data from the device"

    def add_data_to_buffer(self, _data=None):
        """
        Append the received data to the buffer
        """
        self.textbuffer = self.textview.get_buffer()
        self.textbuffer.insert_at_cursor(_data)
        # self.textbuffer.insert_at_cursor(_data)


if __name__ == '__main__':
    win = bluetooth_gtk_client()
    win.connect("delete-event", Gtk.main_quit)
    win.show_all()
    Gtk.main()
