#include <memory>
#include <fstream>
#include <iostream>
#include <utility>

#include "view/json/json_scheme_parser.h"
#include "validators/scheme_validator.h"

#include "core/core.h"

using namespace block_engine::core;

struct CoutCoreApiServer : public ICoreApiServer {
    std::shared_ptr<std::atomic_bool> is_end;

    explicit CoutCoreApiServer(std::shared_ptr<std::atomic_bool> is_end) : is_end(std::move(is_end)) {}

    void notifyError(const CoreError& error) override {
        std::cout << error << std::endl;
    }

    void notifyEvent(const CoreEvent& event) override {
        std::cout << event << std::endl;
        if (event.event_type_id == 0) *is_end = true;
    }
};

int main() {
    std::fstream fs("test.json");
    auto scheme = block_engine::view::json::JsonSchemeParser(fs).parse();
    if (!SchemeValidator().validate(scheme)) throw std::runtime_error(__PRETTY_FUNCTION__);

    auto is_end = std::make_shared<std::atomic_bool>(false);
    Core core{std::make_shared<CoutCoreApiServer>(is_end)};
    core.onSetScheme(scheme);
    core.onStartCalc();

    while (!*is_end) std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return 0;
}