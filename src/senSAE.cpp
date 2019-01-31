#include "senSAE.h"
#include "ofUtils.h"
#include "SoundAnalyzer.h"

//--------------------------------------------------------------
void senSAE::setup(){
    
    // Display setup
    ofSetWindowTitle("Sensory Sound Augmented Electronics");
    // TODO: Define default window size values or check for what window size should be based on display(s).
    // This could also be user-defined.
    windowWidth = ofGetWindowPtr()->getScreenSize().x;
    windowHeight = ofGetWindowPtr()->getScreenSize().y;
    ofSetWindowShape(windowWidth, windowHeight);
    ofSetFrameRate(30);
    fbo.allocate(windowWidth, windowHeight);
    //ofBackground(ofColor::black); ** Depends on theme, don't call here
    // TODO: Other window setup functions? If so, put here.
    sensaeRunColor = ofColor_<float>::fromHsb(0.88f, 0.70f, 0.80f);
    verdanaFont.load("Verdana.ttf", 14);
    
    // Setup variables for SenSAE logo
    logoWireframe.load("NoBars-White-logo.png");
    logoXstart = (ofGetWidth() / 2) - ((logoWireframe.getWidth() - 143) / 2);
    logoYstart = (ofGetHeight() / 2) - ((logoWireframe.getHeight() + 90) / 2);
    ofSeedRandom(ofGetUnixTime());
    float randomWidth = ofRandom(-100.0f, -25.0f);
    redBar = ofRectangle(logoXstart, logoYstart + 150, randomWidth, 30);
    randomWidth = ofRandom(-100.0f, -25.0f);
    orangeBar = ofRectangle(logoXstart, logoYstart + 180, randomWidth, 30);
    randomWidth = ofRandom(-100.0f, -25.0f);
    yellowBar = ofRectangle(logoXstart, logoYstart + 210, randomWidth, 30);
    randomWidth = ofRandom(-100.0f, -25.0f);
    greenBar = ofRectangle(logoXstart, logoYstart + 240, randomWidth, 30);
    randomWidth = ofRandom(-100.0f, -25.0f);
    lightBlueBar = ofRectangle(logoXstart, logoYstart + 270, randomWidth, 30);
    randomWidth = ofRandom(-100.0f, -25.0f);
    royalBlueBar = ofRectangle(logoXstart, logoYstart + 300, randomWidth, 30);
    randomWidth = ofRandom(-100.0f, -25.0f);
    purpleBar = ofRectangle(logoXstart, logoYstart + 330, randomWidth, 30);
    
    // Initialize theme list
    themes = ThemeList();
    // Create map for theme factories
    setupFactories();
    // Setup the theme list variables
    themesInList = new bool[factories.size()];
    for(int i = 0; i < factories.size(); i++) {themesInList[i] = false;}
    runningTheme = -1; // Initially no running theme
    selectedTheme = -1; // Initially no selected theme
    
    // Initialize sound analyzer with this baseApp, # of input channels,
    // sampling rate (in Hz), and size of audio buffer
    sndAnly.setup(this, 1, 44100, 256);
    
    // Setup variables for running senSAE
    desktopGui.setup();
    logoBeingShown = false;
    runningSenSAE = false;
    wasRunning = 0;
    isTransitioning = false;
    timeTransitionStarted = ofGetElapsedTimeMillis();
    timingsSaved = ofGetElapsedTimeMillis() - 30000L;
    showSynth = false;
    showGui = false;
    firstTime = true;
    p_showGui = &showGui;
}

void senSAE::setupFactories() {
    factories["Swirley"] = new ThemeSwirleyFactory();
    factories["SquareDance"] = new ThemeSquareDanceFactory();
    factories["Circles"] = new ThemeCirclesFactory();
    // DO NOT DELETE OR MODIFY THIS LINE: Add factory declarations here
    // The above line is used to tell the "rebuilder" where it needs
    // to put the new theme's factory declaration. Currently don't know
    // a way to avoid having to do this...
}

void senSAE::addThemeToList(const char* themeToAdd) {
    ThemeFactories::const_iterator factoryIter = factories.find(themeToAdd);
    if (factoryIter != factories.end()) {
        ThemeFactory *factory = factoryIter->second;
        Theme *newTheme = factory->create(&sndAnly);
        newTheme->setSize(windowWidth, windowHeight);
        themes.add(newTheme);
    }
    
}

//--------------------------------------------------------------
void senSAE::update(){
    
}

void senSAE::audioIn(float *input, int bufferSize, int nChannels) {
    sndAnly.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void senSAE::draw(){
    
    if (showSynth && runningTheme >= 0) {
        // Init setting up the visualizer frame (important, this must be called before constructing the frame)
        fbo.begin();
        
        // First check to see if senSAE is "running"... If so, then check for transition, but only
        // check as long as senSAE isn't running the last theme in the theme list.
        if (runningSenSAE) {
            if (isTransitioning && ofGetElapsedTimeMillis() - timeTransitionStarted > TRANSITION_DURATION_MS_L * 2L) {
                // End transition...
                isTransitioning = false;
                timeThemeStarted = ofGetElapsedTimeMillis();
                //ofDisableSmoothing();
                
            } else if (isTransitioning) {
                // Continue transition...
                
            } else if (runningTheme < themes.numberOfThemes() - 1 && ofGetElapsedTimeMillis() - timeThemeStarted >= themes.getTransition(runningTheme)) {
                // Begin transition...
                timeTransitionStarted = ofGetElapsedTimeMillis();
                isTransitioning = true;
                runningTheme++;
                //ofEnableSmoothing();
            }
        }
        
        // Draw the most recent visualizer frame of the running theme
        if (runningSenSAE && isTransitioning) {
            if (ofGetElapsedTimeMillis() - timeTransitionStarted <= TRANSITION_DURATION_MS_L) {
                themes.update(runningTheme - 1);
                
                // The old theme continues to run, but gets fainter over time
                themes.draw(runningTheme - 1);
                ofSetColor(0, 0, 0, 255.0f * (((float)ofGetElapsedTimeMillis() - (float)timeTransitionStarted) / TRANSITION_DURATION_MS_F));
                ofDrawRectangle(0,0, windowWidth, windowHeight);
                
            // Once we are halfway done with the transition, start drawing the old theme first so that we can overlay the
            // new theme
            } else {
                // Update running theme
                themes.update(runningTheme);
                
                themes.draw(runningTheme);
                ofSetColor(0, 0, 0, 255.0f * (1.0f - (((float)ofGetElapsedTimeMillis() - TRANSITION_DURATION_MS_F - (float)timeTransitionStarted) / TRANSITION_DURATION_MS_F)));
                ofDrawRectangle(0,0, windowWidth, windowHeight);
            }
            
        } else {
            // Update running theme
            themes.update(runningTheme);
            // Just draw the running theme
            themes.draw(runningTheme);
        }
        
        // End the frame (important, this must be called after drawing)
        fbo.end();
        
        fbo.draw(0, 0, windowWidth, windowHeight);
        
    // Otherwise, display senSAE generative logo
    }
    else {
        if (!logoBeingShown) {
            // Set bar accelerations
            for (int i = 0; i < numBars; i++) {
                barAccel[i] = ofRandom(-3.0f, 3.0f);
                if (barAccel[i] < 0.5f && barAccel[i] > -0.5f) {
                    barAccel[i] = (barAccel[i] / abs(barAccel[i])) * 0.5f;
                }
            }
        }
        
        ofBackground(ofColor::black);
        // Fade in at first
        
        // Draw image
        logoWireframe.draw(logoXstart, logoYstart);
        
        // Show/update bars
        for (int i = 0; i < numBars; i++) {
            switch (i) {
                case 0: // Red
                    redBar.setWidth(redBar.getWidth() + barAccel[i]);
                    if (redBar.getWidth() >= -25.0 || redBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(255, 0, 0, 255);
                    ofDrawRectangle(redBar.getX(), redBar.getY(), redBar.getWidth(), redBar.getHeight());
                    break;
                case 1: // Orange
                    orangeBar.setWidth(orangeBar.getWidth() + barAccel[i]);
                    if (orangeBar.getWidth() >= -25.0 || orangeBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(255, 115, 0, 255);
                    ofDrawRectangle(orangeBar.getX(), orangeBar.getY(), orangeBar.getWidth(), orangeBar.getHeight());
                    break;
                case 2: // Yellow
                    yellowBar.setWidth(yellowBar.getWidth() + barAccel[i]);
                    if (yellowBar.getWidth() >= -25.0 || yellowBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(255, 255, 0, 255);
                    ofDrawRectangle(yellowBar.getX(), yellowBar.getY(), yellowBar.getWidth(), yellowBar.getHeight());
                    break;
                case 3: // Green
                    greenBar.setWidth(greenBar.getWidth() + barAccel[i]);
                    if (greenBar.getWidth() >= -25.0 || greenBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(0, 184, 0, 255);
                    ofDrawRectangle(greenBar.getX(), greenBar.getY(), greenBar.getWidth(), greenBar.getHeight());
                    break;
                case 4: // Light blue
                    lightBlueBar.setWidth(lightBlueBar.getWidth() + barAccel[i]);
                    if (lightBlueBar.getWidth() >= -25.0 || lightBlueBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(0, 166, 255, 255);
                    ofDrawRectangle(lightBlueBar.getX(), lightBlueBar.getY(), lightBlueBar.getWidth(), lightBlueBar.getHeight());
                    break;
                case 5: // Royal blue
                    royalBlueBar.setWidth(royalBlueBar.getWidth() + barAccel[i]);
                    if (royalBlueBar.getWidth() >= -25.0 || royalBlueBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(0, 0, 230, 255);
                    ofDrawRectangle(royalBlueBar.getX(), royalBlueBar.getY(), royalBlueBar.getWidth(), royalBlueBar.getHeight());
                    break;
                case 6: // Purple
                    purpleBar.setWidth(purpleBar.getWidth() + barAccel[i]);
                    if (purpleBar.getWidth() >= -25.0 || purpleBar.getWidth() <= -100.0) {
                        barAccel[i] = -barAccel[i];
                    }
                    ofSetColor(122, 0, 184, 255);
                    ofDrawRectangle(purpleBar.getX(), purpleBar.getY(), purpleBar.getWidth(), purpleBar.getHeight());
                    break;
            }
        }
        
        if (!logoBeingShown) {
            logoBeingShown = true;
            startedShowingLogo = ofGetElapsedTimeMillis();
            ofSetColor(0, 0, 0, 255);
            ofDrawRectangle(0, 0, windowWidth, windowHeight);
        } else if (ofGetElapsedTimeMillis() - startedShowingLogo < TRANSITION_DURATION_MS_L) {
            ofSetColor(0, 0, 0, 255.0f * (1.0f - (((float)ofGetElapsedTimeMillis() - (float)startedShowingLogo) / TRANSITION_DURATION_MS_F)));
            ofDrawRectangle(0, 0, windowWidth, windowHeight);
        }
        ofSetColor(ofColor::white);
        
        if (!showGui)
            verdanaFont.drawString("<Right-click anywhere for settings>", ofGetWidth() / 2 - 170, logoYstart + logoWireframe.getHeight() + 70);
    }
    
    if (showGui) {
        //required to call this at beginning
        desktopGui.begin();
        
        // Set different font (not working yet)
        /*
        verdanaFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("Verdana.ttf", 14);
        unsigned char * out_pixels; int out_width; int out_height;
        ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&out_pixels, &out_width, &out_height);
        ImGui::PushFont(verdanaFont);
         */
        
        // BEGIN THE DESKTOP GUI WINDOW
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            // TESTING WINDOW
            //ImGui::ShowTestWindow();
            
            // Set up colors with ImGuiColFlags
            static int numColors = 17;
            {
                
                //ImGui::GetStyle().AntiAliasedLines = true;
                
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(1.0f, 0.0f, 0.4f, 0.6274f));
                ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(1.0f, 0.0f, 0.4f, 0.1176f));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.0f, 0.4f, 0.7843f));
                ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.9f, 0.2f, 0.35f, 0.4500f));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(1.0f, 0.0f, 0.4f, 0.1176f));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(1.0f, 1.0f, 1.0f, 0.4706f));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.5490f));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(1.0f, 1.0f, 1.0f, 0.6724f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.2f, 0.35f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.95f, 0.1f, 0.4f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.2745f, 0.4f, 0.4569f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.2745f, 0.4f, 0.6724f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.2745f, 0.4f, 0.8235f));
                ImGui::PushStyleColor(ImGuiCol_CloseButton, ImVec4(1.0f, 0.4706f, 0.5490f, 0.5098f));
                ImGui::PushStyleColor(ImGuiCol_CloseButtonHovered, ImVec4(1.0f, 0.4706f, 0.5490f, 0.6667f));
                
            }
            
            // Booleans for controlling various window flags
            static bool no_titlebar = false;
            static bool no_border = true;
            static bool no_resize = false;
            static bool no_move = false;
            static bool no_scrollbar = false;
            static bool no_collapse = false;
            static bool no_menu = false;
            static float bg_alpha = 0.65f;
            
            // Demonstrate the various window flags. Typically you would just use the default.
            ImGuiWindowFlags window_flags = 0;
            if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
            if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
            if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
            if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
            if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
            if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
            if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
            if (!ImGui::Begin("senSAE Preferences", p_showGui, window_flags))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }
            // Add a menu bar
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Quit")) {
                        // Stop the application
                        ofExit();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            
            // Display data stream options (for now, just sound)
            if (ImGui::TreeNode("Data Stream Options")) {
                // Sound devices
                if (ImGui::TreeNode("Sound Options")) {
                    // TODO Group the sound devices into a "Sound" panel
                    vector<ofSoundDevice> soundDevices = sndAnly.getSoundDevices();
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
                    ImGui::BeginChild("soundOptions", ImVec2(ImGui::GetWindowWidth() - 60, soundDevices.size() * 22), true);
                    for (int i = 0; i < soundDevices.size(); i++) {
                        ofSoundDevice d = soundDevices.at(i);
                        const char * deviceName = d.name.c_str();
                        if (d.isDefaultInput) {
                            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "(Default input)");
                            ImGui::SameLine();
                        }
                        ImGui::Text("Device %i: %s", d.deviceID, deviceName);
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar(2);
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::Dummy(ImVec2(20.0f, 5.0f));
            
            // Theme list (aka Theme selector)
            ImGui::Text("Theme List: ");
            {
                ImGui::SameLine(100);
                ImGui::SetNextWindowContentWidth(themes.numberOfThemes() * 150);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
                ImGui::BeginChild("runningThemeList", ImVec2(0, ImGui::GetItemsLineHeightWithSpacing() + 40), true, ImGuiWindowFlags_HorizontalScrollbar);
                for (int i = 0; i < themes.numberOfThemes(); i++) {
                    if (i > 0) ImGui::SameLine();
                    ImGui::PushID(i);
                    // Generate random hue
                    if (randomHues[i] == 0) {
                        randomHues[i] = ofRandom(1);
                    }
                    ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(randomHues[i], 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(randomHues[i], 0.7f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(randomHues[i], 0.8f, 0.8f));
                    float themeNameLength = sizeof(themes.getThemeName(i)) / sizeof(themes.getThemeName(i)[0]);
                    if (ImGui::Button(themes.getThemeName(i), ImVec2(themeNameLength * 10.0f, 30.0f))) {
                        // Let user deselect the currently selected theme
                        if (selectedTheme == i) {
                            selectedTheme = -1;
                        } else {
                            selectedTheme = i;
                        }
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::PopID();
                }
                ImGui::SameLine();
                ImGui::PushID(themes.numberOfThemes());
                ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.3f, 0.8f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.3f, 0.9f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.3f, 1.0f, 0.8f));
                
                // Functionality for updating Theme List
                if (ImGui::Button(" + ", ImVec2(40.0f, 30.0f))) {
                    // Display all available themes for user to choose from
                    // ONLY LET USER CHOOSE THEMES IF SENSAE ISN'T RUNNING
                    if (runningSenSAE) {
                        errorMessage = "Cannot modify theme list while senSAE is running.";
                        ImGui::OpenPopup("Error Modifying Theme List");
                    } else {
                        ImGui::OpenPopup("Select Theme(s)");
                    }
                }
                ImGui::PopStyleColor(3);
                
                if (ImGui::BeginPopupModal("Error Modifying Theme List")) {
                    ImGui::Text("%s", errorMessage.c_str());
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 2 - 50, 0)); ImGui::SameLine();
                    if (ImGui::Button("Ok", ImVec2(60,0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::EndPopup();
                }
                
                if (ImGui::BeginPopupModal("Select Theme(s)", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    bool chosenThemes[factories.size()];
                    for (int i = 0; i < factories.size(); i++) { chosenThemes[i] = false; }
                    
                    // Display a scrollable list of all themes in list of factories
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
                    ImGui::BeginChild("themeList", ImVec2(200, 300), true);
                    ImGui::Text("All Themes:");
                    ThemeFactories::const_iterator factoryIter = factories.begin();
                    for (int i = 0; i < factories.size(); i++) {
                        ImGui::Selectable(factoryIter->first, &themesInList[i]);
                        factoryIter.operator++();
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar(1);
                    
                    if (ImGui::Button("Save list")) {
                        // Update theme list with themesInList
                        // First clear the current theme list
                        // CHECK THAT SYNTH ISN'T RUNNING FIRST
                        if (showSynth) showSynth = false;
                        // Now clear the theme list
                        themes.clearList();
                        
                        factoryIter = factories.begin();
                        for (int i = 0; i < factories.size(); i++) {
                            if (themesInList[i]) {
                                addThemeToList(factoryIter->first);
                            }
                            factoryIter.operator++();
                        }
                        if (themes.numberOfThemes() > 0) {
                            runningTheme = 0;
                        } else {
                            runningTheme = -1;
                            selectedTheme = -1;
                        }
                        // Make sure to setup themes
                        themes.setup();
                        ImGui::CloseCurrentPopup();
                        
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel")) {
                        // Close the window,
                        // But first, refresh themesInList with what's in the actual list
                        // i.e. don't let it stay as what the user might have chosen
                        if (themes.numberOfThemes() > 0) {
                            factoryIter = factories.begin();
                            for (int i = 0; i < factories.size(); i++) {
                                themesInList[i] = themes.hasTheme(factoryIter->first);
                                factoryIter.operator++();
                            }
                            
                        } else {
                            // Set all themesInList bools to false
                            for (int i = 0; i < factories.size(); i++) {
                                themesInList[i] = false;
                            }
                        }
                        
                        ImGui::CloseCurrentPopup();
                    }
                    
                    ImGui::EndPopup();
                }
                
                ImGui::PopID();
                ImGui::EndChild();
                ImGui::PopStyleVar(2);
            }
            
            // Theme List OPTIONS
            if (themes.numberOfThemes() > 1) {
                if (ImGui::TreeNode("Timing Options:")) {
                    
                    // Go through the themes that we can set an end time for,
                    // i.e. themes 0 through numThemes - 1 (no need to set an
                    // end on the last theme)
                    char label[16];
                    for (int i = 0; i < themes.numberOfThemes() - 1; i++) {
                        // Display the theme name
                        ImGui::Text("%s: ", themes.getThemeName(i));
                        ImGui::Indent();
                        ImGui::Text("End after ");
                        ImGui::SameLine();
                        ImGui::PushItemWidth(20);
                        sprintf(label, "hrEnd%i", i);
                        ImGui::InputInt(label, &(hourEnd[i]), 0, 0, false);
                        ImGui::PopItemWidth();
                        ImGui::SameLine(); ImGui::Text("hrs");
                        ImGui::SameLine(); ImGui::Dummy(ImVec2(2.0f, 2.0f));
                        ImGui::SameLine();
                        ImGui::PushItemWidth(20);
                        sprintf(label, "minEnd%i", i);
                        ImGui::InputInt(label, &(minuteEnd[i]), 0, 0, false);
                        ImGui::PopItemWidth();
                        ImGui::SameLine(); ImGui::Text("mins");
                        ImGui::SameLine(); ImGui::Dummy(ImVec2(2.0f, 2.0f));
                        ImGui::SameLine();
                        ImGui::PushItemWidth(20);
                        sprintf(label, "secEnd%i", i);
                        ImGui::InputInt(label, &(secondEnd[i]), 0, 0, false);
                        ImGui::PopItemWidth();
                        ImGui::SameLine(); ImGui::Text("secs");
                        ImGui::Unindent();
                        
                    }
                    
                    // Display a save timings button
                    if (ImGui::Button("Save Timings")) {
                        if (!runningSenSAE) {
                            // Only allow timings to be changed if senSAE isn't running
                            timingsSaved = true; // Assume they will be saved
                            for (int i = 0; i < themes.numberOfThemes() - 1; i++) {
                                // Also, verify that timing inputs are positive but also within a reasonable range
                                // Hours [0, 999]
                                if (hourEnd[i] < 0 || hourEnd[i] > 999) {
                                    hourEnd[i] = 0;
                                    timingsSaved = false;
                                    errorMessage = "Hour timing must be between 0 and 999 (inclusive).";
                                    ImGui::OpenPopup("Error Modifying Timings");
                                    break;
                                }
                                // Minutes [0, 59]
                                if (minuteEnd[i] < 0 || minuteEnd[i] >= 60) {
                                    minuteEnd[i] = 0;
                                    timingsSaved = false;
                                    errorMessage = "Minute timing must be between 0 and 59 (inclusive).";
                                    ImGui::OpenPopup("Error Modifying Timings");
                                    break;
                                }
                                // Seconds [0, 59]
                                if (secondEnd[i] < 0 || secondEnd[i] >= 60) {
                                    secondEnd[i] = 0;
                                    timingsSaved = false;
                                    errorMessage = "Second timing must be between 0 and 59 (inclusive).";
                                    ImGui::OpenPopup("Error Modifying Timings");
                                    break;
                                }
                                
                                // Convert hrs, mins, secs to milliseconds
                                themes.setTransition(i, (hourEnd[i] * 3600000L) + (minuteEnd[i] * 60000L) + round(secondEnd[i] * 1000L));
                            }
                            
                            if (timingsSaved) {
                                whenTimingsSaved = ofGetElapsedTimeMillis();
                            }
                        }
                    }
                    if (ImGui::BeginPopupModal("Error Modifying Timings")) {
                        ImGui::Text("%s", errorMessage.c_str());
                        ImGui::Separator();
                        ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 2 - 50, 0)); ImGui::SameLine();
                        if (ImGui::Button("Ok", ImVec2(60,0))) { ImGui::CloseCurrentPopup(); }
                        ImGui::EndPopup();
                    }
                    
                    if (timingsSaved && ofGetElapsedTimeMillis() - whenTimingsSaved < 2000L) {
                        ImGui::SameLine();
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.4f, 1.0f));
                        ImGui::Text("Timings Saved!");
                        ImGui::PopStyleColor();
                    } else if (timingsSaved) {
                        // End the "Timings Saved!" text
                        timingsSaved = false;
                    }
                    
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(20.0f, 10.0f));
            }
            
            // TODO: Display selected Theme preferences
            ImGui::Indent();
            if (strcmp(themes.getThemeName(selectedTheme), "") != 0) {
                ImGui::Text("Selected Theme: %s",themes.getThemeName(selectedTheme));
                ImGui::Indent();
                ImGui::Text("Theme options here");
                ImGui::Indent();
                ImGui::Text("Change order: "); ImGui::SameLine();
                if (ImGui::Button("Move Forward")) {
                    // Only allow theme to be moved in theme list IF senSAE isn't running
                    if (runningSenSAE) {
                        // Display error
                        errorMessage = "Cannot adjust theme list while senSAE is running.";
                        ImGui::OpenPopup("Error Modifying Settings");
                    } else if (themes.moveUp(themes.getThemeName(selectedTheme))) {
                        float oldHue = randomHues[selectedTheme-1];
                        randomHues[selectedTheme-1] = randomHues[selectedTheme];
                        randomHues[selectedTheme] = oldHue;
                        selectedTheme--;
                        if (selectedTheme == 0) {
                            // Recall setup for the new running theme
                            themes.setup(runningTheme);
                        }
                    }
                } ImGui::SameLine();
                if (ImGui::Button("Move Back")) {
                    if (runningSenSAE) {
                        // Displpay error
                        errorMessage = "Cannot adjust theme list while senSAE is running.";
                        ImGui::OpenPopup("Error Modifying Settings");
                    } else if (themes.moveBack(themes.getThemeName(selectedTheme))) {
                        float oldHue = randomHues[selectedTheme+1];
                        randomHues[selectedTheme+1] = randomHues[selectedTheme];
                        randomHues[selectedTheme] = oldHue;
                        selectedTheme++;
                        if (selectedTheme == 1) {
                            // Recall setup for the new running theme
                            themes.setup(runningTheme);
                        }
                    }
                }
                ImGui::Unindent();
                ImGui::Unindent();
                
                if (ImGui::BeginPopupModal("Error Modifying Settings")) {
                    ImGui::Text("%s", errorMessage.c_str());
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 2 - 50, 0)); ImGui::SameLine();
                    if (ImGui::Button("Ok", ImVec2(60,0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::EndPopup();
                }
            }
            
            ImGui::Dummy(ImVec2(20.0f, 10.0f));
            
//            // TODO: Save button
//            if (ImGui::Button("Save")) {
//                // Save XML File here
//            }
//            
//            // Play Synth / Preview button (probably need to differentiate between playing and preview,
//            // since playing implies running the entire theme list, while preview would just be for
//            // selected theme)
//            ImGui::SameLine();
//            ImGui::Dummy(ImVec2(5.0f, 0.0f));
//            ImGui::SameLine();
            
            // Button to PREVIEW A SENSAE THEME
            if (!showSynth) {
                if (ImGui::Button("Preview")) {
                    if (themes.numberOfThemes() == 0) {
                        // Display message that user must select a Theme in order to preview
                        ImGui::OpenPopup("Preview Error");
                    } else {
                        runningTheme = 0;
                        showSynth = true;
                        logoBeingShown = false;
                    }
                }
                if (ImGui::BeginPopupModal("Preview Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    ImGui::Text("SenSAE cannot display visualizer preview.\nNo theme(s) loaded into theme list.\n\n");
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 2 - 50, 0)); ImGui::SameLine();
                    if (ImGui::Button("Ok", ImVec2(60,0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::EndPopup();
                }
            } else {
                if (ImGui::Button("Stop")) {
                    showSynth = false;
                    runningSenSAE = false;
                    wasRunning = 0;
                }
            }
            ImGui::Dummy(ImVec2(20.0f, 3.0f));
            
            // Button to PLAY/RUN SENSAE (i.e. the theme list, including transitions)
            // First set the color of the checkbox. If running, have the button cycle through colors,
            // and when senSAE isn't running, it will be gray.
            if (!runningSenSAE) {
                ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4((float)sensaeRunColor.r, (float)sensaeRunColor.g, (float)sensaeRunColor.b, sensaeRunColor.getBrightness()));
                if (sensaeRunColor.getHue() > 1.0f) {
                    sensaeRunColor.setHue(0.0f);
                }
                sensaeRunColor.setHue(sensaeRunColor.getHue() + 0.01f);
            }
            
            // Show the checkbox and act accordingly
            if (ImGui::Checkbox("senSAE", &runningSenSAE)) {
                // Start senSAE, only if there are themes in the theme list
                if (themes.numberOfThemes() == 0) {
                    // Display message that user must select a Theme in order to preview
                    ImGui::OpenPopup("senSAE Error");
                    runningSenSAE = false;
                } else {
                    // Make sure timings have something so that transitions occur (DEFAULT TO 30 SECONDS FOR DEMO)
                    for (int i = 0; i < themes.numberOfThemes() - 1; i++) {
                        if (themes.getTransition(i) == 0) {
                            themes.setTransition(i, DEFAULT_TIMING);
                        }
                    }
                    timeStarted = ofGetElapsedTimeMillis();
                    timeThemeStarted = timeStarted;
                    runningSenSAE = true;
                    wasRunning++;
                    runningTheme = 0;
                    showSynth = true;
                    logoBeingShown = false;
                }
                
            }
            ImGui::PopStyleColor();
            if (ImGui::BeginPopupModal("senSAE Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("SenSAE cannot run without a theme selected.\nPlease load at least one theme.\n\n");
                ImGui::Separator();
                ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 2 - 50, 0)); ImGui::SameLine();
                if (ImGui::Button("Ok", ImVec2(60,0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            
            if (wasRunning > 0 && ofGetElapsedTimeMillis() - timeStarted < 2000L) {
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.4f, 1.0f));
                if (wasRunning > 1) {
                    ImGui::Text("Restarted!");
                } else {
                    ImGui::Text("Started!");
                }
                ImGui::PopStyleColor();
            }
            
            // End GUI stuff
            ImGui::End();
            
            // Pop Color Styles (Have to do this)
            for (int i = 0; i < numColors; i++) {
                ImGui::PopStyleColor();
            }
        }
        
        // required to call this at the end
        desktopGui.end();
    }
}

//--------------------------------------------------------------
void senSAE::keyPressed(int key){
    
}

//--------------------------------------------------------------
void senSAE::keyReleased(int key){
    
}

//--------------------------------------------------------------
void senSAE::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void senSAE::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void senSAE::mousePressed(int x, int y, int button){
    // When user right clicks in the window, display the preferences
    if (button == 2) {
        if (showGui) showGui = false;
        else showGui = true;
    }
    
}

//--------------------------------------------------------------
void senSAE::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void senSAE::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void senSAE::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void senSAE::dragEvent(ofDragInfo dragInfo){
    
}
