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
	std::cout << "File Structure Built" << '\n';
	for(int i = 0; i < project->classes.size(); i++)
	{
		std::cout << "BUILDING CLASS: " << project->classes[i]->name << '\n';
		this->buildClass(project->classes[i]);
	}
}

void Builder::buildFileStructure(PCStructs::project* project)
{
    mkdir(project->name.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	std::cout << "DRIVER FILE MADE" << '\n';
	std::string driverPath = project->name +  "/driver.cpp";
	std::ofstream driver(driverPath.c_str());
	for(int i = 0; i < project->classes.size(); i++)
	{
		std::cout << "MAKING CLASS STUFF: " << project->classes[i]->name << '\n';
		std::string name = project->name + "/" +  project->classes[i]->name;
		std::cout << "NAME: " << name << '\n';
		mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		std::string classPath = name + "/" + project->classes[i]->name;
		std::cout << "CLASS PATH: " << classPath << '\n';
		std::ofstream header((classPath + ".h").c_str());
		std::ofstream cpp((classPath + ".cpp").c_str());

		for(int j = 0; j < project->classes[i]->dependancies.size(); j++)
		{
			header << "#include " << project->classes[i]->dependancies[j] << '\n';

		}
		cpp << "#include \"" << project->classes[i]->name << ".h" << '\n';
		driver << "#include \"" << name << "/" << name << ".h\"" << '\n'; 
	}

}


std::string Builder::buildClass(PCStructs::myCls myClass)
{
	std::string returnString;
	/* Include Dependancies
	 * Include Header in cpp
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
	return returnString;
}

std::string Builder::buildVariable(PCStructs::clsVar variable) 
{

}


std::string Builder::buildFunction(PCStructs::clsFunction function) {}

std::string Builder::buildConstructor(PCStructs::clsConstructor constructor) {}
