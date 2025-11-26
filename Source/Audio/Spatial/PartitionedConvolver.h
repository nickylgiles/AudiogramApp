/*
  ==============================================================================

    PartitionedConvolver.h
    Created: 25 Nov 2025 7:42:14pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PartitionedConvolver {
public:
    PartitionedConvolver(int partitionSize);
    void loadIR(const juce::AudioBuffer<float>& irBuffer);
    void processBlock(const float* input, float* output, int numSamples);

private:
    void processPartition();

    int partitionSize;
    int fftSize; // fft size = 2*partitionSize or next power of 2

    std::vector<std::vector<juce::dsp::Complex<float>>> irPartitions; // fft of IR partitions

    juce::AudioBuffer<float> inputBuffer; // accumulate incoming samples until enough for FFT
    int inputBufferPos;

    std::vector<std::vector<juce::dsp::Complex<float>>> inputFFTBuffer;
    int inputFFTBufferPos;


    juce::AudioBuffer<float> overlapBuffer;

    int numPartitions;

    juce::dsp::FFT fft;

    int processedPartitions = 0;

};