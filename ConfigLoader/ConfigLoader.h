#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "../util.h"
#include <fstream>
#include <vector>

class ConfigLoader
{
private:
    std::vector<PCStructs::configSetting> configVariables;

public:
    ConfigLoader(std::ofstream*, std::ifstream*);
    ~ConfigLoader();
    void loadConfig(std::ofstream*, std::ifstream*);
    std::vector<PCStructs::configSetting> getConfigSettings();
};

#endif /* end of include guard: CONFIGLOADER_H */
