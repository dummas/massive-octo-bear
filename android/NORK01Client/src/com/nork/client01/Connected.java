package com.nork.client01;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;

import android.bluetooth.BluetoothSocket;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.EditText;

public class Connected implements Runnable {
	private static final String TAG = "ConnectedThread";
	private final BluetoothSocket bluetoothSocket;
	private final InputStream inputStream;
	private final OutputStream outputStream;
	private int state;
	private static final int HANDSHAKE_STATE = 0;
	private static final int SAMPLE_SENT_STATE = 1;
	private static final int SAMPLE_RECV_STATE = 2;
	private File file = new File(Environment.getExternalStorageDirectory(), "sample.output");
	private OutputStream fileOutputStream = null;
	Handler handler = new Handler();
	
	/**
	 * Connection manager constructor
	 * @param socket
	 */
	public Connected(BluetoothSocket socket, Handler handler) {
		this.handler = handler;
		Log.d(TAG, "Hello from connected thread");
		
		bluetoothSocket = socket;
		InputStream tmpIn = null;
		OutputStream tmpOut = null;
		
		// Get the input and output streams, using temp objects, necause member streams are final
		try {
			tmpIn = socket.getInputStream();
			tmpOut = socket.getOutputStream();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		setState(HANDSHAKE_STATE);
		
		inputStream = tmpIn;
		outputStream = tmpOut;
		try {
			fileOutputStream = new BufferedOutputStream(new FileOutputStream(file));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Running the thread
	 * 
	 * Logic.
	 * 
	 * First thing, is to perform the handshake.
	 * 
	 * Handshake is performed by sending and receiving the 'H' letter.
	 * 
	 * After the handshake is performed, the Master sends the 'S' letter, which means `to Sample`.
	 * 
	 * The Slave receives this message and performs the sample from all the channels it has. After it finishes the sampling process, the result is sent back to the Master.
	 * 
	 * Master record the time then the sample was received and stores the result.
	 * 
	 */
	public synchronized void run() {
		byte[] buffer;
		byte[] raw_buffer;
		int received_buffer_length;
		
		/**
		 * Hand shake
		 */
		this.write("H".getBytes());
		
		while(true) {
			try {
				raw_buffer = new byte[1024];
				received_buffer_length = inputStream.read(raw_buffer);
				buffer = new byte[received_buffer_length];
				System.arraycopy(raw_buffer, 0, buffer, 0, received_buffer_length);
				
				/**
				 * If the handshake was received
				 */
				if (getState() == HANDSHAKE_STATE) {
					if (buffer[0] == 'H') {
						/*
						 * Handshake was okay, send the sample message
						 */
						Log.d(TAG, "Handshake okay");
						setState(SAMPLE_SENT_STATE);
						this.write("S".getBytes());
					}
				}
				if (getState() == SAMPLE_SENT_STATE) {
					/*
					 * Data received
					 */
					Log.d(TAG, "Data received");
					StringBuilder data = new StringBuilder();
					
					for (byte b: buffer) {
						data.append(String.format("%02X", b));
					}
					
					/*
					 * Update the UI
					 */
					Message message = handler.obtainMessage();
					message.obj = data.toString();
					handler.sendMessage(message);
					/*
					 * Send more !
					 */
//					this.write("S".getBytes());
				}
			} catch (IOException e) {
				e.printStackTrace();
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
			outputStream.write(bytes);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Cancel the thread and closing the socket 
	 */
	public void cancel() {
		try {
			bluetoothSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public int getState() {
		return state;
	}

	public void setState(int state) {
		this.state = state;
	}
}