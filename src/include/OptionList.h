// OptionList.h
#ifndef OPTIONLIST_H
#define OPTIONLIST_H

#include "Option.h"
#define MAX_OPTIONS 100
class OptionList {
	Option optionList[MAX_OPTIONS];
	int numOptions;

public:
	OptionList();
    //add by option, (name,value), or (name,value,valueType)
    void add(Option);
    void add(std::string,std::string);
    void add(std::string,std::string,std::string);
    //update value by index or name
    void update(int, std::string);
    void update(std::string, std::string);
    //get by index or name
    Option get(int);
    Option get(std::string);
};

#endif
