/*
  ==============================================================================

    TrackData.h
    Created: 19 Apr 2022 3:02:01pm
    Author:  Shang

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class TrackData 
{
public:

    TrackData(juce::URL _audioURL, juce::String _duration);

    juce::String title;
    juce::String duration;
    juce::URL audioURL;

};
