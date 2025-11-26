/*
  ==============================================================================

    PartitionedConvolver.cpp
    Created: 25 Nov 2025 7:42:14pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "PartitionedConvolver.h"

PartitionedConvolver::PartitionedConvolver(int partitionSize) 
    : partitionSize(partitionSize),
    fftSize(juce::nextPowerOfTwo(partitionSize * 2)),
    fft(static_cast<int>(std::log2(fftSize))) 
{
    inputBuffer.setSize(1, partitionSize);
    inputBuffer.clear();
    inputBufferPos = 0;

    overlapBuffer.setSize(1, fftSize);
    overlapBuffer.clear();

    irPartitions.clear();
    numPartitions = 0;

    DBG("PartitionedConvolver: partitionSize=" << partitionSize << " fftSize=" << fftSize);
}

void PartitionedConvolver::loadIR(const juce::AudioBuffer<float>& irBuffer) {
    irPartitions.clear();

    int irLength = irBuffer.getNumSamples();
    const float* readPointer = irBuffer.getReadPointer(0);
    
    numPartitions = (irLength + partitionSize - 1) / partitionSize;

    std::vector<juce::dsp::Complex<float>> fftData(fftSize);

    for (int p = 0; p < numPartitions; ++p) {
        int offset = p * partitionSize;
        for (int i = 0; i < partitionSize; ++i) {
            if (offset + i < irLength)
                fftData[i] = { readPointer[offset + i], 0.0f };
            else
                fftData[i] = { 0.0f, 0.0f };
        }
        for (int i = partitionSize; i < fftSize; ++i) {
            fftData[i] = { 0.0f, 0.0f };
        }

        fft.perform(fftData.data(), fftData.data(), false);
        irPartitions.push_back(fftData);
    }

    inputFFTBuffer.clear();
    inputFFTBuffer.resize(numPartitions, std::vector<juce::dsp::Complex<float>>(fftSize));
    inputFFTBufferPos = 0;
    
    overlapBuffer.setSize(1, fftSize);
    overlapBuffer.clear();

    DBG("PartitionedConvolver: loaded IR - partitions=" << numPartitions);
}

void PartitionedConvolver::processBlock(const float* input, float* output, int numSamples) {
    // Copy input into buffer
    int samplesProcessed = 0;

    while (samplesProcessed < numSamples) {
        int samplesFree = partitionSize - inputBufferPos;
        int samplesToCopy = std::min(samplesFree, numSamples - samplesProcessed);

        inputBuffer.copyFrom(0, inputBufferPos, input + samplesProcessed, samplesToCopy);
        inputBufferPos += samplesToCopy;

        // Process FFT once enough samples are in buffer
        if (inputBufferPos == partitionSize) {
            DBG("Processing partition");
            processPartition();
            inputBufferPos = 0;
        }

        // Output processed samples
        auto* overlapReadPtr = overlapBuffer.getReadPointer(0);
        for (int i = 0; i < samplesToCopy; ++i) {
            output[samplesProcessed + i] = overlapReadPtr[i];
        }

        // Shift overlap buffer by samplesToCopy
        auto* overlapWritePtr = overlapBuffer.getWritePointer(0);
        int remaining = fftSize - samplesToCopy;
        std::memmove(overlapWritePtr, overlapWritePtr + samplesToCopy, remaining * sizeof(float));
        overlapBuffer.clear(0, fftSize - samplesToCopy, samplesToCopy);

        samplesProcessed += samplesToCopy;
    }
}

void PartitionedConvolver::processPartition() {

    // Convert input buffer to Complex
    std::vector<juce::dsp::Complex<float>> inputFFT(fftSize);
    auto* inputReadPtr = inputBuffer.getReadPointer(0);

    for (int i = 0; i < partitionSize; ++i) {
        inputFFT[i] = juce::dsp::Complex<float>(inputReadPtr[i], 0.0f);
    }
    // Zero-pad input to partition size
    for (int i = partitionSize; i < fftSize; ++i) {
        inputFFT[i] = juce::dsp::Complex<float>(0.0f, 0.0f);
    }
    // Perform FFT on input
    fft.perform(inputFFT.data(), inputFFT.data(), false);

    // Store input FFT on buffer
    inputFFTBuffer[inputFFTBufferPos] = inputFFT;

    // Convolution - input fft blocks convolved with IR partitions in frequency domain
    std::vector<juce::dsp::Complex<float>> accumulator(fftSize, { 0.0f, 0.0f });

    for (int p = 0; p < numPartitions; ++p) {

        int inputBlockIndex = (inputFFTBufferPos - p + numPartitions) % numPartitions;
        auto& inputPartition = inputFFTBuffer[inputBlockIndex];
        auto& irPartition = irPartitions[p];

        for (int i = 0; i < fftSize; ++i) {
            accumulator[i] += inputPartition[i] * irPartition[i]; // Complex multiplication
        }
    }

    // Perform inverse FFT on accumulator
    fft.perform(accumulator.data(), accumulator.data(), true);

    // Scale inverse FFT by N
    for (int i = 0; i < fftSize; ++i) {
        accumulator[i] /= static_cast<float>(fftSize);
    }

    // Overlap-add 
    auto* overlapPtr = overlapBuffer.getWritePointer(0);

    for (int i = 0; i < fftSize; ++i) {
        overlapPtr[i] += accumulator[i].real();
    }

    // increment input block buffer position
    inputFFTBufferPos = (inputFFTBufferPos + 1) % numPartitions;
}