// ThemeList.h
#ifndef THEMELIST_H
#define THEMELIST_H

#include "Theme.h"
#include "OptionList.h"
#include <stdlib.h>
#include <string.h>
#define MAX_THEMES 100

class ThemeList {
    Theme** themeList;

public:
	ThemeList();
    void setup();
    void setup(int);
	void update();
    void update(int);
    void draw();
    void draw(int);
	void transitionTo(int);
    void setTransition(int, long);
    long getTransition(int);
	OptionList loadXML(std::string);

	void add(Theme*);
	void updateTheme(std::string);
    bool moveUp(const char *);
    bool moveBack(const char *);
	void del(std::string);
    void clearList();
    bool isReady(int);
    int numberOfThemes();
    const char* getThemeName(int);
    bool hasTheme(const char *);
    
private:
    int numThemes;
    OptionList globalOptions;
    long transitions[MAX_THEMES-1];
    
};

#endif
