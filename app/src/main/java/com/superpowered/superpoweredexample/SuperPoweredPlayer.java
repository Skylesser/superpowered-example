package com.superpowered.superpoweredexample;


import android.content.Context;
import android.content.ContextWrapper;
import android.media.AudioManager;
import android.os.Build;

import java.io.File;

class SuperPoweredPlayer
{
	
	public static final String URL_A = "http://wezeejay.fr/ws/metier/test/test1.mp3";
	public static final String URL_B = "http://wezeejay.fr/ws/metier/test/test2.mp3";
	public static final String URL_C = "http://wezeejay.fr/ws/metier/test/test3.mp3";
	
	static
	{
		System.loadLibrary("SuperpoweredExample");
	}
	
	public SuperPoweredPlayer(Context context)
	{
		// Get the device's sample rate and buffer size to enable low-latency Android audio output, if available :
		String samplerateString = null;
		String buffersizeString = null;
		if (Build.VERSION.SDK_INT >= 17)
		{
			AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
			if (audioManager != null)
			{
				samplerateString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
				buffersizeString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
			}
		}
		if (samplerateString == null)
		{
			samplerateString = "44100";
		}
		if (buffersizeString == null)
		{
			buffersizeString = "256";
		}
		int sampleRate = Integer.parseInt(samplerateString);
		int bufferSize = Integer.parseInt(buffersizeString);
		
		// création répertoire sauvegarde :
		ContextWrapper contextWrapper = new ContextWrapper(context);
		File repertoire = contextWrapper.getDir("mp3", Context.MODE_PRIVATE);
		if (!repertoire.exists())
		{
			repertoire.mkdirs();
		}
		
		// création lecteur :
		SuperpoweredExample(sampleRate, bufferSize, URL_A, URL_B, URL_C, repertoire.getAbsolutePath());
	}
	
	void openMp3()
	{
		open();
	}
	
	void playMp3()
	{
		play();
	}
	
	private native void SuperpoweredExample(int samplerate, int buffersize, String urlA, String urlB, String urlC, String localPath);
	private native void open();
	private native void play();
	
}
