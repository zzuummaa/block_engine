#ifndef BLOCK_ENGINE_BLOCKSETTINGS_H
#define BLOCK_ENGINE_BLOCKSETTINGS_H

#include <map>
#include <string>

class BlockSettings {
    typedef std::map<std::string, std::string> TSettingsContainer;
public:
    explicit BlockSettings(TSettingsContainer settings) : settings(std::move(settings)) {}

    

private:
    TSettingsContainer settings;
};

#endif //BLOCK_ENGINE_BLOCKSETTINGS_H
