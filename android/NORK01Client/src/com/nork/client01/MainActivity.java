package com.nork.client01;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    private static final int REQUEST_ENABLE_BT = 1;
    private static final int BLUETOOTH_NOT_AVAILABLE = 0;
	private static final int BLUETOOTH_NO_PAIRED_DEVICES = 1;
	private static final int BLUETOOTH_DEVICE_NOT_FOUND = 2;
    Button button_connect = null;
    Button button_disconnect = null;
    BluetoothAdapter mBluetoothAdapter = null;
    BluetoothDevice device_device = null;
    ConnectThread device_connection = null;
    ConnectedThread device_connection_manager = null;
    private static final String TAG = "MainActivity";
    private static final String DEVICE_NAME = "Serial Adapter";
    private final UUID APP_UUID = UUID.fromString("6ccf4af0-ab3f-11e2-9e96-0800200c9a66");
	private final static String SERVER_NAME = "MOJO";


	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Enable Bluetooth
        if ( this.enableBluetooth() ) {
        	this.queryBluetooth();
        }
        // Define the buttons
        button_connect = (Button) findViewById(R.id.button_connect);
        button_disconnect = (Button) findViewById(R.id.button_disconnect);
        
        button_connect.setOnClickListener(new View.OnClickListener() {
        	
        	@Override
        	public void onClick(View v) {
        		/**
        		 * Connect action sequence
        		 */
        		if (device_connection == null) {
        			device_connection = new ConnectThread(device_device);
            		device_connection.start();
        		}
        		
        	}
        });
        
        button_disconnect.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				/**
				 * Disconnect action sequence
				 */
				if (device_connection != null) {
					device_connection.cancel();
					device_connection = null;
				}
			}
		});
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    /**
     * Enabling the bluetooth
     * @return
     */
    public boolean enableBluetooth() {
    	
    	mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    	
    	if (mBluetoothAdapter == null) {
        	/**
        	 * Bluetooth is not supported
        	 */
    		this.displayAlert(BLUETOOTH_NOT_AVAILABLE);
    		return false;
        }
        
        if (!mBluetoothAdapter.isEnabled()) {
        	/**
        	 * Send the user to enable the Bluetooth
        	 */
        	Intent enableBluetoothIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        	startActivityForResult(enableBluetoothIntent, REQUEST_ENABLE_BT);
        }
        
        return true;
    	
    }
    
    /**
     * Querying the Bluetooth for devices 
     */
    public void queryBluetooth() {
    	Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
    	if (pairedDevices.size() > 0) {
    		for (BluetoothDevice device : pairedDevices) {
    			Log.d(TAG, device.getName());
    			if (device.getName() == DEVICE_NAME) {
    				device_device = device;
    			}
    		}
    		
    	} else {
    		this.displayAlert(BLUETOOTH_NO_PAIRED_DEVICES);
    	}
    	if (device_device == null) {
    		this.displayAlert(BLUETOOTH_DEVICE_NOT_FOUND);
    	}
    }
    
    /**
     * Displaying the alert message
     * @param alert_id
     */
    public void displayAlert(int alert_id) {
    	AlertDialog alert_dialog = null;
    	switch(alert_id) {
    	case BLUETOOTH_NOT_AVAILABLE:
    		alert_dialog = new AlertDialog.Builder(MainActivity.this).create();
    		alert_dialog.setTitle("Bluetooth error");
    		alert_dialog.setMessage("Bluetooth is not supported");
    		
    		alert_dialog.setButton("Quit", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// Quiting 
					finish();
				}
			});
    		alert_dialog.show();
    		break;
    	case BLUETOOTH_NO_PAIRED_DEVICES:
    		alert_dialog = new AlertDialog.Builder(MainActivity.this).create();
    		alert_dialog.setTitle("Bluetooth error");
    		alert_dialog.setMessage("No paired devices");
    		
    		alert_dialog.setButton("Quit", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// Quiting
					finish();
				}
			});
    		alert_dialog.setButton("Start pairing", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Start pairing
					
				}
			});
    		alert_dialog.show();
    		break;
    	case BLUETOOTH_DEVICE_NOT_FOUND:
    		alert_dialog = new AlertDialog.Builder(MainActivity.this).create();
    		alert_dialog.setTitle("Bluetooth error");
    		alert_dialog.setMessage("Device in question is not paired");
    		
    		alert_dialog.setButton("Quit", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// Quiting
					finish();
				}
			});
    		alert_dialog.setButton("Start pairing", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Start pairing
					
				}
			});
    		alert_dialog.show();
    		break;
    	}
    }
    
    /**
     * Connect Thread
     * @author maksim
     *
     */
    private class ConnectThread extends Thread {
    	
    	private final BluetoothSocket mmSocket;
    	private final BluetoothDevice mmDevice;
    	
    	/**
    	 * Thread constructor
    	 * @param device
    	 */
    	public ConnectThread(BluetoothDevice device) {
    		BluetoothSocket tmp = null;
    		mmDevice = device;
    		try {
    			tmp = mmDevice.createInsecureRfcommSocketToServiceRecord(APP_UUID);
    		} catch (IOException e) {
    			
    		}
    		mmSocket = tmp;
    	}
    	
    	/**
    	 * Launching thread
    	 */
    	public void run() {
    		mBluetoothAdapter.cancelDiscovery();
    		
    		try {
    			mmSocket.connect();
    		} catch (IOException connectException) {
    			try {
    				mmSocket.close();
    			} catch (IOException closeException) {
    				
    			}
    			return;
    		}
    		
    		/**
    		 * Pass it to connected thread
    		 */
    		device_connection_manager = new ConnectedThread(mmSocket);
    		
    	}
    	
    	/**
    	 * Cancel the operation
    	 */
    	public void cancel() {
    		try {
				mmSocket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	
    }
    
    /**
     * Connection Thread Manager
     * @author maksim
     *
     */
    private class ConnectedThread extends Thread {
    	private final BluetoothSocket mmSocket;
    	private final InputStream mmInStream;
    	private final OutputStream mmOutStream;
    	
    	/**
    	 * Connection manager constructor
    	 * @param socket
    	 */
    	public ConnectedThread(BluetoothSocket socket) {
    		mmSocket = socket;
    		InputStream tmpIn = null;
    		OutputStream tmpOut = null;
    		
    		// Get the input and output streams, using temp objects, necause member streams are final
    		try {
    			tmpIn = socket.getInputStream();
    			tmpOut = socket.getOutputStream();
    		} catch (IOException e) {
    			
    		}
    		
    		mmInStream = tmpIn;
    		mmOutStream = tmpOut;
    	}
    	
    	/**
    	 * Running the thread
    	 */
    	public void run() {
    		byte[] buffer = new byte[1024];
    		int bytes;
    		
    		while(true) {
    			try {
    				// Read from the inputStream
    				bytes = mmInStream.read(buffer);
    				// TODO: Send the obtained bytes to the UI activity
    				
    			} catch (IOException e) {
    				break;
    			}
    		}
    	}
    	
    	/**
    	 * Writing to the device
    	 * @param bytes
    	 */
    	public void write(byte[] bytes) {
    		try {
    			mmOutStream.write(bytes);
    		} catch (IOException e) {
    			e.printStackTrace();
    		}
    	}
    	
    	/**
    	 * Cancel the thread and closing the socket 
    	 */
    	public void cancel() {
    		try {
    			mmSocket.close();
    		} catch (IOException e) {
    			e.printStackTrace();
    		}
    	}
    }
}
