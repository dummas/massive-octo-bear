import bluetooth

print "Starting scan..."
nearly_devices = bluetooth.discover_devices()

print "Printing found devices:"
for bdaddr in nearly_devices:
    print bluetooth.lookup_name(bdaddr)
