package com.superpowered.superpoweredexample;


import android.content.Context;
import android.content.ContextWrapper;

import java.io.File;

class SuperPoweredPlayer
{
	
	private static final String URL = "http://wezeejay.fr/ws/metier/test/test2.mp3";
	
	static
	{
		System.loadLibrary("SuperpoweredExample");
	}
	
	private boolean playState = false;
	
	public SuperPoweredPlayer(Context context)
	{
		
		final int sampleRate = 48000;
		final int bufferSize = 512;
		
		ContextWrapper contextWrapper = new ContextWrapper(context);
		File repertoire = contextWrapper.getDir("mp3", Context.MODE_PRIVATE);
		if (!repertoire.exists())
		{
			repertoire.mkdirs();
		}
		
		SuperpoweredExample(sampleRate, bufferSize, URL, repertoire.getAbsolutePath());
	}
	
	void startPlayback()
	{
		playState = true;
		onPlayPause(true);
	}
	
	void open()
	{
		open(URL);
	}
	
	void togglePlayback()
	{
		playState = !playState;
		onPlayPause(playState);
	}
	
	private native void SuperpoweredExample(int samplerate, int buffersize, String url, String localPath);
	private native void onPlayPause(boolean play);
	private native void open(String url);
	
}
