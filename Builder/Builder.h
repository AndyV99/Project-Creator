#ifndef BUILDER_H
#define BUILDER_H

#include <fstream>
#include "../util.h"
class Builder
{
public:
    Builder();
    Builder(std::ofstream*, PCStructs::project*);
    ~Builder();
	
private:
	void buildProject(PCStructs::project*);
	void buildFileStructure(PCStructs::project*);
	std::vector<std::string> buildClass(PCStructs::myCls);
	std::string buildVariable(PCStructs::clsVar);
	std::string buildFunction(PCStructs::clsFunction, bool);
	std::string buildConstructor(PCStructs::clsConstructor);
	std::string buildFunctionVars(std::vector<PCStructs::clsVar>, bool);

	std::ofstream* LOG;

};

#endif /* end of include guard: BUILDER_H */
