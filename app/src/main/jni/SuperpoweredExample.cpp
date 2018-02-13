
#include <jni.h>
#include <string>
#include <SuperpoweredCPU.h>
#include <SuperpoweredSimple.h>
#include "SuperpoweredExample.h"
#include <malloc.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#define TAG "SuperPoweredExample"

static void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value) {
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File loaded succesfully!");
    };
}

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int __unused samplerate) {
    return ((SuperpoweredExample *)clientdata)->process(audioIO, (unsigned int)numberOfSamples);
}

bool SuperpoweredExample::process(short int *output, unsigned int numberOfSamples) {
    bool silence = !audioPlayer->process(stereoBuffer, false, numberOfSamples);

    if (!silence) {
        /*****************************
        *  APPLY PROCESSING BELOW
        */
//        const int nrChannels = 2;
//        for (int i = 0; i < numberOfSamples * nrChannels; ++i) {
//            stereoBuffer[i] = vibrato.processOneSample(stereoBuffer[i]);
//        }
        /*****************************
         *  APPLY PROCESSING ABOVE
         */

        // The stereoBuffer is ready now, let's put the finished audio into the requested buffers.
        SuperpoweredFloatToShortInt(stereoBuffer, output, numberOfSamples);
    }

    return !silence;
}

SuperpoweredExample::SuperpoweredExample(unsigned int samplerate, unsigned int buffersize, const char *path, const char *localpath)
{
    SuperpoweredAdvancedAudioPlayer::setTempFolder(localpath);

    /*
     * According to the SuperpoweredAdvancedAudioPlayer::process method, the size of our buffer should be: numberOfSamples * 8 + 64 bytes big
     */
    stereoBuffer = (float *)memalign(16, (buffersize * 8) + 64);

    audioPlayer = new SuperpoweredAdvancedAudioPlayer(&audioPlayer , playerEventCallbackA, samplerate, 0);
    audioPlayer->open(path);

    audioSystem = new SuperpoweredAndroidAudioIO(samplerate, buffersize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, 0);
}

SuperpoweredExample::~SuperpoweredExample() {
    delete audioSystem;
    delete audioPlayer;
    free(stereoBuffer);
}

void SuperpoweredExample::onPlayPause(bool play) {
    if (!play) {
        audioPlayer->pause();
    } else {
        audioPlayer->play(false);
    };
    SuperpoweredCPU::setSustainedPerformanceMode(play); // <-- Important to prevent audio dropouts.
}

static SuperpoweredExample *renderer = NULL;

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_SuperpoweredExample(JNIEnv *javaEnvironment, jobject instance, jint samplerate,  jint buffersize, jstring url, jstring localpath)
{
    const char *curl = javaEnvironment->GetStringUTFChars(url, JNI_FALSE);
    const char *clocalpath = javaEnvironment->GetStringUTFChars(url, JNI_FALSE);
    renderer = new SuperpoweredExample((unsigned int)samplerate, (unsigned int)buffersize, curl, clocalpath);
    javaEnvironment->ReleaseStringUTFChars(url, curl);
    javaEnvironment->ReleaseStringUTFChars(url, clocalpath);
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_onPlayPause(JNIEnv *env, jobject instance, jboolean play)
{
    renderer->onPlayPause(play);
}

