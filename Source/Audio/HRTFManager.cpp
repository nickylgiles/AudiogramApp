/*
  ==============================================================================

    HRTFManager.cpp
    Created: 19 Nov 2025 11:12:50pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "HRTFManager.h"

HRTFManager::HRTFManager() {
    audioFormatManager.registerBasicFormats();
}

/*
    Expects HRIRs with name format HRIR_AZ_C.wav
    AZ = azimuth, C = Channel (L or R)
*/
void HRTFManager::loadBinaryData() {
    irMap.clear();
    for (int i = 0; i < BinaryData::namedResourceListSize; ++i) {
        auto name = juce::String(BinaryData::namedResourceList[i]);
        if (!name.startsWith("HRIR_"))
            continue;
        
        auto tokens = juce::StringArray::fromTokens(name, "_", "");
        if (tokens.size() < 3)
            continue;

        int azimuth = tokens[1].getIntValue();
        char channel = tokens[2][0];

        if (channel == 'L') {
            loadIR(name, irMap[azimuth].left);
        }
        else {
            loadIR(name, irMap[azimuth].right);
        }

    }
    DBG(irMap.size() << " HRIR pairs loaded.");
}

void HRTFManager::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
}

const double HRTFManager::getIRSampleRate() {
    return irSampleRate;
}

juce::AudioBuffer<float>& HRTFManager::getIR(float azimuth, int channel) {
    while (azimuth < 0.0f) azimuth += 360.0f;
    while (azimuth >= 360.0f) azimuth -= 360.0f;

    int azInt = static_cast<int>(std::round(azimuth));
    auto it = irMap.lower_bound(azInt);

    if (it == irMap.end())
        it = std::prev(it);
    else if (it != irMap.begin()) {
        int lower = std::prev(it)->first;
        int upper = it->first;
        if (std::abs(azimuth - lower) < std::abs(azimuth - upper))
            it = std::prev(it);
    }
    DBG("Get IR: az = " << azimuth << ". IR az = " << it->first);

    if (channel == 0) return it->second.left;
    else return it->second.right;
}

bool HRTFManager::loadIR(const juce::String& name, juce::AudioBuffer<float>& dest) {

    int size = 0;
    auto* data = BinaryData::getNamedResource(name.toRawUTF8(), size);
    if (data == nullptr || size <= 0) {
        DBG("Could not find HRIR " << name);
        return false;
    }
    auto inputStream = std::make_unique<juce::MemoryInputStream>(data, size, false);

    std::unique_ptr<juce::AudioFormatReader> reader(
        audioFormatManager.createReaderFor(std::move(inputStream)));

    if (reader == nullptr) {
        DBG("Failed to load binary resource " << name);
        return false;
    }

    int length = reader->lengthInSamples;
    dest.setSize(1, length);
    reader->read(&dest, 0, length, 0, true, false);

    DBG("HRIR loaded. Length = " << length << " samples");


    irSampleRate = reader->sampleRate;

    DBG("HRIR sample rate: " << irSampleRate);

    return true;
}
