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
        if (line.length() != 0)
        { //If new class
            (*LOG) << "\t\tCLASS DETECTED" << '\n';
	    PCStructs::myCls newClass;
	    newClass.name = line;
	    (*LOG) << "\t\tNAME: " << newClass.name << '\n';
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
		else if(line[4] == '=')
		{
		    (*LOG) << "\t\tCONSTRUCTOR DETECTED" << '\n';
		}
		else
		{
		    (*LOG) << "\t\tVARIABLE DETECTED" << '\n';
		}

            }

        }
        else
        { //whitespace
	    (*LOG) << "\t[--WHITESPACE--]" << '\n';
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

    int last = nextOpen-1;
    int next = PCUtil::findNext(last, line, ','); 
    while(next > 0){
        PCStructs::clsVar newVar = makeVariable(line.substr(last+2, next-last-2));
        (*LOG) << "\t\t\tFUNCTION PARAMETER: " << line.substr(last+2, next-last-2) << '\n';
       
	last = next; 
        next = PCUtil::findNext(next+1, line, ',');
        if(next == 0 && last != line.length()-1)
	{
	    next = line.length()-1;
	}

	returnFunction.functionVars.push_back(newVar); 
        
    }

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
    bool printLog = true;
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
    if(input.length() != returnVar.type.length())
    {
	returnVar.name = input.substr(returnVar.type.length()+1);
	(*LOG) << "\t\t\t\t\tNAME: " << returnVar.name << '\n';
    }
    return returnVar;
}

std::vector<PCStructs::myCls> Parser::getClasses()
{
    return this->classes;
}
