/*
  ==============================================================================

    WaveformDisplay.h
    Created: 20 Mar 2022 11:37:22am
    Author:  Shang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener   
{
public:

    WaveformDisplay(juce::AudioFormatManager* formatManagerToUse,juce::AudioThumbnailCache& cacheToUse,DJAudioPlayer* _player);

    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void loadURL(juce::URL audioURL);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void setPositionRelative(double pos);

    void setTime(double time);

    void mouseDown(const juce::MouseEvent& event) override;

    void mouseDrag(const juce::MouseEvent& event) override;

    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;

private:

    DJAudioPlayer* player;

    juce::AudioThumbnail audioThumbnail;

    bool fileLoaded = false;

    double playheadPos;

    double timeCode;
    
    juce::String timeString;

    int loopControlsHeight;

    double  loopControlsWidth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
