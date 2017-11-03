#include "Builder.h"
#include <cctype>
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
		this->buildClass(project->classes[i]);
	}
}

void Builder::buildFileStructure(PCStructs::project* project)
{
    mkdir(project->name.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	std::string driverPath = project->name +  "/driver.cpp";
	std::ofstream driver(driverPath.c_str());
	for(int i = 0; i < project->classes.size(); i++)
	{
		std::string name = project->classes[i].name;
		driver << "#include \"" << name << "/" << name << ".h\"" << '\n'; 
	}

}


std::string Builder::buildClass(PCStructs::myCls myClass)
{

	/* Class Name With Inheritance
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
}

std::string Builder::buildVariable(PCStructs::clsVar variable) 
{

}


std::string Builder::buildFunction(PCStructs::clsFunction function) {}

std::string Builder::buildConstructor(PCStructs::clsConstructor constructor) {}
