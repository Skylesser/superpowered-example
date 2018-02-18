
#ifndef SUPERPOWEREDEXAMPLEB_SUPERPOWEREDEXAMPLE_H
#define SUPERPOWEREDEXAMPLEB_SUPERPOWEREDEXAMPLE_H


#include <AndroidIO/SuperpoweredAndroidAudioIO.h>
#include <SuperpoweredAdvancedAudioPlayer.h>

class SuperpoweredExample {

public:
    SuperpoweredExample(unsigned int samplerate, unsigned int buffersize, const char *path, const char *localpath);
    ~SuperpoweredExample();

    bool process(short int *output, unsigned int numberOfSamples);
    void onPlayPause(bool play);
    void open(const char *path);
    void onSeek();

private:
    SuperpoweredAndroidAudioIO *audioSystem;
    SuperpoweredAdvancedAudioPlayer *audioPlayer;
    float *stereoBuffer;

};


#endif
