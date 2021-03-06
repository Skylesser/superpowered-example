package com.superpowered.superpoweredexample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity
{
	
	private SuperPoweredPlayer player = null;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// TODO : *-> bug comment connaître le niveau de téléchargement d'un titre en secondes à tout moment
		// TODO : *-> bug lecture ne se lance pas sur LG Q6
		// TODO : *-> bug aléatoire si coupure internet pendant le téléchargement d'un titre : la lecture se coupe, même s'il y a assez de données, et à la reconnexion, la lecture ne reprend pas
		// TODO : *-> bug si pas assez de données : le son rejoue dès qu'il y a (beaucoup) moins de 2 secondes à jouer
		// TODO : *-> 		légères variations de volumes si le son est accéléré en master tempo / parasites si le son est ralenti en master tempo
		// TODO : *-> 		play(false) retardé si on fait pause avant
		// TODO : *-> 		si player.open() 2 fois de suite dans une séquence de téléchargement, player.fullyDownloadedFilePath redevient NULL
		
		// TODO : télécharger depuis une position (voir open())
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		player = new SuperPoweredPlayer(this);
	}
	
	public void onClickOpen(View view)
	{
		if (player != null)
		{
			Log.w("SuperPoweredExample", "open");
			player.openMp3();
		}
	}
	
	public void onClickPlay(View view)
	{
		if (player != null)
		{
			Log.w("SuperPoweredExample", "play");
			player.playMp3();
		}
	}
	
}
