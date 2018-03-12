
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
static bool lockOpen;
static int step = 0;

static void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File loaded succesfully!");
	    lockOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 1");
	    lockOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_HLSNetworkError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 2");
	    lockOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_ProgressiveDownloadError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 3");
	    lockOpen = false;
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
	// process:
	bool silenceA = !playerA->process(stereoBuffer1, false, numberOfSamples);
	bool silenceB = !playerB->process(stereoBuffer2, false, numberOfSamples);
	bool silenceC = !playerC->process(stereoBuffer3, false, numberOfSamples);
	if (!silenceA)
	{
		SuperpoweredFloatToShortInt(stereoBuffer1, audioIO, numberOfSamples);
	}
	if (!silenceB)
	{
		SuperpoweredFloatToShortInt(stereoBuffer2, audioIO, numberOfSamples);
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
	
	lockOpen = false;

    playerA = new SuperpoweredAdvancedAudioPlayer(&playerA, playerEventCallbackA, samplerate, 0);
    playerB = new SuperpoweredAdvancedAudioPlayer(&playerB, playerEventCallbackA, samplerate, 0);
    playerC = new SuperpoweredAdvancedAudioPlayer(&playerC, playerEventCallbackA, samplerate, 0);
	
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
}

void SuperpoweredExample::open()
{
	// init download sequence in SuperpoweredExample::process():
	step = 1;
}

void SuperpoweredExample::check()
{
	// download sequence:
	if (step == 1)
	{
		step++;
		if (!lockOpen)
		{
			lockOpen = true;
			__android_log_print(ANDROID_LOG_INFO, TAG, "playerA->open(http://www.wezeejay.fr/audio/17.mp3)");
			playerA->open("http://www.wezeejay.fr/audio/17.mp3");
		}
	}
	else if (step == 2)
	{
		if (playerA->fullyDownloadedFilePath != NULL)
		{
			step++;
			__android_log_print(ANDROID_LOG_DEBUG, TAG, "playerA complete!");
		}
	}
	else if (step == 3)
	{
		step++;
		if (!lockOpen)
		{
			lockOpen = true;
//			__android_log_print(ANDROID_LOG_INFO, TAG, "playerB->open(http://www.wezeejay.fr/audio/38.mp3)");
//			playerB->open("http://www.wezeejay.fr/audio/38.mp3");
			__android_log_print(ANDROID_LOG_INFO, TAG, "playerA->open(http://www.wezeejay.fr/audio/17.mp3)");
			playerA->open("http://www.wezeejay.fr/audio/17.mp3");
		}
	}
	else if (step == 4)
	{
		if (playerB->fullyDownloadedFilePath != NULL)
		{
			step++;
			__android_log_print(ANDROID_LOG_DEBUG, TAG, "playerB complete!");
		}
	}
	else if (step == 5)
	{
		step++;
		if (!lockOpen)
		{
			lockOpen = true;
			__android_log_print(ANDROID_LOG_INFO, TAG, "playerC->open(http://www.wezeejay.fr/audio/2.mp3)");
			playerC->open("http://www.wezeejay.fr/audio/2.mp3");
		}
	}
	else if (step == 6)
	{
		if (playerC->fullyDownloadedFilePath != NULL)
		{
			step++;
			__android_log_print(ANDROID_LOG_DEBUG, TAG, "playerC complete!");
		}
	}
	else if (step == 7)
	{
		step++;
		if (!lockOpen)
		{
			lockOpen = true;
			__android_log_print(ANDROID_LOG_INFO, TAG, "playerB->open(http://www.wezeejay.fr/audio/35.mp3)");
			playerB->open("http://www.wezeejay.fr/audio/35.mp3");
		}
	}
	else if (step == 8)
	{
		if (playerB->fullyDownloadedFilePath != NULL)
		{
			step++;
			__android_log_print(ANDROID_LOG_DEBUG, TAG, "playerB complete!");
		}
	}
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

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_check(JNIEnv *env, jobject instance)
{
	renderer->check();
}
