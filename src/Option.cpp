#include "include/Option.h"

Option::Option() {
    //c++ requires this, but should never be used except for initialization
    name = "";
    value = "";
    valueType = "";
}

Option::Option(std::string n, std::string v) {
    name = n;
    value = v;
    valueType = "std::string";
}

Option::Option(std::string n, std::string v, std::string vT) {
    name = n;
    value = v;
    valueType = vT;
}

void Option::setName(std::string n) {
    name = n;
}

std::string Option::getName() {
    return name;
}

void Option::setValue(std::string v) {
    value = v;
}

std::string Option::getValue() {
    return value;
}

void Option::setValueType(std::string vT) {
    valueType = vT;
}

std::string Option::getValueType() {
    return valueType;
}
