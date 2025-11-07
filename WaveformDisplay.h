

#pragma once

#include <JuceHeader.h>

class WaveformDisplay : public juce::Component,
    public juce::ChangeListener
{
public:
    // --- Listener for seek events ---
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void waveformClicked(double normalizedPosition) = 0;
    };

    WaveformDisplay(juce::AudioFormatManager& formatManager);
    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    // --- Mouse events for seeking ---
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    // --- Public methods ---
    void addListener(Listener* l);
    void loadFile(const juce::File& file);
    void setPosition(double normalizedPosition);

private:
    Listener* listener{ nullptr };

    
    std::unique_ptr<juce::AudioThumbnailCache> thumbnailCache;
   
    juce::AudioThumbnail thumbnail;

    double playheadPosition{ 0.0 };

    juce::String formatTime(double seconds);
    double currentMousePosSeconds{ 0.0 };
    bool isDragging{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};