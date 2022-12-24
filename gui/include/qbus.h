#pragma once

#include <vector>
#include "qbusline.h"
#include "qpin.h"

class QBus {
public:
    static std::optional<QBus> concat(const QBus& lhs, const QBus& rhs);

    explicit QBus(QBusLine* firstPart);

    QBus(const QBus& other) = default;

    QBus& operator=(const QBus&) = default;

    const BusTypeInfo* getTypeInfo() const;

    [[nodiscard]] bool isInputLinked() const;

    [[nodiscard]] bool isOutputLinked(QPin* pin) const;

    [[nodiscard]] bool isPartLinked(QBusLine* part) const;

    [[nodiscard]] bool isSuitablePin(QPin* pin) const;

    bool linkInput(QPin* newInput);

    bool linkOutput(QPin* newOutput);

    bool linkPart(QBusLine* part);

private:
    QPin* input;
    std::vector<QPin*> outputs;
    std::vector<QBusLine*> parts;

    QBus(QPin* input, std::vector<QPin*> outputs, std::vector<QBusLine*> parts);
};