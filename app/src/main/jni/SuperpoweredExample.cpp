
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
#define T1 270242.494
#define T2 31746.100

static SuperpoweredExample *renderer = NULL;

static void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File A loaded succesfully!");
        SuperpoweredAdvancedAudioPlayer *player = *((SuperpoweredAdvancedAudioPlayer **)clientData);
        player->setBpm(127.98);
        player->setFirstBeatMs(T1);
        player->setPosition(0, true, false);
    };
}

static void playerEventCallbackB(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File B loaded succesfully!");
        SuperpoweredAdvancedAudioPlayer *player = *((SuperpoweredAdvancedAudioPlayer **)clientData);
        player->setBpm(125.0);
        player->setFirstBeatMs(T2);
        player->setPosition(player->firstBeatMs, true, false);
    };
}

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredExample *)clientdata)->process(audioIO, (unsigned int)numberOfSamples);
}

bool SuperpoweredExample::process(short int *audioIO, unsigned int numberOfSamples)
{
    double masterBpm = playerA->currentBpm;
    double msElapsedSinceLastBeatA = playerA->msElapsedSinceLastBeat; // When playerB needs it, playerA has already stepped this value, so save it now.

    __android_log_print(ANDROID_LOG_INFO, TAG, "playerA->positionMs = %f, T1 = %f, playerA->bufferEndPercent = %f", playerA->positionMs, T1, playerA->bufferEndPercent);
    __android_log_print(ANDROID_LOG_INFO, TAG, "playerA->looping = %d", playerA->looping);

    // play playerB at the right moment:
    if (playerA->bpm > 0 && playerA->positionMs > T1 && !playerB->playing)
    {
        playerB->play(true);
        __android_log_print(ANDROID_LOG_WARN, TAG, "playerB->play(true)");
    }

    // process and filters:
    bool silence1 = !playerA->process(stereoBuffer1, false, numberOfSamples, 1.0, masterBpm); // , playerB->msElapsedSinceLastBeat
    bool silence2 = !playerB->process(stereoBuffer2, false, numberOfSamples, 1.0, masterBpm, msElapsedSinceLastBeatA);
    if (!silence2)
    {
    //    filter->process(stereoBuffer2, stereoBuffer2, numberOfSamples);
    }

    // the stereoBuffer is ready now, let's put the finished audio into the requested buffers :
    if (!silence1 || !silence2)
    {
        if (silence2)
        {
            SuperpoweredFloatToShortInt(stereoBuffer1, audioIO, numberOfSamples);
        }
        else if (silence1)
        {
            SuperpoweredFloatToShortInt(stereoBuffer2, audioIO, numberOfSamples);
        }
        else
        {
            SuperpoweredAdd2(stereoBuffer1, stereoBuffer2, stereoBuffer, numberOfSamples * 2);
            SuperpoweredFloatToShortInt(stereoBuffer, audioIO, numberOfSamples);
        }
    }

    return (!silence1 || !silence2);
}

SuperpoweredExample::SuperpoweredExample(unsigned int samplerate, unsigned int buffersize, const char *pathA, const char *pathB, const char *pathC, const char *localpath)
{
    SuperpoweredAdvancedAudioPlayer::setTempFolder(localpath);

    stereoBuffer1 = (float *)memalign(16, (buffersize * 8) + 64);
    stereoBuffer2 = (float *)memalign(16, (buffersize * 8) + 64);
    stereoBuffer = (float *)memalign(16, (buffersize * 8) + 64);

    playerA = new SuperpoweredAdvancedAudioPlayer(&playerA , playerEventCallbackA, samplerate, 0);
    playerA->open(pathA);
    playerB = new SuperpoweredAdvancedAudioPlayer(&playerB, playerEventCallbackB, samplerate, 0);
    playerB->open(pathB);
    // playerC = new SuperpoweredAdvancedAudioPlayer(&playerC, playerEventCallbackC, samplerate, 0);
    // playerC->open(pathC);

    playerA->syncMode = SuperpoweredAdvancedAudioPlayerSyncMode_TempoAndBeat;
    playerB->syncMode = SuperpoweredAdvancedAudioPlayerSyncMode_TempoAndBeat;

    filter = new Superpowered3BandEQ(samplerate);
    filter->bands[0] = 0.7;
    filter->bands[1] = 1;
    filter->bands[2] = 1;
    filter->enable(true);

    audioSystem = new SuperpoweredAndroidAudioIO(samplerate, buffersize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, buffersize * 2);
}

SuperpoweredExample::~SuperpoweredExample()
{
    delete audioSystem;
    delete playerA;
    delete playerB;
    // delete playerC;
    delete filter;
    free(stereoBuffer1);
    free(stereoBuffer2);
    free(stereoBuffer);
}

void SuperpoweredExample::onPlayPause(bool play)
{
    if (!play)
    {
        if (playerA->playing)
        {
            playerA->pause(0.3);
        }
        if (playerB->playing)
        {
            playerB->pause(0.3);
        }
    }
    else
    {
        playerA->play(false);
        //playerB->play(true);
        //playerA->setTempo(1.0, true);
    };
    SuperpoweredCPU::setSustainedPerformanceMode(play); // <-- Important to prevent audio dropouts.
}

void SuperpoweredExample::open(const char *path)
{
    // playerA->open(path);
}

void SuperpoweredExample::onSeek()
{
    playerA->setPosition(265000, true, false);
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_SuperpoweredExample(JNIEnv *env, jobject instance, jint samplerate,
                                                                                                                   jint buffersize, jstring urlA, jstring urlB, jstring urlC,
                                                                                                                   jstring localpath)
{
    const char *curlA = env->GetStringUTFChars(urlA, JNI_FALSE);
    const char *curlB = env->GetStringUTFChars(urlB, JNI_FALSE);
    const char *curlC = env->GetStringUTFChars(urlC, JNI_FALSE);
    const char *clocalpath = env->GetStringUTFChars(localpath, JNI_FALSE);
    renderer = new SuperpoweredExample((unsigned int) samplerate, (unsigned int) buffersize, curlA, curlB, curlC, clocalpath);
    env->ReleaseStringUTFChars(urlA, curlA);
    env->ReleaseStringUTFChars(urlB, curlB);
    env->ReleaseStringUTFChars(urlC, curlC);
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

