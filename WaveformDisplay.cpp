

#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager)
    : thumbnailCache(std::make_unique<juce::AudioThumbnailCache>(5)), 
    thumbnail(512, formatManager, *thumbnailCache) 
{
    thumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
    thumbnail.removeChangeListener(this);
}

void WaveformDisplay::addListener(Listener* l)
{
    listener = l;
}

void WaveformDisplay::loadFile(const juce::File& file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
}

void WaveformDisplay::setPosition(double normalizedPosition)
{
    playheadPosition = normalizedPosition;
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour::fromRGB(40, 50, 90).brighter(0.2f));

    // Waveform
    if (thumbnail.isFullyLoaded())
    {
        g.setColour(juce::Colours::cornflowerblue);
        thumbnail.drawChannels(g,
            getLocalBounds(),
            0.0,
            thumbnail.getTotalLength(),
            1.0f);
    }
    else
    {
        g.setColour(juce::Colours::lightsteelblue);
        g.drawText("Loading waveform...", getLocalBounds(), juce::Justification::centred);
    }

    // Playhead
    g.setColour(juce::Colours::whitesmoke.withAlpha(0.8f));
    auto playheadX = (int)(playheadPosition * (double)getWidth());
    g.drawVerticalLine(playheadX, 0.0f, (float)getHeight());

    // Time labels
    juce::String totalTimeStr = formatTime(thumbnail.getTotalLength());
    juce::String currentTimeStr = formatTime(playheadPosition * thumbnail.getTotalLength());

    g.setColour(juce::Colours::lightsteelblue);
    g.setFont(12.0f);
    g.drawText(currentTimeStr, getLocalBounds().reduced(4, 0), juce::Justification::centredLeft);
    g.drawText(totalTimeStr, getLocalBounds().reduced(4, 0), juce::Justification::centredRight);

    // Show time under mouse when dragging
    if (isDragging)
    {
        juce::String mouseTimeStr = formatTime(currentMousePosSeconds);
        auto mouseX = (int)(currentMousePosSeconds / thumbnail.getTotalLength() * getWidth());

        g.setColour(juce::Colours::white);
        g.drawVerticalLine(mouseX, 0.0f, (float)getHeight());
        g.drawText(mouseTimeStr, mouseX + 5, 5, 50, 20, juce::Justification::centredLeft);
    }
}

void WaveformDisplay::resized()
{
    // N/A
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint();
    }
}

void WaveformDisplay::mouseDown(const juce::MouseEvent& e)
{
    isDragging = true;
    mouseDrag(e); // Handle click as a drag
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent& e)
{
    if (listener != nullptr && thumbnail.isFullyLoaded())
    {
        auto normalizedPos = juce::jlimit(0.0, 1.0, (double)e.position.x / (double)getWidth());
        currentMousePosSeconds = normalizedPos * thumbnail.getTotalLength();
        listener->waveformClicked(normalizedPos);
        repaint();
    }
}

void WaveformDisplay::mouseUp(const juce::MouseEvent& e)
{
    isDragging = false;
    repaint();
}

juce::String WaveformDisplay::formatTime(double seconds)
{
    int totalSecs = static_cast<int>(std::round(seconds));
    int mins = totalSecs / 60;
    int secs = totalSecs % 60;
    return juce::String::formatted("%02d:%02d", mins, secs);
}