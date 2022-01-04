#include <connector.h>

void Connector::connect(int index, const Connector::value_type& bus) {
    at(index);
}

Bus& Connector::getBus(Connector::size_type _n) {
    return *at(_n);
}

const Bus& Connector::getBus(Connector::size_type _n) const {
    return *at(_n);
}

Connector::size_type Connector::count() const {
    return vector::size();
}

void Connector::setBus(unsigned long long int n, Bus* bus) {
    at(n) = bus;
}
