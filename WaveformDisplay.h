#pragma once
#include <JuceHeader.h>

class WaveformDisplay : public juce::Component,
    public juce::ChangeListener,
    private juce::Timer
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManager);
    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadFile(const juce::File& audioFile);
    void setTransportSource(juce::AudioTransportSource* transport);

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;

    std::function<void(double)> onPositionChange;

    void setCueMarker(double normalizedPosition);
    void clearCueMarker();

    
    void setTheme(juce::Colour background, juce::Colour waveform, juce::Colour cueMarker, juce::Colour playhead);
   

private:
    void timerCallback() override;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;

    juce::AudioTransportSource* transportSource{ nullptr };
    double playheadPosition = 0.0; // 0.0 to 1.0
    double cuePointPosition = 0.0; // 0.0 to 1.0

   
    juce::Colour bgColour;
    juce::Colour waveColour;
    juce::Colour cueColour;
    juce::Colour playheadColour;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};