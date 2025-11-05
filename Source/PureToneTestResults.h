/*
  ==============================================================================

    PureToneTestResults.h
    Created: 5 Nov 2025 5:43:08pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

struct PureToneTestResults {
    std::map<float, float> left;
    std::map<float, float> right;

    std::map<float, float>& operator[](size_t index) {
        if (index == 0) return left;
        if (index == 1) return right;
        throw std::out_of_range("Index must be 0 or 1");
    }

    const std::map<float, float>& operator[](size_t index) const {
        if (index == 0) return left;
        if (index == 1) return right;
        throw std::out_of_range("Index must be 0 or 1");
    }
};