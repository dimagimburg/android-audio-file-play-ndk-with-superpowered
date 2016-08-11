#ifndef __Metronome_H
#define __Metronome_H

#include <math.h>
#include <pthread.h>

#include "Metronome.h"
#include <SuperpoweredAdvancedAudioPlayer.h>
#include <AndroidIO/SuperpoweredAndroidAudioIO.h>

class Metronome {

public:
    Metronome(unsigned int sampleRate = 44100, unsigned int bufferSize = 512, const char *path = nullptr, int fileStartOffset = 0, int fileLength = 0, int bpm=100, int beats=4, int bars=4);
    ~Metronome();

    bool process(short int *output, unsigned int numberOfSamples);
    void onPlayPause(bool play);

private:
    pthread_mutex_t mutex;
    SuperpoweredAndroidAudioIO *audioSystem;
    unsigned int sampleRate, bufferSize;
    int bpm, beats, bars;
    SuperpoweredAdvancedAudioPlayer *MetronomePlayer;
    float *stereoBuffer;
    void createBeat();
    void createBar();
};

#endif //__Metronome_H
