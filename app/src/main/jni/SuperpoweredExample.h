
#ifndef SUPERPOWEREDEXAMPLE_SUPERPOWEREDEXAMPLE_H
#define SUPERPOWEREDEXAMPLE_SUPERPOWEREDEXAMPLE_H


#include <AndroidIO/SuperpoweredAndroidAudioIO.h>
#include <SuperpoweredAdvancedAudioPlayer.h>
#include <Superpowered3BandEQ.h>
#include "AudioPlayer.h"

class SuperpoweredExample {

public:
    SuperpoweredExample(unsigned int samplerate, unsigned int buffersize, const char *pathA, const char *pathB, const char *pathC, const char *localpath);
    ~SuperpoweredExample();

    bool process(short int *output, unsigned int numberOfSamples);
	void open();
	void play();

private:
    SuperpoweredAndroidAudioIO *audioSystem;
    AudioPlayer *playerA;
    AudioPlayer *playerB;
    AudioPlayer *playerC;
	float *stereoBuffer1;
	float *stereoBuffer2;
	float *stereoBuffer3;
	float *stereoBufferP;

};


#endif
