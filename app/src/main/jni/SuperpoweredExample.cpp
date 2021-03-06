
#include <jni.h>
#include <string>
#include <SuperpoweredSimple.h>
#include "SuperpoweredExample.h"
#include <malloc.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#define TAG "SuperPoweredExample"

static SuperpoweredExample *renderer = NULL;

static void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "SuperpoweredExample playerEventCallbackA");
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File loaded succesfully!");
	    AudioPlayer *player = *((AudioPlayer **) clientData);
	    if (player->test)
	    {
		    __android_log_print(ANDROID_LOG_WARN, TAG, "Play!");
		    player->play(false);
	    }
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 1");
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_HLSNetworkError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 2");
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_ProgressiveDownloadError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 3");
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_EOF)
    {
        __android_log_print(ANDROID_LOG_INFO, TAG, "File stopped!");
        *((bool *)value) = true;
    }
}

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
	return ((SuperpoweredExample *) clientdata)->process(audioIO, (unsigned int) numberOfSamples);
}

bool SuperpoweredExample::process(short int *audioIO, unsigned int numberOfSamples)
{
	// init:
	double msElapsedSinceLastBeat1 = playerA->msElapsedSinceLastBeat;
	
	// launch playerB when playerA reaches a position:
	if (playerA->bufferEndPercent >= positionLog)
	{
		__android_log_print(ANDROID_LOG_INFO, TAG, "Position : %lf, bufferEndPercent : %lf, playing : %i", playerA->positionMs, playerA->bufferEndPercent, playerA->playing);
		positionLog += 0.02;
	}
	if (playerA->positionMs > 5000)
	{
//		playerB->play(true);
	}
	
	// process:
	bool silenceA = !playerA->process(stereoBuffer1, false, numberOfSamples, 0.2, 120);
	bool silenceB = !playerB->process(stereoBuffer2, false, numberOfSamples, 1, 120, msElapsedSinceLastBeat1);
	bool silenceC = !playerC->process(stereoBuffer3, false, numberOfSamples);
	if (!silenceA && silenceB)
	{
		SuperpoweredFloatToShortInt(stereoBuffer1, audioIO, numberOfSamples);
	}
	else if (!silenceB && silenceA)
	{
		SuperpoweredFloatToShortInt(stereoBuffer2, audioIO, numberOfSamples);
	}
	else if (!silenceA && !silenceB)
	{
		SuperpoweredAdd2(stereoBuffer1, stereoBuffer2, stereoBufferP, numberOfSamples * 2);
		SuperpoweredFloatToShortInt(stereoBufferP, audioIO, numberOfSamples);
	}
	if (!silenceC)
	{
		SuperpoweredFloatToShortInt(stereoBuffer3, audioIO, numberOfSamples);
	}
	return (!silenceA || !silenceB || !silenceC);
}

SuperpoweredExample::SuperpoweredExample(unsigned int samplerate, unsigned int buffersize, const char *pathA, const char *pathB, const char *pathC, const char *localpath)
{
    SuperpoweredAdvancedAudioPlayer::setTempFolder(localpath);
	
	stereoBuffer1 = (float *) memalign(16, (buffersize * 8) + 64);
	stereoBuffer2 = (float *) memalign(16, (buffersize * 8) + 64);
	stereoBuffer3 = (float *) memalign(16, (buffersize * 8) + 64);
	stereoBufferP = (float *) memalign(16, (buffersize * 8) + 64);
	
    playerA = new AudioPlayer(&playerA, playerEventCallbackA, samplerate, 0);
    playerB = new AudioPlayer(&playerB, playerEventCallbackA, samplerate, 0);
    playerC = new AudioPlayer(&playerC, playerEventCallbackA, samplerate, 0);
	
	playerA->syncMode = SuperpoweredAdvancedAudioPlayerSyncMode_TempoAndBeat;
	playerB->syncMode = SuperpoweredAdvancedAudioPlayerSyncMode_TempoAndBeat;
	playerC->syncMode = SuperpoweredAdvancedAudioPlayerSyncMode_TempoAndBeat;
	
	audioSystem = new SuperpoweredAndroidAudioIO(samplerate, buffersize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, buffersize * 2);
	
	__android_log_print(ANDROID_LOG_INFO, TAG, "SuperpoweredExample started");
}

SuperpoweredExample::~SuperpoweredExample()
{
    delete audioSystem;
    delete playerA;
    delete playerB;
	delete playerC;
	free(stereoBuffer1);
	free(stereoBuffer2);
	free(stereoBuffer3);
	free(stereoBufferP);
}

void SuperpoweredExample::open()
{
	positionLog = 0;
	
	playerA->test = true;
	playerA->open("http://www.wezeejay.fr/audio/89.mp3");
	playerB->open("http://www.wezeejay.fr/audio/200.mp3");
	
	playerA->setBpm(120);
	playerB->setBpm(120);
	
	playerA->setFirstBeatMs(5000);
	playerB->setFirstBeatMs(1000);
	playerB->setPosition(1000, true, false);
	
//	playerA->play(false);
}

void SuperpoweredExample::play()
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "playerA download complete : %i", (playerA->fullyDownloadedFilePath != NULL));
	__android_log_print(ANDROID_LOG_INFO, TAG, "playerB download complete : %i", (playerB->fullyDownloadedFilePath != NULL));
	playerA->play(false);
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

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_open(JNIEnv *env, jobject instance)
{
	renderer->open();
}

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_play(JNIEnv *env, jobject instance)
{
	renderer->play();
}
