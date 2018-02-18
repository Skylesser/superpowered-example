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
		// TODO : connaître le niveau de téléchargement d'un titre en secondes à tout moment / comment savoir qu'un titre est complètement téléchargé ?
		// TODO : filtre 3 bandes
		// TODO : comment connaître la position à tout instant ?
		// TODO : comment lancer un second titre à une frame précise d'un premier titre, et jouer 2 titres simultanément ?
		// TODO : (volume : quel type ? multiplicateur d'amplitude ?)
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}
	
	public void onClickPlay(View view)
	{
		player = new SuperPoweredPlayer(this);
		player.startPlayback();
		view.setVisibility(View.INVISIBLE);
	}
	
	public void onClickOpen(View view)
	{
		if (player != null)
		{
			if (view.getId() == R.id.open2)
			{
				player.open2();
			}
			else if (view.getId() == R.id.open3)
			{
				player.open3();
			}
			else if (view.getId() == R.id.open4)
			{
				player.open4();
			}
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
