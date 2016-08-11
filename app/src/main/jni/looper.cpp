#include "Metronome.h"
#include <SuperpoweredSimple.h>
#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

static void metronomePlayerEventCallback(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value) {
    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess) {
        __android_log_print(ANDROID_LOG_DEBUG, "NATIVE LOOPER", "metronomePlayerEventCallback :: Event :: LoadSuccess");
    };
}

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int __unused samplerate) {
    return ((Metronome *)clientdata)->process(audioIO, (unsigned int)numberOfSamples);
}

Metronome::Metronome(unsigned int _sampleRate, unsigned int _bufferSize, const char *path, int fileStartOffset, int fileLength, int _bpm, int _beats, int _bars) {

    //pthread_mutex_init(&mutex, NULL);

    bpm = _bpm;
    beats = _beats;
    bars = _bars;
    sampleRate = _sampleRate;
    bufferSize = _bufferSize;

    stereoBuffer = (float *)memalign(16, (bufferSize + 16) * sizeof(float) * 2);

    MetronomePlayer = new SuperpoweredAdvancedAudioPlayer(&MetronomePlayer , metronomePlayerEventCallback, sampleRate, 0);
    MetronomePlayer->open(path, fileStartOffset, fileLength);
    audioSystem = new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2);
}

bool Metronome::process(short int *output, unsigned int numberOfSamples) {
    bool silence = !MetronomePlayer->process(stereoBuffer, false, numberOfSamples);
    // The stereoBuffer is ready now, let's put the finished audio into the requested buffers.
    if (!silence) SuperpoweredFloatToShortInt(stereoBuffer, output, numberOfSamples);
    return !silence;
}

Metronome::~Metronome() {
    delete audioSystem;
    delete MetronomePlayer;
    free(stereoBuffer);
    //pthread_mutex_destroy(&mutex);
}

void Metronome::onPlayPause(bool play) {
    //pthread_mutex_lock(&mutex);
    if (!play) {
        MetronomePlayer->pause();
    } else {
        MetronomePlayer->play(false);
    };
    //pthread_mutex_unlock(&mutex);
}

static Metronome *metronome = NULL;

extern "C" JNIEXPORT void JNICALL
Java_com_dima_looper_MainActivity_Metronome(
        JNIEnv *env,
        jobject instance,
        jint sampleRate,
        jint bufferSize,
        jstring apkPath_,
        jint fileStartOffset,
        jint fileLength,
        jint bpm,
        jint beats,
        jint bars
) {
    const char *apkPath = env->GetStringUTFChars(apkPath_, 0);

    metronome = new Metronome((unsigned int)sampleRate, (unsigned int)bufferSize, apkPath, fileStartOffset, fileLength);

    env->ReleaseStringUTFChars(apkPath_, apkPath);
}

JNIEXPORT void JNICALL
Java_com_dima_looper_Models_MetronomeTrack_onMetronomeStop(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_dima_looper_Models_MetronomeTrack_onMetronomePause(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_dima_looper_Models_MetronomeTrack_onMetronomeChangeBpm(JNIEnv *env, jobject instance,
                                                                jint bpm) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_dima_looper_Models_MetronomeTrack_onMetronomeChangeBeats(JNIEnv *env, jobject instance,
                                                                  jint beats) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_dima_looper_Models_MetronomeTrack_onMetronomeChangeBars(JNIEnv *env, jobject instance,
                                                                 jint bars) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_dima_looper_Models_MetronomeTrack_onMetronomeChangeVolume(JNIEnv *env, jobject instance,
                                                                   jdouble volume) {

    // TODO

}

extern "C" JNIEXPORT void JNICALL
Java_com_dima_looper_MainActivity_SetTempFolder(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    SuperpoweredAdvancedAudioPlayer::setTempFolder(path);

    env->ReleaseStringUTFChars(path_, path);
}

extern "C" JNIEXPORT void JNICALL
Java_com_dima_looper_MainActivity_onMetronomePlay(JNIEnv *env, jobject instance, jboolean play) {
    __android_log_print(ANDROID_LOG_DEBUG, "NATIVE LOOPER", "Java_com_dima_looper_MainActivity_onMetronomePlay play = %d", play);
    metronome->onPlayPause(play);

}