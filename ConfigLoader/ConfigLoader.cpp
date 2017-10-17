#include "ConfigLoader.h"
#include "../util.h"
#include <iostream>

ConfigLoader::ConfigLoader(std::ofstream* LOG, std::ifstream* configFile)
{
    loadConfig(LOG, configFile);
}

ConfigLoader::~ConfigLoader(){}

std::vector<PCStructs::configSetting> ConfigLoader::getConfigSettings()
{
    return configVariables;
}

void ConfigLoader::loadConfig(std::ofstream* LOG, std::ifstream* configFile)
{
    (*LOG) << "\t[---loadConfig()---]" << '\n';
    std::string line;
    while (std::getline((*configFile), line)) {
        (*LOG) << "\t\tLINE: " << line << '\n';
        PCStructs::configSetting newSetting;

        int next = PCUtil::findNext(0, line, ':');
        newSetting.name = line.substr(0,next);
        (*LOG) << "\t\t\tNAME: " << newSetting.name << '\n';

        next = PCUtil::findNext(next, line, '-');
        int next2 = PCUtil::findNext(next, line, '=');
        newSetting.type = line.substr(next+1, next2-next-1);
        (*LOG) << "\t\t\tTYPE: " << newSetting.type << '\n';

        next = PCUtil::findNext(next2, line, '-');
        newSetting.typeCode = line.substr(next2+1, next-next2-1);
        (*LOG) << "\t\t\tTYPECODE: " << newSetting.typeCode << '\n';

        if(line[next] == '-')
        {
            newSetting.dependancy = line.substr(next+1);
            (*LOG) << "\t\t\tDEPENDANCY: " << newSetting.dependancy << '\n';
        }
        else
        {
            (*LOG) << "\t\t\tNO DEPENDANCY" << '\n';
        }
        configVariables.push_back(newSetting);
    }

}
