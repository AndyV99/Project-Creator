#include "Builder.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <memory>
#include <iomanip>
#include <sys/stat.h>

Builder::Builder(){}

Builder::Builder(std::ofstream* LOG, PCStructs::project* project)
{
	this->buildProject(project);
	this->LOG = LOG;
}

Builder::~Builder(){}

void Builder::buildProject(PCStructs::project* project)
{
	this->buildFileStructure(project);
	for(int i = 0; i < project->classes.size(); i++)
	{
		std::string header = project->name + '/' + project->classes[i]->name + '/' + project->classes[i]->name + ".h";
		std::string cpp = project->name + '/' + project->classes[i]->name + '/' + project->classes[i]->name + ".cpp";
		std::ofstream headerFile(header);
		std::ofstream cppFile(cpp);

		std::vector<std::string> strings = buildClass(project->classes[i]);
		headerFile << strings[0];
		cppFile << strings[1];

		headerFile.close();
		cppFile.close();
	}
}

void Builder::buildFileStructure(PCStructs::project* project)
{
    mkdir(project->name.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int i = 0; i < project->classes.size(); i++)
	{
		std::cout << "FILE STRUCTURE" << '\n';
		std::string name = project->name + "/" +  project->classes[i]->name;
		mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
}

std::vector<std::string> Builder::buildClass(PCStructs::myCls myClass)
{
	std::cout << "BUILD CLASS" << '\n';
	std::stringstream headerStream;
	std::stringstream cppStream;
	std::vector<std::string> returnStrings;

	for(int i = 0; i < myClass.dependancies.size(); i++)
	{
		headerStream << "#include " << myClass.dependancies[i] << '\n';
	}

	headerStream << "class " <<  myClass.name;
	if(myClass.parent != "")
	{
		headerStream << " : public " << myClass.parent << '\n';
		cppStream << "#include \"../" << myClass.parent << "/" << myClass.parent << '\n' << '\n';
	}
	else
	{
		headerStream << '\n';
	}
	headerStream << '{' << '\n';

	// PUBLIC
	headerStream << "public:" << '\n';

	// CONSTRUCTORS
	for(int i = 0; i < myClass.constructors.size(); i++)
	{
		headerStream << '\t' << myClass.name <<  buildFunctionVars(myClass.constructors[i].constructorVars, false) << ';' << '\n';
		cppStream << myClass.name << "::" << myClass.name << buildFunctionVars(myClass.constructors[i].constructorVars, true);
		if(myClass.constructors[i].inherited)
		{
			std::cout << myClass.constructors[i].parentVars[0].name << '\n';
			cppStream << " : " << myClass.parent << buildFunctionVars(myClass.constructors[i].parentVars, true);
		}
		cppStream << " {}" << '\n' << '\n';
	}

	// DESTRUCTOR
	headerStream << '\t' << "~" << myClass.name << "();" << '\n' << '\n';
	cppStream << myClass.name << "::~"  << myClass.name << "() {}" << '\n' << '\n';

	// VARIABLES
	std::cout << "myClass.publicVars.size(): " << myClass.publicVars.size() << '\n';
	for(int i = 0; i < myClass.publicVars.size(); i++)
	{
		headerStream << '\t' << buildVariable(myClass.publicVars[i]) << ';' << '\n';
	}

	// FUNCTIONS
	for(int i = 0; i < myClass.publicFunctions.size(); i++)
	{
		headerStream << '\t' << myClass.publicFunctions[i].returnTypeCode << ' ' << buildFunction(myClass.publicFunctions[i], false) << ';' << '\n';
		cppStream << myClass.publicFunctions[i].returnTypeCode << ' ' <<  myClass.name << "::" << buildFunction(myClass.publicFunctions[i], true) << "{}" << '\n' << '\n';  
	}

	// PRIVATE
	headerStream << "private:" << '\n';		
	
	// VARIABLES
	for(int i = 0; i < myClass.privateVars.size(); i++)
	{
		headerStream << '\t' << buildVariable(myClass.privateVars[i]) << ';' << '\n';
	}

	// FUNCTIONS
	for(int i = 0; i < myClass.privateFunctions.size(); i++)
	{
		headerStream << '\t' << myClass.privateFunctions[i].returnTypeCode << ' ' << buildFunction(myClass.privateFunctions[i], false) << ';' << '\n';
		cppStream << myClass.privateFunctions[i].returnTypeCode << ' ' << myClass.name << "::" << buildFunction(myClass.privateFunctions[i], true) << "{}" << '\n' << '\n';
	}

	headerStream << "};" << '\n';

	returnStrings.push_back(headerStream.str());
	returnStrings.push_back(cppStream.str());
	return returnStrings;
}

std::string Builder::buildVariable(PCStructs::clsVar variable) 
{
	std::stringstream outStream;
	outStream << variable.typeCode << " " << variable.name;
	return outStream.str();
}


std::string Builder::buildFunction(PCStructs::clsFunction function, bool useName) 
{
	std::stringstream outStream;

	outStream << function.name << buildFunctionVars(function.functionVars, useName); 

	return outStream.str();
}


std::string Builder::buildFunctionVars(std::vector<PCStructs::clsVar> vars, bool useName)
{
	std::stringstream outStream;
	outStream << "(";	
	for(int i = 0; i < vars.size(); i++)
	{
		if(i == vars.size()-1)
		{
			outStream << vars[i].typeCode;
			if(useName)
			{
				outStream << ' ' << vars[i].name;
			}
		}
		else
		{
			outStream << vars[i].typeCode;
			if(useName)
			{
				outStream << ' ' << vars[i].name;
			}
			outStream << ", ";
		}
	}
	outStream << ")";
	return outStream.str();
}
