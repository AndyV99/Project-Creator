#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "../ConfigLoader/ConfigLoader.h"
#include "../util.h"

class Parser
{
public:
    Parser();
    Parser(std::ofstream*, std::ifstream*, ConfigLoader*);
    ~Parser();

    void parseFile();
    std::vector<PCStructs::myCls> getClasses();

private:
    std::vector<PCStructs::myCls> classes;
    PCStructs::clsFunction makeFunction(std::string);
    PCStructs::clsVar makeVariable(std::string);
    ConfigLoader* config;
    std::ofstream* LOG;
    std::ifstream* projectFile;

};

#endif
