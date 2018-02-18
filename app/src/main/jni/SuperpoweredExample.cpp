
#include <jni.h>
#include <string>
#include <SuperpoweredCPU.h>
#include <SuperpoweredSimple.h>
#include "SuperpoweredExample.h"
#include <malloc.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <Superpowered3BandEQ.h>

#define TAG "SuperPoweredExample"

static SuperpoweredExample *renderer = NULL;
static Superpowered3BandEQ *equalizer = NULL;

static void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value) {
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File loaded succesfully!");
    };
}

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int __unused samplerate) {
    return ((SuperpoweredExample *)clientdata)->process(audioIO, (unsigned int)numberOfSamples);
}

bool SuperpoweredExample::process(short int *audioIO, unsigned int numberOfSamples) {
    bool silence = !audioPlayer->process(stereoBuffer, false, numberOfSamples, 1.0f);

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
        SuperpoweredFloatToShortInt(stereoBuffer, audioIO, numberOfSamples);
        //equalizer->process(audioIO, audioIO, numberOfSamples);
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
        audioPlayer->setTempo(1.0f, true);
    };
//    SuperpoweredCPU::setSustainedPerformanceMode(play); // <-- Important to prevent audio dropouts.
}

void SuperpoweredExample::open(const char *path)
{
    audioPlayer->open(path);
}

void SuperpoweredExample::onSeek()
{
    audioPlayer->setPosition(120000.4658f, true, false);
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_SuperpoweredExample(JNIEnv *env, jobject instance, jint samplerate,  jint buffersize, jstring url, jstring localpath)
{
    const char *curl = env->GetStringUTFChars(url, JNI_FALSE);
    const char *clocalpath = env->GetStringUTFChars(localpath, JNI_FALSE);
    renderer = new SuperpoweredExample((unsigned int) samplerate, (unsigned int) buffersize, curl, clocalpath);
    equalizer = new Superpowered3BandEQ((unsigned int) samplerate);
    env->ReleaseStringUTFChars(url, curl);
    env->ReleaseStringUTFChars(localpath, clocalpath);
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_onPlayPause(JNIEnv *env, jobject instance, jboolean play)
{
    renderer->onPlayPause(play);
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_open(JNIEnv *env, jobject instance, jstring url)
{
    const char *curl = env->GetStringUTFChars(url, JNI_FALSE);
    renderer->open(curl);
    env->ReleaseStringUTFChars(url, curl);
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_onSeek(JNIEnv *env, jobject instance)
{
    renderer->onSeek();
}

