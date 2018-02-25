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
		// TODO : *-> resume download and play si coupure internet
		// TODO : *-> comment connaître le niveau de téléchargement d'un titre en secondes à tout moment / comment savoir qu'un titre est complètement téléchargé ? (voir player->fullyDownloadedFilePath non null)
		// TODO : *-> play ne fonctionne pas même si on remet une connexion internet / play(false) retardé si on fait pause avant
		// TODO : -> comment arrêter le player si arrivé à la fin du titre ?
		// TODO : -> légères variations de volumes si le son est accéléré en master tempo
		// TODO : tests transitions au tempo (changement de tempo en cours de route / enchaînements multiples)
		
		// TODO : (volume : quel type ? multiplicateur d'amplitude ?)
		// TODO : thread séparé pour le lecteur audio
		// TODO : télécharger depuis une position (voir open())
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		player = new SuperPoweredPlayer(this);
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
