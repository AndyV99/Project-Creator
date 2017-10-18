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
    while(std::getline((*projectFile), line))
    {
        (*LOG) << "\tLINE: " << line << '\n';
        if (line[0] != ' ')
        { //If new class
            PCStructs::myCls newClass;
            while(std::getline((*projectFile), line))
            {
                if(line.substr(line.length()-2, 2) == "()")
                { // if it's a function
                    (*LOG) << "\t\tFUNCTION DETECTED" << '\n';
                    PCStructs::clsFunction newFunction = makeFunction(line);
                    if(line[4] == '+')
                    {
                        newClass.publicFunctions.push_back(newFunction);
                    }
                    else if(line[4] == '-')
                    {
                        newClass.privateFunctions.push_back(newFunction);
                    }
                }
                else if (true)
                {
                    /* code */
                }
            }

            /*
            //if 6 to current config var length is equal to the config type
            for (int i = 0; i < config->getConfigSettings(); i++)
            { //for each config setting
                bool stillSame == true;
                for (int j = 0; j < config->getConfigSettings()[i].type && stillSame; j++)
                { //for each character in configsetting.type
                    if(line[5+j] != config->getConfigSettings()[i].type[j])
                    { //if any 2 characters aren't the same
                        stillSame = false;
                    }
                }
                if(stillSame)
                { //if all of the caracters are the same

                }
            }
            */
        }
        else
        { //whitespace

        }
    }

}

PCStructs::clsFunction Parser::makeFunction(std::string line)
{
    PCStructs::clsFunction returnFunction;
    bool foundType = false;;
    for (int i = 0; i < config->getConfigSettings().size()  && !foundType; i++)
    { //for each config setting
        bool stillSame = true;
        for (int j = 0; j < config->getConfigSettings()[i].type.length() && stillSame; j++)
        { //for each character in configsetting.type
            (*LOG) << "\t\t\tComparing return var to : " << config->getConfigSettings()[i].name << '\n';
            (*LOG) << "\t\t\t\tline[5+j] = " << line[5+j] << '\n';
            (*LOG) << "\t\t\t\ttype[j] = " << config->getConfigSettings()[i].type[j] << '\n';
            if(line[5+j] != config->getConfigSettings()[i].type[j])
            { //if any 2 characters aren't the same
                stillSame = false;
            }
        }
        if(stillSame)
        { //if all of the caracters are the same
            (*LOG) << "\t\t\t\tRETURN TYPE FOUND: " << config->getConfigSettings()[i].name << '\n';
            returnFunction.returnType = config->getConfigSettings()[i].type;
            returnFunction.returnTypeCode = config->getConfigSettings()[i].typeCode;
            foundType = true;
        }
        else
        { //invalid type
            (*LOG) << "\t\t\t\tRETURN TYPE NOT: " << config->getConfigSettings()[i].name << '\n';
        }
    }
    return returnFunction;
}

std::vector<PCStructs::myCls> Parser::getClasses()
{
    return this->classes;
}
