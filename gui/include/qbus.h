#pragma once

#include <vector>
#include "qbusline.h"
#include "qpin.h"

class QBus {
public:
    static QBus* concat(const QBus* lhs, const QBus* rhs);

    QBus();

    QBus(const QBus&) = default;
    QBus(QBus&&) = default;

    [[nodiscard]] const BusTypeInfo* getTypeInfo() const;

    [[nodiscard]] bool isInputLinked() const;

    [[nodiscard]] bool isOutputLinked(QPin* pin) const;

    [[nodiscard]] bool isPartLinked(QBusLine* part) const;

    [[nodiscard]] bool isSuitablePin(QPin* pin) const;

    bool linkPin(QPin* newPin);

    bool linkPart(QBusLine* part);

    template<typename TConsumer>
    void forEachItem(const TConsumer& consumer) {
        if (input) {
            consumer(input);
        }

        for (auto output: outputs) {
            consumer(output);
        }

        for (auto part: parts) {
            consumer(part);
        }
    }

private:
    QPin* input;
    std::vector<QPin*> outputs;
    std::vector<QBusLine*> parts;

    QBus(QPin* input, std::vector<QPin*> outputs, std::vector<QBusLine*> parts);

    bool linkInput(QPin* newInput);
    bool linkOutput(QPin* newOutput);
};