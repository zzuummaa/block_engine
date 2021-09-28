#include <connector.h>

void Connector::connect(int index, const Bus &bus) {
    at(index) = bus;
}

const std::vector<Bus>& Connector::getBusses() {
    return *this;
}
