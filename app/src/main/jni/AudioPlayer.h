
#ifndef SUPERPOWEREDEXAMPLE_AUDIOPLAYER_H
#define SUPERPOWEREDEXAMPLE_AUDIOPLAYER_H


#include <sstream>
#include <SuperpoweredAdvancedAudioPlayer.h>

class AudioPlayer : public SuperpoweredAdvancedAudioPlayer
{
	public:
		
		AudioPlayer(void *clientData, void (*callback)(void *, SuperpoweredAdvancedAudioPlayerEvent, void *), unsigned int sampleRate, unsigned char cachedPointCount,
	                unsigned int internalBufferSizeSeconds = 2, unsigned int negativeSeconds = 0);
		
		bool test;
	
	private:
	
};


#endif //SUPERPOWEREDEXAMPLE_AUDIOPLAYER_H
