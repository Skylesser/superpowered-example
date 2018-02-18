package com.superpowered.superpoweredexample;


import android.content.Context;
import android.content.ContextWrapper;

import java.io.File;

class SuperPoweredPlayer
{
	
	public static final String URL_2 = "http://wezeejay.fr/ws/metier/test/test2.mp3";
	public static final String URL_3 = "http://wezeejay.fr/ws/metier/test/test3.mp3";
	public static final String URL_4 = "http://wezeejay.fr/ws/metier/test/test4.mp3";
	
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
		
		SuperpoweredExample(sampleRate, bufferSize, URL_4, repertoire.getAbsolutePath());
	}
	
	void startPlayback()
	{
		playState = true;
		onPlayPause(true);
	}
	
	void open2()
	{
		playState = false;
		open(URL_2);
	}
	
	void open3()
	{
		playState = false;
		open(URL_3);
	}
	
	void open4()
	{
		playState = false;
		open(URL_4);
	}
	
	void togglePlayback()
	{
		playState = !playState;
		onPlayPause(playState);
	}
	
	void seek()
	{
		playState = false;
		onSeek();
	}
	
	private native void SuperpoweredExample(int samplerate, int buffersize, String url, String localPath);
	private native void onPlayPause(boolean play);
	private native void open(String url);
	private native void onSeek();
	
}
