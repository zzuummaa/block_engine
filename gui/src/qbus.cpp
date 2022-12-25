#include "qbus.h"

#include <utility>

QBus* QBus::concat(const QBus* lhs, const QBus* rhs) {
    const auto& lhsTypeInfo = lhs->getTypeInfo();
    const auto& rhsTypeInfo = rhs->getTypeInfo();

    bool isConcatableByType = !lhsTypeInfo || !rhsTypeInfo || (*lhsTypeInfo == *rhsTypeInfo);
    if (!isConcatableByType) {
        return nullptr;
    }

    if (lhs->output && rhs->output) {
        return nullptr;
    }
    const auto& input = lhs->output ? lhs->output : rhs->output;

    // Expect that output has only one linked bus
    std::vector<QPin*> outputs;
    std::copy(lhs->inputs.begin(), lhs->inputs.end(), std::back_inserter(outputs));
    std::copy(rhs->inputs.begin(), rhs->inputs.end(), std::back_inserter(outputs));
    std::sort(outputs.begin(), outputs.end());
    outputs.erase(std::unique(outputs.begin(), outputs.end()), outputs.end());
    if (outputs.size() != lhs->inputs.size() + rhs->inputs.size()) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    std::vector<QBusLine*> parts;
    std::copy(lhs->parts.begin(), lhs->parts.end(), std::back_inserter(parts));
    std::copy(rhs->parts.begin(), rhs->parts.end(), std::back_inserter(parts));
    std::sort(parts.begin(), parts.end());
    parts.erase(std::unique(parts.begin(), parts.end()), parts.end());
    if (parts.size() != lhs->parts.size() + rhs->parts.size()) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    return new QBus { input, outputs, parts };
}

QBus::QBus(
    QPin* output,
    std::vector<QPin*> inputs,
    std::vector<QBusLine*> parts)
    : output(output)
    , inputs(std::move(inputs))
    , parts(std::move(parts)) { }

QBus::QBus() : output(nullptr) {
}

const BusTypeInfo* QBus::getTypeInfo() const {
    if (isOutputLinked()) {
        return &output->info();
    } else if (!inputs.empty()) {
        return &inputs[0]->info();
    } else {
        return nullptr;
    }
}

bool QBus::isOutputLinked() const {
    return output;
}

bool QBus::isInputLinked(QPin* pin) const {
    return std::count(inputs.begin(), inputs.end(), pin) > 0;
}

bool QBus::isPartLinked(QBusLine* part) const {
    return std::count(parts.begin(), parts.end(), part) > 0;
}

bool QBus::isSuitablePin(QPin* pin) const {
    const auto& typeInfo = getTypeInfo();
    if (typeInfo) {
        return *typeInfo == pin->info();
    } else {
        return true;
    }
}

bool QBus::linkPin(QPin* newPin) {
    if (newPin->isInput()) {
        return linkInput(newPin);
    } else {
        return linkOutput(newPin);
    }
}

bool QBus::linkOutput(QPin* newOutput) {
    if (!isOutputLinked() && isSuitablePin(newOutput)) {
        output = newOutput;
        return true;
    } else {
        return false;
    }
}

bool QBus::linkInput(QPin* newInput) {
    if (!isInputLinked(newInput)) {
        inputs.push_back(newInput);
        return true;
    } else {
        return false;
    }
}

bool QBus::linkPart(QBusLine* part) {
    if (!isPartLinked(part)) {
        parts.push_back(part);
        return true;
    } else {
        return false;
    }
}


