/*
  ==============================================================================

    Envelope.h
    Created: 29 Oct 2025 8:41:59pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

class Envelope {
public:
    void start(int numSamples);
    void setRiseTime(int numSamples);
    void setFallTime(int numSamples);
    void stop();
    float nextSample();
private:
    int duration = 0;
    int currentSample = 0;
    int riseTime = 0;
    int fallTime = 0;
    bool active = false;
};