#include "include/ThemeList.h"

ThemeList::ThemeList() {
    globalOptions = OptionList();
    numThemes = 0;
    themeList = (Theme**)malloc(MAX_THEMES); // TEST: Initialize themeList with 1000 empty themes
    for (int i = 0; i < MAX_THEMES-1; i++) {
        transitions[i] = 0;
    }
}

void ThemeList::setup() {
    for(int i = 0; i < numThemes; i++) {
        themeList[i]->setup();
    }
}

void ThemeList::setup(int index) {
    themeList[index]->setup();
}

void ThemeList::update() {
    for(int i = 0; i < numThemes; i++) {
        themeList[i]->update();
    }
}

void ThemeList::update(int index) {
    themeList[index]->update();
}

void ThemeList::draw() {
    for(int i = 0; i < numThemes; i++) {
        themeList[i]->draw();
    }
}

void ThemeList::draw(int index) {
    themeList[index]->draw();
}

/**
 * Check if theme in list given by index is ready to display
 */
bool ThemeList::isReady(int index) {
    return themeList[index]->isReady();
}

void ThemeList::transitionTo(int index) {
    // Transition to the next theme
    
    // This involves doing some sort of fade between the themes...
    
}

long ThemeList::getTransition(int index) {
    return transitions[index];
}

void ThemeList::setTransition(int index, long timeToWait) {
    if (index >= numThemes) {
        return; // This should never happen
    }
    transitions[index] = timeToWait; // In milliseconds
}

OptionList ThemeList::loadXML(std::string path) {
    //load xml from path
    //parse into Options, add to OptionList
    return OptionList();
}

void ThemeList::add(Theme *theme) {
    //somehow find theme class and create new instance
    //master list of themes and includes somewhere?
    
    // Add options to optionList that specifies the start and end times
    std::string startOptionName(theme->getName());
    startOptionName.operator+=("StartTime");
    globalOptions.add(startOptionName, "", "");
    std::string endOptionName(theme->getName());
    endOptionName.operator+=("EndTime");
    globalOptions.add(endOptionName, "", "");
    
    themeList[numThemes++] = theme;
}

bool ThemeList::moveUp(const char * themeName) {
    for (int i = 0; i < numThemes; i++) {
        if (strcmp(themeName, themeList[i]->getName()) == 0) {
            // This is the theme we're looking for, move it forward.
            
            // First check that i != 0
            if (i == 0)
                return false; // If it is 0, stop here. Can't move theme up.
            
            // Save previous theme
            Theme * saveTheme = themeList[i-1];
            // Move theme at i to i-1
            themeList[i-1] = themeList[i];
            themeList[i] = saveTheme;
            return true; // Done
        }
    }
}

bool ThemeList::moveBack(const char * themeName) {
    for (int i = 0; i < numThemes; i++) {
        if (strcmp(themeName, themeList[i]->getName()) == 0) {
            // This is the theme we're looking for, move it back.
            
            // First check that i != numThemes
            if (i == numThemes - 1)
                return false; // If it is the number of themes, stop. Can't move theme back.
            
            // Save next theme
            Theme * saveTheme = themeList[i+1];
            // Move theme at i to i+1
            themeList[i+1] = themeList[i];
            themeList[i] = saveTheme;
            return true; // Done
        }
    }
}

void ThemeList::updateTheme(std::string themeName) {
    //same as above
}

void ThemeList::del(std::string themeName) {
    //same as above
}

void ThemeList::clearList() {
    for (int i = 0; i < numThemes; i++) {
        themeList[i] = nullptr;
    }
    numThemes = 0;
    // Also clear options by reconstructing the list
    globalOptions = OptionList();
}

const char* ThemeList::getThemeName(int index) {
    if (index < 0 || index > numThemes) {
        return "";
    }
    return themeList[index]->getName();
}

int ThemeList::numberOfThemes() {
    return numThemes;
}

bool ThemeList::hasTheme(const char * themeName) {
    for (int i = 0; i < numThemes; i++) {
        if (strcmp(themeName, themeList[i]->getName()) == 0) {
            return true;
        }
    }
    return false;
}
