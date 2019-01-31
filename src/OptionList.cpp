#include "include/OptionList.h"

OptionList::OptionList() {
    numOptions = 0;
    for(int i = 0; i < MAX_OPTIONS; i++) {
        optionList[i] = Option();
    }
}

void OptionList::add(Option o) {
    optionList[numOptions] = o;
    numOptions++;
}

void OptionList::add(std::string name, std::string value) {
    Option o = Option(name, value);
    add(o);
}

void OptionList::add(std::string name, std::string value, std::string valueType) {
    Option o = Option(name, value, valueType);
    add(o);
}

void OptionList::update(int index, std::string value) {
    optionList[index].setValue(value);
}

void OptionList::update(std::string name, std::string value) {
    for(int i = 0; i < MAX_OPTIONS; i++) {
        if(optionList[i].getName() == name) { //make sure this works in c++
            optionList[i].setValue(value);
        }
    }
}

Option OptionList::get(int index) {
    return optionList[index];
}

Option OptionList::get(std::string name) {
    for(int i = 0; i < MAX_OPTIONS; i++) {
        if(optionList[i].getName() == name) { //make sure this works in c++
            return optionList[i];
        }
    }
    return Option();
}
