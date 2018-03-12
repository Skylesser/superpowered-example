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
		// TODO : *-> bug comment connaître le niveau de téléchargement d'un titre en secondes à tout moment
		// TODO : *-> play ne fonctionne pas même si on remet une connexion internet / play(false) retardé si on fait pause avant
		// TODO : *-> légères variations de volumes si le son est accéléré en master tempo / parasites si le son est ralenti en master tempo
		// TODO : -> download fail player.open() si séquence téléchargement de 3 players ou plus
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
			Log.w("SuperPoweredExample", "open test start...");
			player.openSequence();
			
			new CountDownTimer(120000, 1000) {
				
				@Override
				public void onTick(long millisUntilFinished)
				{
					player.checkSequence();
				}
				
				@Override
				public void onFinish()
				{
					Log.e("SuperPoweredExample", "open test finished");
				}
			}.start();
		}
	}
	
}
