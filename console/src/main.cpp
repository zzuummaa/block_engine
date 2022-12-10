#include <memory>
#include <fstream>
#include <iostream>
#include <utility>

#include "view/json/json_scheme_parser.h"
#include "validators/scheme_validator.h"

#include "core/core.h"

using namespace block_engine::core;

struct CoutPrintEventHandler : public ICalcEngineEventHandler {
    std::shared_ptr<std::atomic_bool> is_end;

    explicit CoutPrintEventHandler(std::shared_ptr<std::atomic_bool> is_end) : is_end(std::move(is_end)) {}

    void notifyError(const CoreError& error) override {
        std::cout << error << std::endl;
    }

    void notifyEvent(const CoreEvent& event) override {
        std::cout << event << std::endl;
        if (event.sub_type == CoreEventSubType::Stop) *is_end = true;
    }
};

int main() {
    std::ifstream fs("test.json");
	if (!fs.good()) {
		throw std::system_error(errno, std::generic_category(), __PRETTY_FUNCTION__);
	}

    auto scheme = block_engine::view::json::JsonSchemeParser(fs).parse();
    if (!SchemeValidator().validate(scheme)) throw std::runtime_error(__PRETTY_FUNCTION__);

    auto is_end = std::make_shared<std::atomic_bool>(false);
    Core core{std::make_shared<CoutPrintEventHandler>(is_end)};
    core.onSetScheme(scheme);
    core.onStartCalc();

    while (!*is_end) std::this_thread::sleep_for(std::chrono::milliseconds(10));

    core.onStopCalc();

    return 0;
}