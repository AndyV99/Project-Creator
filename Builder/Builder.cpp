#include "Builder.h"


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
	
}

std::string Builder::buildClass(PCStructs::myCls myClass)
{
	/* Class Name With Inheritance
	 * Public
	 * 	Constructors
	 * 	Deconstructor
	 * 	Public Variables
	 * 	public functions
	 * 	add all dependancies
	 *
	 */
}

std::string Builder::buildVariable(PCStructs::clsVar variable) {}

std::string Builder::buildFunction(PCStructs::clsFunction function) {}

std::string Builder::buildConstructor(PCStructs::clsConstructor constructor) {}
