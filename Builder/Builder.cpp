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
		this->buildClass(project->classes[i], project->name);
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

std::string Builder::buildClass(PCStructs::myCls myClass, std::string projectName)
{
	std::cout << "BUILD CLASS" << '\n';
	std::string returnString;
	std::string header = projectName + "/" + myClass.name + "/" + myClass.name + ".h";
	std::string cpp = projectName + "/" + myClass.name + "/" + myClass.name + ".cpp";

	std::ofstream headerFile(header);
	std::ofstream cppFile(cpp);

	for(int i = 0; i < myClass.dependancies.size(); i++)
	{
		headerFile << "#include " << myClass.dependancies[i] << '\n';
	}

	headerFile << myClass.name;
	if(myClass.parent != "")
	{
		headerFile << " : " << myClass.parent << '\n';
	}
	else
	{
		headerFile << '\n';
	}
	headerFile << '{' << '\n';
	headerFile << "public:" << '\n';
	for(int i = 0; i < myClass.constructors.size(); i++)
	{
		std::cout << "CONSTRUCTOR" << '\n';
		headerFile << '\t' << myClass.name <<  buildConstructor(myClass.constructors[i]) << '\n';
	}
	/*  
	 * Include Header/Dependancies
	 * Class Name With Inheritance
	 * Public
	 * 	Constructors
	 * 	Deconstructor
	 * 	Public Variables
	 * 	public functions
	 *  
	 * Private
	 *  Private Variables
	 *  Private Functions
	 */
	headerFile.close();
	cppFile.close();
	return returnString;
}

std::string Builder::buildVariable(PCStructs::clsVar variable) 
{

}


std::string Builder::buildFunction(PCStructs::clsFunction function) {}

std::string Builder::buildConstructor(PCStructs::clsConstructor constructor)
{
	std::stringstream outStream;
	outStream << buildFunctionVars(constructor.constructorVars);
	if(constructor.inherited)
	{
		outStream << ":" << buildFunctionVars(constructor.parentVars) << '\n';
	}
	else
	{
		outStream << '\n';
	}
	return outStream.str();
}

std::string Builder::buildFunctionVars(std::vector<PCStructs::clsVar> vars)
{
	std::cout << "buildFunctionVars: " << '\n';
	std::stringstream outStream;
	
	std::cout << "(";
	outStream << "(";	
	for(int i = 0; i < vars.size(); i++)
	{
		if(i == vars.size()-1)
		{
			std::cout << vars[i].typeCode;
			outStream << vars[i].typeCode;
		}
		else
		{
			std::cout << vars[i].typeCode << ",";
			outStream << vars[i].typeCode << ",";
		}
	}
	std::cout << ")";
	outStream << ")";
	return outStream.str();
}
