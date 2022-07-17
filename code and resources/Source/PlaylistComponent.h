/*
  ==============================================================================

    PlaylistComponent.h
    Created: 17 Apr 2022 6:26:44pm
    Author:  Shang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "TrackData.h"
#include <vector>
#include <string>
#include <fstream>

class PlaylistComponent : public juce::Component, 
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::TextEditor::Listener,
                          public juce::FileDragAndDropTarget
{

public:

    PlaylistComponent(DeckGUI* _deck1,DeckGUI* _deck2);

    ~PlaylistComponent() override;

    void paint(juce::Graphics&) override;

    void resized() override;

    void buttonClicked(juce::Button* button) override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&,int rowNumber,int width,int height,bool rowIsSelected) override;
    
    void paintCell(juce::Graphics&,int rowNumber,int columnId,int width,int height,bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,int columnId,bool isRowSelected,Component* existingComponentToUpdate) override;

    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;

    void searchForTrack(juce::String searchInput);

    void addToTrackDatas(juce::URL audioURL, juce::String duration);

    bool trackFound;

    std::vector<TrackData>* getTrackDatas();

    juce::TableListBox tableComponent;

    std::vector<std::string> tokenise(std::string Line, char separator);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void savePlaylist();

    void loadPlaylist();

    void loadPlaylistAtStart();
   
    void updatePlaylist();

private:

    DeckGUI* deck1;
    DeckGUI* deck2;

    juce::ImageButton saveButton;
    juce::ImageButton loadLibraryButton;
    juce::ImageButton insertButton;

    juce::Label label;

    juce::TextEditor search{ "search", 0 };

    std::vector<TrackData> trackDatas;
    std::vector<TrackData> trackDatasFound;

    std::ofstream playlistSave;
    std::string playlistUrlString;
    juce::String urljuce;
    std::string urlstd;
    std::string filenamestd;
    std::string durationstd;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};