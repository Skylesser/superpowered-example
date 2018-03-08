package com.superpowered.superpoweredexample;

import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity
{
	
	private SuperPoweredPlayer player = null;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// TODO : *-> resume download and play si coupure internet
		// TODO : *-> comment connaître le niveau de téléchargement d'un titre en secondes à tout moment / comment savoir qu'un titre est complètement téléchargé ? (voir player->fullyDownloadedFilePath non null)
		// TODO : *-> play ne fonctionne pas même si on remet une connexion internet / play(false) retardé si on fait pause avant
		// TODO : *-> légères variations de volumes si le son est accéléré en master tempo / parasites si le son est ralenti en master tempo
		// TODO : tests transitions au tempo (changement de tempo en cours de route / enchaînements multiples)
		
		// TODO : télécharger depuis une position (voir open())
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		player = new SuperPoweredPlayer(this);
	}
	
	public void onClickOpen(View view)
	{
		if (player != null)
		{
			new CountDownTimer(5000, 50) {
				
				@Override
				public void onTick(long millisUntilFinished)
				{
					player.openFile();
				}
				
				@Override
				public void onFinish()
				{
					Log.e("SuperPoweredExample", "stress open test finished");
				}
			}.start();
			//player.openFile();
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
	
	public void onClickStop(View view)
	{
		if (player != null)
		{
			player.stopDownload();
		}
	}
	
}
