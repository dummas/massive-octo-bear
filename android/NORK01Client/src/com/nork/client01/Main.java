package com.nork.client01;

import java.util.Set;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class Main extends Activity {

    private static final int REQUEST_ENABLE_BT = 0;
    private static final int BLUETOOTH_NOT_AVAILABLE = 1;
	private static final int BLUETOOTH_NO_PAIRED_DEVICES = 2;
	private static final int BLUETOOTH_DEVICE_NOT_FOUND = 3;
    Button button_connect = null;
    Button button_disconnect = null;
    Button button_start = null;
    Button button_stop = null;
    Button button_exit = null;
    EditText editText = null;

    BluetoothAdapter mBluetoothAdapter = null;
    BluetoothDevice device_device = null;
    BluetoothSocket mmSocket = null;
    Connecting device_connection = null;
    Connected device_connection_manager = null;
	protected Connected connected;
    private static final String TAG = "MainActivity";
    private static final String DEVICE_NAME = "patric";
    
    Thread connection_thread;
    Thread connected_thread;
    
    protected Handler handler = new Handler() {
    	public void handleMessage(Message msg) {
    		Log.d(TAG, "ATEJO MSG");
    		Log.d(TAG, (String)msg.obj);
    		editText.append((String)msg.obj);
    		editText.append("\n");
    	}
    };

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
        button_start = (Button) findViewById(R.id.button_start);
        button_stop = (Button) findViewById(R.id.button_stop);
        button_disconnect = (Button) findViewById(R.id.button_disconnect);
        button_exit = (Button) findViewById(R.id.button_exit);
        editText = (EditText) findViewById(R.id.editText1);
        
        button_connect.setOnClickListener(new View.OnClickListener() {
        	
	    	@Override
	    	public void onClick(View v) {
	    		/**
	    		 * Connect action sequence
	    		 */
	    		Log.d(TAG, "Hello from connect button");
	    		if (device_connection == null) {
	    			/**
	    			 * Run device connect
	    			 */
	    			device_connection = new Connecting(device_device);
	    			connection_thread = new Thread(device_connection);
	    			connection_thread.start();
	    		}
	    	}
        });
        
        button_start.setOnClickListener(new View.OnClickListener() {
        	
			@Override
			public void onClick(View v) {
				/**
				 * Starting the communication sequence
				 */
				Log.d(TAG, "Hello from start button");
				if (device_connection != null) {
					/**
					 * Pass it to connected thread
					 */
					try {
						connection_thread.join();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					connected = new Connected(device_connection.getSocket(), handler);
					connected_thread = new Thread(connected);
					connected_thread.start();
				}
			}
		});
        
        button_stop.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Log.d(TAG, "Hello from stop button");
				if (connected_thread != null) {
					connected_thread.stop();
				}
				if (connected != null) {
					connected.cancel();
				}
			}
		});
        
        button_disconnect.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Log.d(TAG, "Hello from disconnect button");
				if (connection_thread != null) {
					connection_thread.stop();
				}
				/**
				 * Disconnect action sequence
				 */
				if (device_connection != null) {
					device_connection.cancel();
					device_connection = null;
				}
			}
		});
        
        button_exit.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.d(TAG, "Hello from exit button");
				
				if (connected_thread != null) {
					try {
						connected_thread.join();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					connected_thread.interrupt();
				}
				
				if (connection_thread != null) {
					try {
						connection_thread.join();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					connection_thread.interrupt();
				}

				if (device_connection != null) {
					device_connection.cancel();
					device_connection = null;
				}
				finish();
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
    			Log.d(TAG, "Paired device: " + device.getName());
    			if (device.getName().equals(DEVICE_NAME)) {
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
    		alert_dialog = new AlertDialog.Builder(Main.this).create();
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
    		alert_dialog = new AlertDialog.Builder(Main.this).create();
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
    		alert_dialog = new AlertDialog.Builder(Main.this).create();
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
}
