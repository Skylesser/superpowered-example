package com.superpowered.superpoweredexample;

import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity
{
	
	private SuperPoweredPlayer player = null;
	private int a = 0;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// TODO : *-> resume download and play si coupure internet
		// TODO : *-> bug comment connaître le niveau de téléchargement d'un titre en secondes à tout moment
		// TODO : *-> play ne fonctionne pas même si on remet une connexion internet / play(false) retardé si on fait pause avant
		// TODO : *-> légères variations de volumes si le son est accéléré en master tempo / parasites si le son est ralenti en master tempo
		// TODO : -> bug x 2 player.open() si 3 players ou plus
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
			Log.d("SuperPoweredExample", "stress open test start...");
			new CountDownTimer(300000, 8000) {
				
				@Override
				public void onTick(long millisUntilFinished)
				{
					if (a == 0)
					{
						player.openFile(0, 17);
					}
					else if (a == 1)
					{
						player.openFile(1, 38);
					}
					else if (a == 2)
					{
						player.openFile(2, 2);
					}
					else
					{
						player.openFile(2, 35);
					}
					a++;
					Log.i("SuperPoweredExample", "loop #" + a);
				}
				
				@Override
				public void onFinish()
				{
					Log.e("SuperPoweredExample", "stress open test finished");
				}
			}.start();
		}
	}
	
}
