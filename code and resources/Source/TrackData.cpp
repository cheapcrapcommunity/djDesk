/*
  ==============================================================================

    TrackData.cpp
    Created: 19 Apr 2022 3:02:01pm
    Author:  Shang

  ==============================================================================
*/

#include "TrackData.h"
#include <string>

TrackData::TrackData(juce::URL _audioURL, juce::String _duration)
    : audioURL(_audioURL)
{
    //track title
    title = juce::URL::removeEscapeChars(audioURL.getFileName());
    //track duration
    duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');
}

