/*
  ==============================================================================

    SoundSource.h
    Created: 26 Nov 2025 3:20:57pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

// Abstract base class for tone generator, noise generator, and sound file players
class SoundSource {
public:
    virtual ~SoundSource() = default;
    virtual void process(float* outputL, float* outputR, int numSamples) = 0;
    virtual bool isFinished() const = 0;
};