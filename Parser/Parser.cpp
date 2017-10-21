#include "Parser.h"
#include <iostream>

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
                if(line.substr(line.length()-1, 1) == ")")
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
    (*LOG) << "\t\t\tMAKE FUNCTION: " << line << '\n';
    PCStructs::clsFunction returnFunction;

    int nextSpace = PCUtil::findNext(5, line, ' ');
    int nextOpen = PCUtil::findNext(nextSpace, line, '(');
    returnFunction.name = line.substr(nextSpace, nextOpen-nextSpace);
    (*LOG) << "\t\t\t\tFUNCTION NAME: " << returnFunction.name << '\n';

    PCStructs::clsVar tmp = makeVariable(line.substr(5, PCUtil::findNext(5, line, ' ')-5));
    returnFunction.returnType = tmp.type;
    (*LOG) << "\t\t\t\tRETURN TYPE: " << tmp.type << '\n';

    returnFunction.returnTypeCode = tmp.typeCode;
    (*LOG) << "\t\t\t\tRETURN TYPE CODE: " << tmp.typeCode << '\n';

    int next = nextOpen;
    do
    {
        std::cout << "LINE: " << line << '\n';
        std::cout << "nextOpen: " << nextOpen << '\n';
        std::cout << "line.length(): " << line.length() << '\n';
        std::cout << "next: " << next << '\n';
        std::cout << "newVar string: " << line.substr(1+next, PCUtil::findNext(next+1, line, ',')-next) << '\n';
        PCStructs::clsVar newVar = makeVariable(line.substr(1+next, PCUtil::findNext(next+1, line, ',')-next));
        (*LOG) << "\t\t\tIDK WHAT THIS DOES: " << line.substr(1+next, PCUtil::findNext(next+1, line, ',')-next) << '\n';
        if(next+1 != line.length())
        {
            next = PCUtil::findNext(next+1, line, ',');
        }
        else
        {
            next = 0;
        }
        std::cout << "\t\t\tWHAT: " << PCUtil::findNext(next, line, ',')-next << '\n';
    }while(PCUtil::findNext(next, line, ',') != 0 && next <= line.length());

    return returnFunction;
}

/*
input example: "str myString"
output example
clsVar
{
name = myString
type = str
typeCode = std::string
dependancy = <string>
}
*/
PCStructs::clsVar Parser::makeVariable(std::string input)
{
    bool printLog = false;;
    if(printLog)
    {
        (*LOG) << "\t\t\tMAKE VAR: " << input << '\n';
    }
    PCStructs::clsVar returnVar;
    bool foundType = false;
    int i, j;
    for (i = 0; i < config->getConfigSettings().size()  && !foundType; i++)
    { //for each config setting
        bool stillSame = true;
        for (j = 0; j < config->getConfigSettings()[i].type.length() && stillSame; j++)
        { //for each character in configsetting.type
            if(printLog)
            {
                (*LOG) << "\t\t\t\tComparing input var to : " << config->getConfigSettings()[i].name << '\n';
                (*LOG) << "\t\t\t\t\tinput[j] = " << input[j] << '\n';
                (*LOG) << "\t\t\t\t\ttype[j] = " << config->getConfigSettings()[i].type[j] << '\n';
            }
            if(input[j] != config->getConfigSettings()[i].type[j])
            { //if any 2 characters aren't the same
                stillSame = false;
            }
        }
        if(stillSame)
        { //if all of the caracters are the same
            if(printLog)
            {
                (*LOG) << "\t\t\t\t\tTYPE FOUND: " << config->getConfigSettings()[i].name << '\n';
            }
            returnVar.type = config->getConfigSettings()[i].type;
            returnVar.typeCode = config->getConfigSettings()[i].typeCode;
            foundType = true;
        }
        else
        { //not this type type
            if(printLog)
            {
                (*LOG) << "\t\t\t\t\tTYPE NOT: " << config->getConfigSettings()[i].name << '\n';
            }
        }
    }
    if(!foundType)
    {
        if(printLog)
        {
            (*LOG) << "\t\t\t\tINVALID TYPE" << '\n';
        }
    }
    return returnVar;
}

std::vector<PCStructs::myCls> Parser::getClasses()
{
    return this->classes;
}
