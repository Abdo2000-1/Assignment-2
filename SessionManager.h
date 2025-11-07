#pragma once
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

// Handles saving & loading session data for multiple audio files
class SessionManager
{
public:
    SessionManager(const juce::File& jsonFile);

    void saveSession(const juce::String& fileId,
        double positionSeconds);

    juce::var getSession(const juce::String& fileId);

private:
    juce::File file;
    juce::var data;

    void load();
    void save();
};
