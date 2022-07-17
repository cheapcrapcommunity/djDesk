/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 17 Apr 2022 6:26:44pm
    Author:  Shang

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(DeckGUI* _deck1, DeckGUI* _deck2) :
    trackFound(false),
    deck1(_deck1),
    deck2(_deck2) {
    //three icons' image
    auto saveImage = juce::ImageCache::getFromMemory(BinaryData::save_png, BinaryData::save_pngSize);
    auto loadLibraryImage = juce::ImageCache::getFromMemory(BinaryData::folder_png, BinaryData::folder_pngSize);
    auto insertImage = juce::ImageCache::getFromMemory(BinaryData::insert_png, BinaryData::insert_pngSize);
    //set images for three buttons
    saveButton.setImages(true, true, true, saveImage, 1, juce::Colours::lightgrey, juce::Image(nullptr), 1, juce::Colours::dodgerblue, juce::Image(nullptr), 1, juce::Colours::hotpink);
    loadLibraryButton.setImages(true, true, true, loadLibraryImage, 1, juce::Colours::lightgrey, juce::Image(nullptr), 1, juce::Colours::dodgerblue, juce::Image(nullptr), 1, juce::Colours::hotpink);
    insertButton.setImages(true, true, true, insertImage, 1, juce::Colours::lightgrey, juce::Image(nullptr), 1, juce::Colours::dodgerblue, juce::Image(nullptr), 1, juce::Colours::hotpink);

    //set up saveButton and tooltip
    addAndMakeVisible(saveButton);
    saveButton.setTooltip("Save Playlist");
    saveButton.addListener(this);

    //set up loadLibraryButton and tooltip
    addAndMakeVisible(loadLibraryButton);
    loadLibraryButton.addListener(this);
    loadLibraryButton.setTooltip("Load Playlist");

    //set up insertButton and tooltip
    addAndMakeVisible(insertButton);
    insertButton.addListener(this);
    insertButton.setTooltip("Insert track to Playlist");

    //set up playList label
    addAndMakeVisible(label);
    label.setText(" Playlist ", juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    label.toBack();

    //set up search bar
    addAndMakeVisible(search);
    search.addListener(this);
    search.onReturnKey = [this] {searchForTrack(search.getText()); };
    search.setTextToShowWhenEmpty("Search", juce::Colours::white);
    search.setJustification(juce::Justification::centredLeft);
    search.setColour(juce::TextEditor::backgroundColourId, juce::Colours::lightgrey);
    //set up tableComponent
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    tableComponent.setColour(juce::ListBox::backgroundColourId, juce::Colour::fromRGB(218, 229, 245));

    //set header for 5 columns 
    tableComponent.getHeader().addColumn("Title", 1, 400);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Track 1", 3, 100);
    tableComponent.getHeader().addColumn("Track 2", 4, 100);
    tableComponent.getHeader().addColumn("Trash", 5, 100);

    playlistUrlString = "myPlayList_2022_4_21_1_3.txt";
    loadPlaylistAtStart();
    //tableComponent.updateContent();
}

PlaylistComponent::~PlaylistComponent() {

}

void PlaylistComponent::paint(juce::Graphics& g) {

    g.fillAll(juce::Colours::lightslategrey);

}

void PlaylistComponent::resized() {

    double rowH = getHeight() / 8;
    double colW = getWidth() / 14;

    tableComponent.setBounds(0, rowH * 0.8, getWidth(), getHeight());

    label.setBounds(0, rowH * 0.2, getWidth(), rowH * 0.4);

    search.setBounds(getWidth() * 0.7, rowH * 0.1, getWidth() * 0.3, rowH * 0.6);
    saveButton.setBounds(0, rowH * 0.2, colW, rowH * 0.35);

    loadLibraryButton.setBounds(colW, rowH * 0.2, colW, rowH * 0.4);
    insertButton.setBounds(colW * 2, rowH * 0.2, colW, rowH * 0.4);

}

int PlaylistComponent::getNumRows()
{
    return trackDatas.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {

    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::hotpink);//highlight the selected row
    }
    else {
        g.fillAll(juce::Colours::whitesmoke);//set row background color
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    //write tracks' titles and duratiosn into corresponding cells
    if (trackFound)
    {
        if (columnId == 1) //title
            g.drawText(trackDatasFound[rowNumber].title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        if (columnId == 2) //duration
            g.drawText(trackDatasFound[rowNumber].duration, 2, 0, width - 4, height, juce::Justification::horizontallyCentred, true);
    }
    else
    {
        if (columnId == 1) //title
            g.drawText(trackDatas[rowNumber].title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        if (columnId == 2) //duration
            g.drawText(trackDatas[rowNumber].duration, 2, 0, width - 4, height, juce::Justification::horizontallyCentred, true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    //set up buttons for loading the tracks to deck1/2
    if (columnId == 3 || columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Cue" };
            if (columnId == 3) {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colours::dodgerblue);
            }
            if (columnId == 4) {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colours::hotpink);
            }
            juce::String id{ std::to_string(rowNumber) + std::to_string(columnId) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    //set up delete button
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Delete" };
            //unique id -> rowNumber + columnId
            juce::String id{ std::to_string(rowNumber) + std::to_string(columnId) };
            btn->setComponentID(id);
            //add itself as a button listener
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    //set up the saveButton to save playlist (into the build file)
    if (button == &saveButton)
    {
        savePlaylist();
    }
    //set up the loadLibraryButton to load playList
    else if (button == &loadLibraryButton)
    {
        loadPlaylist();
    }
    //insert a track into trackData and current playlist
    else if (button == &insertButton)
    {
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            juce::URL audioURL = juce::URL{ chooser.getResult() };
            addToTrackDatas(audioURL, deck1->player->getTrackDurationWithoutLoadingIntoDeck(audioURL));
        }
    }
    //set up load to deck1/2 buttons and delete button for each track
    else
    {
        int rowIndex = std::stoi(button->getComponentID().toStdString().substr(0, 1));
        int colIndex = std::stoi(button->getComponentID().toStdString().substr(1, 2));

        juce::URL trackToPlay;

        if (trackFound)
        {
            trackToPlay = trackDatasFound[rowIndex].audioURL;
        }
        else
        {
            trackToPlay = trackDatas[rowIndex].audioURL;
        }

        //load tracks into different decks

        //reset and load to deck 1
        if (colIndex == 3)
        {
            deck1->myPlayButton.setToggleState(false, false);
            deck1->player->loadURL(trackToPlay);
            deck1->waveformDisplay.loadURL(trackToPlay);
            deck1->waveformDisplay.setPositionRelative(deck1->player->getPositionRelative());
            deck1->player->setLoopStart(0.);
            deck1->player->setLoopEnd(1.);
            deck1->speedSlider.setValue(1.);
            deck1->volSlider.setValue(0.5);
        }

        //reset and load to deck 2
        if (colIndex == 4)
        {
            deck2->myPlayButton.setToggleState(false, false);
            deck2->player->loadURL(trackToPlay);
            deck2->waveformDisplay.loadURL(trackToPlay);
            deck2->waveformDisplay.setPositionRelative(deck1->player->getPositionRelative());
            deck2->player->setLoopStart(0.);
            deck2->player->setLoopEnd(1.);
            deck2->speedSlider.setValue(1.);
            deck2->volSlider.setValue(0.5);
        }

        //delete the track from trackDatas
        if (colIndex == 5)
        {
            trackDatas.erase(trackDatas.begin() + rowIndex);
            tableComponent.updateContent();

            updatePlaylist();

        }
    }
}

void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    //set up search bar
    juce::String searchInput = search.getText();
    searchForTrack(searchInput);
    tableComponent.updateContent();
    repaint();
    trackDatasFound.clear();
}

void PlaylistComponent::searchForTrack(juce::String searchInput)
{
    trackFound = false;

    if (searchInput == "") {
        tableComponent.deselectAllRows();
    }
    else {
        //select the row contains matching text with user input
        for (int i = 0; i < trackDatas.size(); i++) {
            if (trackDatas[i].title.contains(searchInput)) {
                tableComponent.selectRow(i, false, false);
            }
        }
    }
}

void PlaylistComponent::addToTrackDatas(juce::URL audioURL, juce::String duration)
{
    //collect the url and duration info of the new added tracks into trackDatas
    
    trackDatas.push_back(TrackData{ audioURL, duration });
    tableComponent.updateContent();

    updatePlaylist();

}

std::vector<TrackData>* PlaylistComponent::getTrackDatas()
{
    return &trackDatas;
}

std::vector<std::string> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
    //tokenise the playlist txt file
    std::vector<std::string> tokens;
    signed int begin, end;
    std::string token;
    begin = csvLine.find_first_not_of(separator, 0);
    do {
        end = csvLine.find_first_of(separator, begin);
        if (begin == csvLine.length() || begin == end) break;
        if (end >= 0) token = csvLine.substr(begin, end - begin);
        else token = csvLine.substr(begin, csvLine.length() - begin);
        tokens.push_back(token);
        begin = end + 1;
    } while (end > 0);

    return tokens;
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files) {
    //enable fileDrag in playlistComponent
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {

    if (files.size() >= 1)
    {
        //get dropped files' data and add to the trackDatas
        for (int i = 0; i < files.size(); i++) {

            std::string tempName = juce::URL{ juce::File{ files[i] } }.toString(false).toStdString();
            std::string exten = tempName.substr(tempName.find_last_of(".") + 1);
            if (exten == "mp3" || exten == "wav")
            {
                DBG("Loading Drag Data" + tempName);
                addToTrackDatas(juce::URL{ juce::File{ files[i] } }, deck1->player->getTrackDurationWithoutLoadingIntoDeck(juce::URL{ juce::File{ files[i] } }));
            }
            else {
                DBG("Not Loading Drag Data" + tempName);
            }

        }
        tableComponent.updateContent();
    }
}

void PlaylistComponent::savePlaylist() {

    //set local time as playlist title format
    auto now = std::time(0);
    std::string dt = ctime(&now);
    tm* ltm = localtime(&now);
    std::string yr = std::to_string(1900 + ltm->tm_year);
    std::string mon = std::to_string(1 + ltm->tm_mon);
    std::string mday = std::to_string(ltm->tm_mday);
    std::string hs = std::to_string(ltm->tm_hour);
    std::string mins = std::to_string(ltm->tm_min);
    //set playlist as text file
    playlistSave.open("myPlayList_" + yr + "_" + mon + "_" + mday + "_" + hs + "_" + mins + ".txt", std::ios::out | std::ios::app);
    //write track url and duration into the playlist
    for (int i = 0; i < trackDatas.size(); ++i) {
        urljuce = trackDatas[i].audioURL.toString(false);
        urlstd = urljuce.toStdString();
        durationstd = trackDatas[i].duration.toStdString();
        playlistSave << urlstd << "," << durationstd << std::endl;
    }
 
}

void PlaylistComponent::loadPlaylist() {

    trackFound = false;
    juce::FileChooser chooser{ "Select a file..." };

    if (chooser.browseForFileToOpen())
    {
        juce::URL playListUrl = chooser.getResult().getFileName().toStdString();


        if (!playListUrl.isEmpty()) {
            std::string filenameStr = playListUrl.toString(false).toStdString();

            if (filenameStr.substr(filenameStr.find_last_of(".") + 1) == "txt") {

                playlistUrlString = filenameStr;

                DBG("Valid File " + playlistUrlString);
            }
            else {
                DBG("Invalid File" + filenameStr);
            }

        }


        std::ifstream playlistLoad{ chooser.getResult().getFileName().toStdString() };
        std::string line;
        std::vector<std::string> twoData;
        if (playlistLoad.is_open()) {

            trackDatas.clear();

            while (std::getline(playlistLoad, line)) {
                try {
                    twoData = tokenise(line, ',');
                    DBG("good data");
                    
                    addToTrackDatas(juce::URL{ twoData[0] }, twoData[1]);
                }
                catch (const std::exception& e) {
                    DBG("bad data");
                }
            }
        }
    }
    tableComponent.updateContent();
   
    
    
    DBG("Load Playlist: "+playlistUrlString);
}


void PlaylistComponent::loadPlaylistAtStart() {

    trackFound = false;
    trackDatas.clear();

    std::ifstream playlistLoad{playlistUrlString};
    std::string line;
    std::vector<std::string> twoData;
    if (playlistLoad.is_open()) {
        while (std::getline(playlistLoad, line)) {
            try {
                twoData = tokenise(line, ',');
                //DBG("good data");
                addToTrackDatas(juce::URL{ twoData[0] }, twoData[1]);
            }
            catch (const std::exception& e) {
                DBG("bad data");
            }
        }
    } 
    tableComponent.updateContent();
}

void PlaylistComponent::updatePlaylist() {

    std::ofstream ofs;
    ofs.open(playlistUrlString);

    if (ofs.is_open()) {
        for (int i = 0; i < trackDatas.size(); ++i) {
            urljuce = trackDatas[i].audioURL.toString(false);
            urlstd = urljuce.toStdString();
            durationstd = trackDatas[i].duration.toStdString();
            ofs << urlstd << "," << durationstd << std::endl;
        }
    }
    else {
        DBG("IS NOT OPEN FILE");
    }
    
    ofs.close();
}



