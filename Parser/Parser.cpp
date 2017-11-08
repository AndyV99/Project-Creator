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
        std::cout << "\tLINE: " << line << '\n';
        if (line.length() != 0)
        { //If new class
            (*LOG) << "\t\tCLASS DETECTED" << '\n';
			std::cout << "\t\tCLASS DETECTED" << '\n';
			PCStructs::myCls newClass;
			int colon = PCUtil::findNext(0, line, ':');
			if(colon == 0)
			{
				std::cout << "\t\tBASE CLASS" << '\n';
				newClass.name = line;
				std::cout << "\t\tNAME: " << newClass.name << '\n';
			}
			else
			{
				std::cout << "\t\tINHERITED CLASS" << '\n';
				newClass.name = line.substr(0, colon-1);
				newClass.parent = line.substr(colon+2);
				std::cout << "\t\tNAME: " << newClass.name << '\n';
				std::cout << "\t\tPARENT NAME: " << newClass.parent << '\n';
			}

            while(std::getline((*projectFile), line) && line.length() != 0)
            { // for each line under the class
				// std::cout << "\tLINE: " << line << '\n';
				if(line[4] == '=')
				{ // if line starts with '=', it's a constructor
					(*LOG) << "\t\tCONSTRUCTOR DETECTED" << '\n';
					std::cout << "\t\tCONSTRUCTOR DETECTED" << '\n';
					PCStructs::clsConstructor newConstructor = makeConstructor(line);
			}
				else if(line[4] == '~')
				{ // if line starts with '~', it's a destructor
					(*LOG) << "\t\tDESTRUCTOR DETECTED" << '\n';
					std::cout << "\t\tDESTRUCTOR DETECTED" << '\n';
				}
				else if(line.substr(line.length()-1, 1) == ")")
                { // if line ends in ')', it's a function 
                    (*LOG) << "\t\tFUNCTION DETECTED" << '\n';
					std::cout << "\t\tFUNCTION DETECTED" << '\n';
                    PCStructs::clsFunction newFunction = makeFunction(line);
                    if(line[4] == '+')
                    { // if line starts with '+', function is public
                        newClass.publicFunctions.push_back(newFunction);
                    }
                    else if(line[4] == '-')
                    { // if line starts with '-', function is private
                        newClass.privateFunctions.push_back(newFunction);
                    }
                }
				else
				{ // if it isn't a constructor, destructor, or function, it's a variable
					(*LOG) << "\t\tVARIABLE DETECTED" << '\n';
					std::cout << "\t\tVARIABLE DETECTED" << '\n';
					PCStructs::clsVar newVar = makeVariable(line.substr(5));
					if(line[4] == '+')
					{ // if line starts with '+', variable is public
						newClass.publicVars.push_back(newVar);
					}
					else if(line[4] == '-')
					{ // if line starts with '-', variable is private
						newClass.privateVars.push_back(newVar);
					}
				}
            }
			newClass.buildDependancies();
			PCStructs::myCls* cls = new PCStructs::myCls(&newClass);
			classes.push_back(cls);
        }
        else
        { //whitespace
			(*LOG) << "\t[--WHITESPACE--]" << '\n';
        }
    }
}

PCStructs::clsFunction Parser::makeFunction(std::string input)
{
    (*LOG) << "\t\t\tMAKE FUNCTION: " << input << '\n';
    PCStructs::clsFunction returnFunction;

    int nextSpace = PCUtil::findNext(5, input, ' ');
    int open = PCUtil::findNext(nextSpace, input, '(');
	int close = PCUtil::findNext(nextSpace, input, ')');

    returnFunction.name = input.substr(nextSpace, open-nextSpace);
    (*LOG) << "\t\t\t\tFUNCTION NAME: " << returnFunction.name << '\n';

    PCStructs::clsVar tmp = makeVariable(input.substr(5, PCUtil::findNext(5, input, ' ')-5));
    returnFunction.returnType = tmp.type;
    (*LOG) << "\t\t\t\tRETURN TYPE: " << tmp.type << '\n';

    returnFunction.returnTypeCode = tmp.typeCode;
    (*LOG) << "\t\t\t\tRETURN TYPE CODE: " << tmp.typeCode << '\n';

	if(open != close-1)
	{ // if there's stuff between the parenthesis
		returnFunction.functionVars = makeFunctionVars(input.substr(open, close-open));
	}
    return returnFunction;
}

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
			returnVar.dependancy = config->getConfigSettings()[i].dependancy;
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
		if(printLog)
		{
			(*LOG) << "\t\t\t\t\tNAME: " << returnVar.name << '\n';
		}
    }
    return returnVar;
}

PCStructs::clsConstructor Parser::makeConstructor(std::string input)
{
	(*LOG) << "\t\t\tMAKE CONSTRUCTOR: " << input << '\n';
	std::cout << "\t\t\tMAKE CONSTRUCTOR: " << input << '\n';
	PCStructs::clsConstructor returnConstructor;
	
	int open = PCUtil::findNext(0, input, '(');
	int close = PCUtil::findNext(open, input, ')');
	
	if(open != close-1)
	{ // If the parenthesis aren't empty
		returnConstructor.constructorVars = makeFunctionVars(input.substr(open, close-open));
		if(close != input.length()-1)
		{ // If there is a colon indicated inheritance
			open = PCUtil::findNext(close+1, input, '(');
			close = PCUtil::findNext(close+1, input, ')');
			returnConstructor.parentVars = makeFunctionVars(input.substr(open, close-open));
			returnConstructor.inherited = true;
		}
	}
	else
	{ // If the parenthesis are empty
		(*LOG) << "\t\t\t\tNO CONSTRUCTOR VARS" << '\n';
	}
	return returnConstructor;
}

std::vector<PCStructs::clsVar> Parser::makeFunctionVars(std::string input)
{
	std::vector<PCStructs::clsVar> returnVector;
	bool done = false;
	int last = 0;
	int next = PCUtil::findNext(last, input, ',');
	if(next == 0)
	{ // if there's only 1 parameter
		next = input.length();
	}
	while(!done)
	{
		PCStructs::clsVar newVar = makeVariable(input.substr(last+1, next-last-1));
		(*LOG) << "\t\t\t\tFUNCTION PARAMETER: " << input.substr(last+1, next-last-1) << '\n';
		std::cout << "\t\t\t\tFUNCTION PARAMETER: " << input.substr(last+1, next-last-1) << '\n';

		if(next == input.length())
		{ // if we are at the end of the line
			done = true;
		}
		else if(PCUtil::findNext(next+1, input ,',')  != 0)
		{ // if there is another comma 
			// go to next comma
			last = next+1;
			next = PCUtil::findNext(next+1, input, ',');
		}
		else if(PCUtil::findNext(next+1, input, ',') == 0 && next != input.length()-1)
		{ // if there are no more commas
			// handle the closing parenthesis
			last = next+1;
			next = input.length();
		}
		else
		{ // if there aren't any commas and we have handled the last variable
			done = true;
		}
		returnVector.push_back(newVar); 
	}
	return returnVector;
}

std::vector<PCStructs::myCls*> Parser::getClasses()
{
    return this->classes;
}
