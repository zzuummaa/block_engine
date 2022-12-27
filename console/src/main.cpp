#include <memory>
#include <fstream>
#include <iostream>
#include <utility>

#include "view/json/json_scheme_parser.h"
#include "validators/scheme_validator.h"

#include "core/core.h"

using namespace block_engine::core;

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