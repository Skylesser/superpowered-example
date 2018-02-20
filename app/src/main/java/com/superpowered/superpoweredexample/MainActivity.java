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
		// TODO : filtre 3 bandes
		// TODO : comment connaître le niveau de téléchargement d'un titre en secondes à tout moment / comment savoir qu'un titre est complètement téléchargé ?
		// TODO : comment connaître la position à tout instant (pour changement filtre et volume) ?
		// TODO : (volume : quel type ? multiplicateur d'amplitude ?)
		
		// TODO : thread séparé pour le lecteur audio
		// TODO : tests transitions au tempo
		
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
