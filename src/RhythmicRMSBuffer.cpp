//
// Created by chels on 5/22/2025.
//

#include "RhythmicRMSBuffer.h"


void RhythmicRMSBuffer::addData(juce::AudioBuffer<float> buffer, float playHead) {
    // assuming that playhead represents beginning of buffer
    int startIndex = std::fmod(playHead, 1.0f);
    float numChannels = buffer.getNumChannels();
    float channelFactor = 1.f / numChannels;
    // CHUNK LOGIC
    int startChunk = floor((double) startIndex / (double) bufferSize) * (double) NUM_CHUNKS;
    int endChunk = floor((double) (startIndex + buffer.getNumSamples()) / (double) bufferSize) * (double) NUM_CHUNKS;
    // FILL POW BUFFER
    for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
        const float* channelData = buffer.getReadPointer(channel, 0);
        for (int i = 0; i < numChannels; i++) {
            storedAudio[startIndex + i] += std::powf(channelData[i], 2.f) * channelFactor;
        }
    }
    // prefix summify
    for (int i = startIndex; i < startIndex + buffer.getNumSamples(); i++) {
        if (i == 0) {continue;}
        storedAudio[i] += storedAudio[i - 1];
    }
    // CALCULATE RMS
    for (int chunk = startChunk; chunk < endChunk; chunk++) {
        int chunkStart = (chunk / NUM_CHUNKS) * bufferSize;
        int chunkEnd = ((chunk + 1) / NUM_CHUNKS) * bufferSize;
        float sum = storedAudio[chunkEnd] - storedAudio[chunkStart];
        rmsBuffer[chunk] = std::sqrt(sum);
    }
}
void RhythmicRMSBuffer::setSampleRate(int sampleRate) {
    this->sampleRate = sampleRate;
}
void RhythmicRMSBuffer::setBpm(float bpm) {
    this->bpm = bpm;
}