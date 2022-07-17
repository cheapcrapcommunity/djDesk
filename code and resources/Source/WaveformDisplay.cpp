/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 20 Mar 2022 11:37:22am
    Author:  Shang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay (juce::AudioFormatManager* formatManagerToUse,juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer* _player)
    ://initialise everything
    audioThumbnail(1000, *formatManagerToUse, cacheToUse),
    fileLoaded(false),
    playheadPos(0.),
    timeCode(0),
    player{ _player },
    loopControlsHeight(0.),
    loopControlsWidth(0.)
{
    audioThumbnail.addChangeListener(this);   
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);

    //set height of waveform display
    int waveHeight{ getHeight() - (loopControlsHeight * 2) }; 

    //set bounding boxes
    juce::Rectangle<int> startBox{ 0, 0, getWidth(), loopControlsHeight }; 
    juce::Rectangle<int> waveBox{ 0, loopControlsHeight, getWidth(), waveHeight }; 
    juce::Rectangle<int> endBox{ 0, loopControlsHeight + waveHeight, getWidth(), loopControlsHeight };

    g.setColour(juce::Colours::whitesmoke); 
    g.fillRect(startBox);
    g.fillRect(endBox);

    if (fileLoaded)
    {
        if (player->getLoopStart() > 0 || player->getLoopEnd() < 1)
        {
            //draw the looping area waveform
            g.setColour(juce::Colours::lightblue);
            audioThumbnail.drawChannel(g, waveBox, 0.0, audioThumbnail.getTotalLength(), 0, 1.0f);
            //looping area bounding box
            juce::Rectangle<int> loopBox(static_cast<int>(player->getLoopStart() * getWidth()),
                loopControlsHeight,
                static_cast<int>(player->getLoopEnd() * getWidth()- player->getLoopStart() * getWidth()),
                waveHeight);

           g.saveState();

           //draw looping area in different colour
            g.setColour(juce::Colours::hotpink);
            g.reduceClipRegion(loopBox);
            audioThumbnail.drawChannel(g, waveBox, 0.0, audioThumbnail.getTotalLength(), 0, 1.0f);

            g.restoreState();
        }
        else
        {
            //draw the whole waveform
            g.setColour(juce::Colours::hotpink);
            audioThumbnail.drawChannel(g, waveBox, 0.0, audioThumbnail.getTotalLength(), 0, 1.0f);
        }

        //set up start triangle for loop control
        juce::Path startTri;
        startTri.addTriangle((player->getLoopStart() * getWidth()), (loopControlsHeight - 1),
            (player->getLoopStart() * getWidth() - loopControlsWidth), 1.0f,
            (player->getLoopStart() * getWidth() + loopControlsWidth), 1.0f
        ); 

        //set up end triangle for loop control
        juce::Path endTri;
        endTri.addTriangle((player->getLoopEnd() * getWidth()), (loopControlsHeight + waveHeight + 1),
            (player->getLoopEnd() * getWidth() - loopControlsWidth),(getHeight() - 1),
            (player->getLoopEnd() * getWidth() + loopControlsWidth),(getHeight() - 1)
        ); 

        //draw the triangles
        g.setColour(juce::Colours::dodgerblue);
        g.fillPath(startTri);
        g.fillPath(endTri);
        g.setColour(juce::Colours::hotpink);
        juce::PathStrokeType stroke{1};
        g.strokePath(startTri, stroke);
        g.strokePath(endTri, stroke);

        //draw loop start/end lines on waveform
        g.drawLine((player->getLoopStart() * getWidth()),
            (loopControlsHeight),
            (player->getLoopStart() * getWidth()),
            (getHeight() - loopControlsHeight)
        );

        g.drawLine((player->getLoopEnd() * getWidth()),
            (loopControlsHeight),
            (player->getLoopEnd() * getWidth()),
            (getHeight() - loopControlsHeight)
        );

        //draw playhead
        g.setColour(juce::Colours::dodgerblue);
        g.drawLine((playheadPos * getWidth()),
            (loopControlsHeight),
            (playheadPos * getWidth()),
            static_cast<float>(getHeight() - loopControlsHeight),
            2);

        //draw current playing time
        g.setColour(juce::Colours::blueviolet);
        juce::String timeString(timeCode, 2);
        g.drawText(timeString, getLocalBounds(), juce::Justification::centred, true);
    }
    else
    { 
        //set indication while starting
        g.setFont(juce::Font(16.0f, juce::Font::italic));
        g.setColour(juce::Colours::dodgerblue);
        g.drawText("Hey~ You can drop an audio file here", waveBox, juce::Justification::centred, true);
       
    }

    g.setColour(juce::Colours::whitesmoke);
    g.drawRect(getLocalBounds(), 0.3);
    g.drawRect(waveBox, 1);
}

void WaveformDisplay::resized()
{   
    loopControlsHeight = juce::jmax(getHeight() / 10, 10);
    loopControlsWidth = loopControlsHeight ; 
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    //update if playhead move
    if (fileLoaded && pos != playheadPos)
    {
        playheadPos = pos;
        repaint();
    }
}

void WaveformDisplay::setTime(double time) {
    if (time > 0. && time != timeCode)
    {
        timeCode = time;
    }
}

void WaveformDisplay::mouseDown(const juce::MouseEvent& event)
{
    //set mouse down control for loop
    int yStart{ event.getMouseDownY() };

    double ratio{ double(event.getMouseDownX()) / double(getWidth()) };

    if (yStart <= loopControlsHeight)
    {
        player->setLoopStart(ratio);
        repaint();
    }
    else if (yStart >= getHeight() - loopControlsHeight)
    {
        player->setLoopEnd(ratio);
        repaint();
    }
    else
    {
        player->setPositionRelative(ratio);
        repaint();
    }
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent& event)
{
    //set mouse drag control for loop
    int xStart{ event.getMouseDownX() };
    int yStart{ event.getMouseDownY() };
    int xDist{ event.getDistanceFromDragStartX() };
    int xCurr{ xStart + xDist };

    double ratio{ double(xCurr) / double(getWidth()) };

    if (yStart <= loopControlsHeight)
    {
        player->setLoopStart(ratio);
        repaint();
    }
    else if (yStart >= getHeight() - loopControlsHeight)
    {
        player->setLoopEnd(ratio);
        repaint();
    }
    else
    {
        player->setPositionRelative(ratio);
        repaint();
    }
}

void WaveformDisplay::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    //set mouse wheel control for moving the playhead within the loop area
    if (event.getMouseDownY() <= loopControlsHeight)
    {
        player->setLoopStart(player->getLoopStart() + wheel.deltaY / 200);
        repaint();
    }
    else if (event.getMouseDownY() >= getHeight() - loopControlsHeight)
    {
        player->setLoopEnd(player->getLoopEnd() + wheel.deltaY / 200);
        repaint();
    }
    else
    {
        player->setPositionRelative(player->getPositionRelative() + wheel.deltaY / 50);
    }
}

