// Option.h
#ifndef OPTION_H
#define OPTION_H

#include <string>

class Option {
	std::string name;
    std::string valueType;
    std::string value;

public:
	Option();
    Option(std::string, std::string);
    Option(std::string, std::string, std::string);
    void setName(std::string);
    std::string getName();
    void setValue(std::string);
    std::string getValue();
    void setValueType(std::string);
    std::string getValueType();
};

#endif
