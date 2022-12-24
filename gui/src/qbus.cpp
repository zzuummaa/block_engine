#include "qbus.h"

#include <utility>

QBus* QBus::concat(const QBus* lhs, const QBus* rhs) {
    const auto& lhsTypeInfo = lhs->getTypeInfo();
    const auto& rhsTypeInfo = rhs->getTypeInfo();

    bool isConcatableByType = !lhsTypeInfo || !rhsTypeInfo || (*lhsTypeInfo == *rhsTypeInfo);
    if (!isConcatableByType) {
        return nullptr;
    }

    if (lhs->input && rhs->input) {
        return nullptr;
    }
    const auto& input = lhs->input ? lhs->input : rhs->input;

    // Expect that output has only one linked bus
    std::vector<QPin*> outputs;
    std::copy(lhs->outputs.begin(), lhs->outputs.end(), std::back_inserter(outputs));
    std::copy(rhs->outputs.begin(), rhs->outputs.end(), std::back_inserter(outputs));
    std::sort(outputs.begin(), outputs.end());
    outputs.erase(std::unique(outputs.begin(), outputs.end()), outputs.end());
    if (outputs.size() != lhs->outputs.size() + rhs->outputs.size()) {
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
    QPin* input,
    std::vector<QPin*> outputs,
    std::vector<QBusLine*> parts)
    : input(input)
    , outputs(std::move(outputs))
    , parts(std::move(parts)) { }

QBus::QBus() : input(nullptr) {
}

const BusTypeInfo* QBus::getTypeInfo() const {
    if (isInputLinked()) {
        return &input->info();
    } else if (!outputs.empty()) {
        return &outputs[0]->info();
    } else {
        return nullptr;
    }
}

bool QBus::isInputLinked() const {
    return input;
}

bool QBus::isOutputLinked(QPin* pin) const {
    return std::count(outputs.begin(), outputs.end(), pin) > 0;
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

bool QBus::linkInput(QPin* newInput) {
    if (!isInputLinked() && isSuitablePin(newInput)) {
        input = newInput;
        return true;
    } else {
        return false;
    }
}

bool QBus::linkOutput(QPin* newOutput) {
    if (!isOutputLinked(newOutput)) {
        outputs.push_back(newOutput);
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


