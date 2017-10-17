#include <fstream>
#include "Parser/Parser.h"
#include "ConfigLoader/ConfigLoader.h"

int main(int argc, char const *argv[]) {
    std::ofstream* LOG = new std::ofstream("log.txt");
    std::ifstream* projectPlanFile = new std::ifstream("projectplan.txt");
    std::ifstream* configFile = new std::ifstream("config.txt");

    std::string projectName;
    std::getline((*projectPlanFile), projectName);


    /*
        1. Load Config
        2. Parse
        3. File Structure
        4. Build
    */

    (*LOG) << "[-----BEGIN LOAD CONFIG-----]" << '\n';
    ConfigLoader* configLoader = new ConfigLoader(LOG, configFile);


    (*LOG) << "[-----BEGIN PARSE-----]" << '\n';
    Parser* parser = new Parser(LOG, projectPlanFile, configFile);


    LOG->close();
    projectPlanFile->close();
    configFile->close();
    delete LOG;
    delete projectPlanFile;
    delete configFile;
    delete configLoader;
    delete parser;
    return 0;
}
