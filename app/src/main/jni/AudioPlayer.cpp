
#include <string>
#include <android/log.h>
#include "AudioPlayer.h"


AudioPlayer::AudioPlayer(void *clientData, void (*callback)(void *, SuperpoweredAdvancedAudioPlayerEvent, void *), unsigned int sampleRate, unsigned char cachedPointCount,
                         unsigned int internalBufferSizeSeconds, unsigned int negativeSeconds) :
						 SuperpoweredAdvancedAudioPlayer(clientData, callback, sampleRate, cachedPointCount, internalBufferSizeSeconds, negativeSeconds)
{
}
