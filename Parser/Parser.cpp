#include "Parser.h"
#include <iostream>

Parser::Parser(){}

Parser::Parser(std::ofstream* inLOG, std::ifstream* inProjectFile, ConfigLoader* inConfig)
{
    this->LOG = inLOG;
    this->config = inConfig;
    this->projectFile = inProjectFile;
}

Parser::~Parser(){}

void Parser::parseFile()
{
    std::string line;
    while(std::getline((*projectFile), line))
    {
        if (line.length() != 0)
        { //If new class
			PCStructs::myCls newClass;

			int colon = PCUtil::findNext(0, line, ':');

			if(colon == 0)
			{ // If there isn't a colon, it's a base class
				newClass.name = line;
			}
			else
			{ // If there is a colon, it's a child class
				newClass.name = line.substr(0, colon-1);
				newClass.parent = line.substr(colon+2);
			}

            while(std::getline((*projectFile), line) && line.length() != 0)
            { // for each line under the class
				// std::cout << "\tLINE: " << line << '\n';
				if(line[4] == '=')
				{ // if line starts with '=', it's a constructor
					PCStructs::clsConstructor newConstructor = makeConstructor(line);
					newClass.constructors.push_back(newConstructor);
				}
				else if(line[4] == '~')
				{ // if line starts with '~', it's a destructor
					// (*LOG) << "\t\tDESTRUCTOR DETECTED" << '\n';
					// std::cout << "\t\tDESTRUCTOR DETECTED" << '\n';
				}
				else if(line.substr(line.length()-1, 1) == ")")
                { // if line ends in ')', it's a function 
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
			// (*LOG) << "\t[--WHITESPACE--]" << '\n';
        }
    }
}

PCStructs::clsFunction Parser::makeFunction(std::string input)
{
    PCStructs::clsFunction returnFunction;

    int nextSpace = PCUtil::findNext(5, input, ' ');
    int open = PCUtil::findNext(nextSpace, input, '(');
	int close = PCUtil::findNext(nextSpace, input, ')');

    returnFunction.name = input.substr(nextSpace+1, open-nextSpace-1);

    PCStructs::clsVar tmp = makeVariable(input.substr(5, PCUtil::findNext(5, input, ' ')-5));
    returnFunction.returnType = tmp.type;

    returnFunction.returnTypeCode = tmp.typeCode;

	if(open != close-1)
	{ // if there's stuff between the parenthesis
		returnFunction.functionVars = makeFunctionVars(input.substr(open, close-open));
	}
    return returnFunction;
}

PCStructs::clsVar Parser::makeVariable(std::string input)
{
    PCStructs::clsVar returnVar;
    bool foundType = false;
    int i, j;
    for (i = 0; i < config->getConfigSettings().size()  && !foundType; i++)
    { //for each config setting
        bool stillSame = true;
        for (j = 0; j < config->getConfigSettings()[i].type.length() && stillSame; j++)
        { //for each character in configsetting.type
            if(input[j] != config->getConfigSettings()[i].type[j] && input[j] != '*')
            { //if any 2 characters aren't the same
                stillSame = false;
            }
			if(input[j] == '<' && config->getConfigSettings()[i].type[j] == '<')
			{ // If templated Class
				returnVar.type = config->getConfigSettings()[i].type.substr(0, PCUtil::findNext(0, config->getConfigSettings()[i].type, '<'));

				returnVar.typeCode = config->getConfigSettings()[i].typeCode.substr(0, PCUtil::findNext(0, config->getConfigSettings()[i].typeCode, '<'));

				PCStructs::clsVar newVar = makeVariable(input.substr(PCUtil::findNext(0, input, '<')+1, PCUtil::findPrev(input.length(), input, '>')-PCUtil::findNext(0, input, '<')-1));

				returnVar.type = returnVar.type + "<" + newVar.type + ">";

				returnVar.typeCode = returnVar.typeCode + "<" + newVar.typeCode + "> ";

				stillSame = false;
				foundType = true;
			}
        }

        if(stillSame)
        { //if all of the caracters are the same
            returnVar.type = config->getConfigSettings()[i].type;
            returnVar.typeCode = config->getConfigSettings()[i].typeCode;
			returnVar.dependancy = config->getConfigSettings()[i].dependancy;
			if(input[j] == '*')
			{
				returnVar.isPointer = true;
				returnVar.type = returnVar.type + '*';
				returnVar.typeCode = returnVar.typeCode + '*';
			}
			else
			{
				returnVar.isPointer = false;
			}
            foundType = true;
        }
    }
    if(!foundType)
    {
		(*LOG) << "\t\t\t\tINVALID TYPE" << '\n';
		returnVar.name = input;
    }
    if(PCUtil::findNext(0, input, ' ') != 0)
	{
		returnVar.name = input.substr(returnVar.type.length()+1);
    }
    return returnVar;
}

PCStructs::clsConstructor Parser::makeConstructor(std::string input)
{
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
			std::cout << "makeConstructor parent vars: " << input.substr(open+1, close-open-1) << '\n';
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

