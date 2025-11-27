/*
  ==============================================================================

    NoiseGenerator.cpp
    Created: 29 Oct 2025 3:55:39pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(bool bandPassFilterNoise)
{
    filterNoise = bandPassFilterNoise;
    amplitude = 0.0f;
    centreFrequency = 0.0f;
    sampleRate = 44100.0;

    prepareFilter();
}

float NoiseGenerator::nextSample()
{
    float whiteNoise = random.nextFloat() * 2.0f - 1.0f;
    if (!filterNoise) return whiteNoise * amplitude;

    float filteredSample = bandPassFilter.processSample(whiteNoise);
    return filteredSample * amplitude;
}



void NoiseGenerator::setFrequency(float newFrequency, float bandwidthOctaves) {
    centreFrequency = newFrequency;
    float q = 1.0f / (std::pow(2.0f, bandwidthOctaves / 2.0f) - std::pow(2.0f, -bandwidthOctaves / 2.0f));
    *bandPassFilter.coefficients = *juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, centreFrequency, q);
}

void NoiseGenerator::setAmplitude(float newAmplitude)
{
    amplitude = newAmplitude;
}

void NoiseGenerator::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;

    prepareFilter();
}

void NoiseGenerator::prepareFilter()
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = 1;
    spec.numChannels = 1;
    bandPassFilter.prepare(spec);
}
