
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TrackData.h"

class PlaylistComponent;

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer     
{
    public :
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager* formatManagerToUse,juce::AudioThumbnailCache& cacheToUse,PlaylistComponent* _playlistComponent);
    ~DeckGUI();

    void paint(juce::Graphics&) override;

    void resized() override;

    void buttonClicked(juce::Button*) override;

    void sliderValueChanged(juce::Slider*) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

    juce::ImageButton myPlayButton;

    juce::Slider volSlider;

    juce::Slider speedSlider;
    
private:

    juce::Slider posSlider;

    juce::Label  volLabel;
    juce::Label  speedLabel;

    juce::ImageButton myStopButton;
    juce::ImageButton myLoadButton;
    juce::ImageButton myLoopButton;
    juce::ImageButton myForwardButton;
    juce::ImageButton myBackwardButton;

    PlaylistComponent* playlistComponent;

    std::vector<TrackData>* trackDatas;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
