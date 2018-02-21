package com.superpowered.superpoweredexample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

public class MainActivity extends AppCompatActivity
{
	
	private SuperPoweredPlayer player = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// TODO : *resume download and play si coupure internet
		// TODO : comment lancer un second titre à une frame précise d'un premier titre ? (voir processMulti() / cachePosition() / setBpm() / setFirstBeatMs() / play(true) / audioStartMs / waitingForSyncMs)
		// TODO : comment connaître le niveau de téléchargement d'un titre en secondes à tout moment / comment savoir qu'un titre est complètement téléchargé ? (voir player->bufferStartPercent / player->bufferEndPercent / player->waitingForBuffering / player->fullyDownloadedFilePath non null)
		// TODO : (volume : quel type ? multiplicateur d'amplitude ?)
		
		// TODO : thread séparé pour le lecteur audio
		// TODO : tests transitions au tempo (avec changement de tempo en cours de route)
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		player = new SuperPoweredPlayer(this);
	}
	
	public void onClickPlay(View view)
	{
		if (player != null)
		{
			player.startPlayback();
		}
	}
	
	public void onClickToggle(View view)
	{
		if (player != null)
		{
			player.togglePlayback();
		}
	}
	
	public void onClickSeek(View view)
	{
		if (player != null)
		{
			player.seek();
		}
	}
	
}
