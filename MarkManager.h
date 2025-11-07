#pragma once
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

// Handles saving & loading marks (timestamps) for multiple audio files
class MarkManager
{
public:
    MarkManager(const juce::File& jsonFile);

    void addMark(const juce::String& fileId, double seconds);
    void removeMark(const juce::String& fileId, int index);
    juce::Array<double> getMarks(const juce::String& fileId);

private:
    juce::File file;
    juce::var data;

    void load();
    void save();
};
