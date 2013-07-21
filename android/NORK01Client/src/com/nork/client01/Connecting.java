package com.nork.client01;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

public class Connecting implements Runnable {
	
	private final BluetoothSocket mmSocket;
	private final BluetoothDevice mmDevice;
	private static final String TAG = "ConnectThread";
	private final UUID APP_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	BluetoothAdapter mBluetoothAdapter = null;
	private Connected connected = null;
	
	/**
	 * Thread constructor
	 * @param device
	 * @throws NoSuchMethodException 
	 * @throws InvocationTargetException 
	 * @throws IllegalAccessException 
	 * @throws IllegalArgumentException 
	 */
	public Connecting(BluetoothDevice device) {
		
		Log.d(TAG, "Connect thread constructor");
		BluetoothSocket tmp = null;
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		mmDevice = device;
		try {
			Log.d(TAG, "Create rfcomm socket");
			tmp = mmDevice.createRfcommSocketToServiceRecord(APP_UUID);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		Method m = null;
		try {
			Log.d(TAG, "Module part");
			m = mmDevice.getClass().getMethod("createRfcommSocket", new Class[] {int.class});
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
        try {
        	Log.d(TAG, "Invoke part");
			tmp = (BluetoothSocket) m.invoke(device, 1);
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
        Log.d(TAG, "Passing socket part");
		mmSocket = tmp;
	}
	
	/**
	 * Launching thread
	 */
	public synchronized void run() {
		mBluetoothAdapter.cancelDiscovery();
		
		try {
			mmSocket.connect();
		} catch (IOException connectException) {
			connectException.printStackTrace();
			try {
				mmSocket.close();
			} catch (IOException closeException) {
				closeException.printStackTrace();
			}
			return;
		}
			
	}
	
	public BluetoothSocket getSocket() {
		return this.mmSocket;
	}
	
	/**
	 * Cancel the operation
	 */
	public void cancel() {
		try {
			connected.cancel();
			mmSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}