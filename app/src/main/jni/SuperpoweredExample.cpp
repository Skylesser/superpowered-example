
#include <jni.h>
#include <string>
#include "SuperpoweredExample.h"
#include <malloc.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#define TAG "SuperPoweredExample"

static SuperpoweredExample *renderer = NULL;
static bool lockAOpen;

static void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "File loaded succesfully!");
	    lockAOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 1");
	    lockAOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_HLSNetworkError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 2");
	    lockAOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_ProgressiveDownloadError)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "File error loading 3");
	    lockAOpen = false;
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_EOF)
    {
        __android_log_print(ANDROID_LOG_INFO, TAG, "File stopped!");
        *((bool *)value) = true;
    }
}

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
	return true;
}

SuperpoweredExample::SuperpoweredExample(unsigned int samplerate, unsigned int buffersize, const char *pathA, const char *pathB, const char *pathC, const char *localpath)
{
    SuperpoweredAdvancedAudioPlayer::setTempFolder(localpath);

	lockAOpen = false;

    playerA = new SuperpoweredAdvancedAudioPlayer(&playerA, playerEventCallbackA, samplerate, 0);
    playerB = new SuperpoweredAdvancedAudioPlayer(&playerB, playerEventCallbackA, samplerate, 0);
    playerC = new SuperpoweredAdvancedAudioPlayer(&playerC, playerEventCallbackA, samplerate, 0);
	playerT = new SuperpoweredAdvancedAudioPlayer(&playerT, playerEventCallbackA, samplerate, 0);

    audioSystem = new SuperpoweredAndroidAudioIO(samplerate, buffersize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, buffersize * 2);
	
	__android_log_print(ANDROID_LOG_INFO, TAG, "SuperpoweredExample started");
}

SuperpoweredExample::~SuperpoweredExample()
{
    delete audioSystem;
    delete playerA;
    delete playerB;
	delete playerC;
	delete playerT;
}

void SuperpoweredExample::open(const char *path, int id)
{
	if (!lockAOpen)
	{
		__android_log_print(ANDROID_LOG_INFO, TAG, "open %d url = %s", id, path);
		lockAOpen = true;
		if (id == 0)
		{
			playerA->open(path);
		}
		else if (id == 1)
		{
			playerB->open(path);
		}
		else if (id == 2)
		{
			playerC->open(path);
		}
	}
	else
	{
		__android_log_print(ANDROID_LOG_WARN, TAG, "open %d url = %s locked!", id, path);
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

extern "C" JNIEXPORT void JNICALL Java_com_superpowered_superpoweredexample_SuperPoweredPlayer_open(JNIEnv *env, jobject instance, jstring url, jint id)
{
    const char *curl = env->GetStringUTFChars(url, JNI_FALSE);
    renderer->open(curl, id);
    env->ReleaseStringUTFChars(url, curl);
}
