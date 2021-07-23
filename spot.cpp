#include "spot.hpp"
#include "time_utils.hpp"

#include <iostream>
#include <sstream>


Spot::Spot(std::time_t date, double open, double high, double low, double close, double adj_close) {
    this->date = date;
    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;
    this->adj_close = adj_close;
}

Spot::Spot(std::string date, double open, double high, double low, double close, double adj_close) {
    this->date = dateToEpoch(date.c_str());
    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;
    this->adj_close = adj_close;
}

Spot::Spot(std::time_t date, double price){
  this->date = date;
  this->close = price;
  this->open = price;
  this->high = price;
  this->low = price;
  this->adj_close = price;
}

Spot::Spot(std::string date, double price){
  this->date = dateToEpoch(date.c_str());
  this->close = price;
  this->open = price;
  this->high = price;
  this->low = price;
  this->adj_close = price;
}

Spot::~Spot() {}

std::time_t Spot::getDate() {
    return this->date;
}

std::string Spot::getDateToString() {
    return epochToDate(this->date);
}

double Spot::getOpen() {
    return this->open;
}

double Spot::getHigh() {
    return this->high;
}

double Spot::getLow() {
    return this->low;
}

double Spot::getClose() {
    return this->close;
}

double Spot::getAdjClose() {
    return this->adj_close;
}

std::string Spot::toString() {
    std::ostringstream osOpen;
    osOpen << this->open;
    std::ostringstream osHigh;
    osHigh << this->high;
    std::ostringstream osLow;
    osLow << this->low;
    std::ostringstream osClose;
    osClose << this->close;
    std::ostringstream osAdjClose;
    osAdjClose << this->adj_close;
    return "{ date: " + this->getDateToString()
            + " open: " + osOpen.str()
            + " high: " + osHigh.str()
            + " low: " + osLow.str()
            + " close: " + osClose.str()
            + " adj close: " + osAdjClose.str()
            + " }";
}

void Spot::printSpot() {
    std::cout << this->toString() << std::endl;
}
