#include "Parser.h"

Parser::Parser(){}

Parser::Parser(std::ofstream* inLOG, std::ifstream* inProjectFile, ConfigLoader* inConfig)
{
    this->LOG = inLOG;
    this->config = inConfig;
    this->projectFile = inProjectFile;
    (*LOG) << "\tPARSER OBJECT CONSTRUCTED" << '\n';
}

Parser::~Parser(){}

void Parser::parseFile()
{
    std::string line;
    while(std::getline(projectFile, line))
    {
        (*LOG) << "\tLINE: " << line << '\n';
        if (line[0] != ' ')
        { //If new class
            PCStructs::myCls newClass;
            while(std::getline(projectFile, line))
            {
                if(line.substr(line.length()-2, 2) == "()")
                { // if it's a function
                    (*LOG) << "\t\tFUNCTION DETECTED" << '\n';
                    PCStructs::clsFunction newFunction = makeFunction(line);
                    if(line[4] == '+')
                    {

                    }
                    else if(line[4] == '-')
                    {
                        
                    }
                }
            }

            //if 6 to current config var length is equal to the config type
            for (int i = 0; i < config->getConfigSettings(); i++)
            { //for each config setting
                bool stillSame == true;
                for (int j = 0; j < config->getConfigSettings()[i].type && stillSame; j++)
                { //for each character in configsetting.type
                    if(line[6+j] != config->getConfigSettings()[i].type[j])
                    { //if any 2 characters aren't the same
                        stillSame = false;
                    }
                }
                if(stillSame)
                { //if all of the caracters are the saem

                }
            }
        }
        else
        { //whitespace

        }
    }

}

PCStructs::clsFunction Parser::makeFunction(std::string)
{

}

std::vector<PCStructs::myCls> Parser::getClasses()
{
    return this->classes;
}
