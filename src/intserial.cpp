#include "intserial.h"

intSerial::intSerial() : serializable() {
    this->value = 0;
}

intSerial::intSerial(int i) : serializable() {
    this->value = i;
}

intSerial::intSerial(const intSerial& other) {
    this->value = other.getValue();
}

intSerial::~intSerial() {}

intSerial intSerial::operator=(const intSerial& other) {
    intSerial aux(other);
    if(this == &other)
        return *this;
    value = other.getValue();
    return aux;
}

bool intSerial::operator==(const intSerial &other) const {
    return this->value == other.getValue();
}

bool intSerial::operator<(const intSerial &other) const {
    return this->value < other.getValue();
}

bool intSerial::operator<=(const intSerial &other) const {
    return this->value <= other.getValue();
}

bool intSerial::operator>(const intSerial &other) const {
    return this->value > other.getValue();
}

bool intSerial::operator>=(const intSerial &other) const {
    return this->value >= other.getValue();
}

bool intSerial::operator!=(const intSerial &other) const {
    return this->value != other.getValue();
}

intSerial intSerial::operator+(const intSerial& other) const {
    intSerial aux;
    aux.setValue(this->value + other.getValue());
    return aux;
}

intSerial intSerial::operator-(const intSerial& other) const {
    intSerial aux;
    aux.setValue(this->value - other.getValue());
    return aux;
}

intSerial intSerial::operator*(const intSerial& other) const {
    intSerial aux;
    aux.setValue(this->value * other.getValue());
    return aux;
}

intSerial intSerial::operator/(const intSerial& other) const {
    intSerial aux;
    aux.setValue(this->value / other.getValue());
    return aux;
}

void intSerial::operator++() {
    this->value++;
}

void intSerial::operator--() {
    this->value--;
}

void intSerial::setValue(int v) {
    this->value = v;
}

int intSerial::getValue() const {
    return this->value;
}

string intSerial::toString() {
    return string(reinterpret_cast<char*>(&value), sizeof(value));
}

void intSerial::fromString(string repr) {
    repr.copy(reinterpret_cast<char*>(&value), sizeof(value));
}

string intSerial::toXML() {

}

void intSerial::fromXML(string repr) {

}

string intSerial::toCSV() {

}

void intSerial::fromCSV(string repr) {

}

string intSerial::toJSON() {

}

void intSerial::fromJSON(string repr) {

}

unsigned long long int intSerial::size() const {
    return sizeof(value);
}
