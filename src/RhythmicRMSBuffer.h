//
// Created by chels on 5/22/2025.
//

#ifndef RHYTHMICRMSBUFFER_H
#define RHYTHMICRMSBUFFER_H

#include <JuceHeader.h>

class RhythmicRMSBuffer {
    // figure out whether the playhead position represnets BEGINNING or END
    // playHead: represents where we are into playback
public:
    void addData(juce::AudioBuffer<float> buffer, float playHead);
    void setSampleRate(int sampleRate);
    void setBpm(float bpm);
    //
    static constexpr int NUM_CHUNKS = 10;
    float storedAudio[100000];
    float rmsBuffer[NUM_CHUNKS];
    float sampleRate = 44100.f;
    int bufferSize = 1000;
    float bpm;
};



#endif //RHYTHMICRMSBUFFER_H
