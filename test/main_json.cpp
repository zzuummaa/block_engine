#include <memory>
#include <fstream>
#include <iostream>

#include <json_scheme_parser.h>
#include <scheme_validator.h>

#include <core/core.h>

int main() {
    std::fstream fs("test.json");
    auto scheme = JsonSchemeParser(fs).parse();
    if (!SchemeValidator().validate(scheme)) throw std::runtime_error(__PRETTY_FUNCTION__);

    block_engine::core::CalcEngine calc_engine(scheme);

    calc_engine.start();
    calc_engine.process_step();
    calc_engine.stop();

    return 0;
}