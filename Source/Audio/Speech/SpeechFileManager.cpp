/*
  ==============================================================================

    SpeechFileManager.cpp
    Created: 29 Nov 2025 1:28:44pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SpeechFileManager.h"

SpeechFileManager::SpeechFileManager() {
    loadBinaryData();
}

void SpeechFileManager::loadBinaryData() {
    for (int i = 0; i < BinaryData::namedResourceListSize; ++i) {
        juce::String name(BinaryData::namedResourceList[i]);

        if (!name.startsWith("speech_")) {
            continue;
        }

        auto tokens = juce::StringArray::fromTokens(name, "_", "");
        if (tokens.size() < 3)
            continue;

        int digit = tokens[1].getIntValue();
        int size = 0;

        SpeechResource res;

        res.data = BinaryData::getNamedResource(name.toRawUTF8(), size);

        if(res.data == nullptr || size<=0) {
            DBG("Failed to load BinaryData resource: " << name);
            continue;
        }

        res.dataSize = (size_t)size;

        digitsMap[digit] = res;
    }
}

const SpeechFileManager::SpeechResource& SpeechFileManager::getDigit(int digit) {
    auto it = digitsMap.find(digit);
    if (it == digitsMap.end()) {
        throw std::out_of_range("Digit not found: " + std::to_string(digit));
    }

    return it->second;
}
