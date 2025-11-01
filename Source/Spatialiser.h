/*
  ==============================================================================

    Spatialiser.h
    Created: 1 Nov 2025 7:00:41pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

class Spatialiser {
public:
    Spatialiser();
    void setSampleRate(double newSampleRate);
    void setAzimuth(float azimuthDegrees);
    void reset();

    void setFFTBlockSize(int newBlockSize);

    void processBlock(const float* input, float* outputL, float* outputR, int numSamples);
private:

};