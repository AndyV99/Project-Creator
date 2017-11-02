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
	std::string buildClass(PCStructs::myCls);
	std::string buildVariable(PCStructs::clsVar);
	std::string buildFunction(PCStructs::clsFunction);
	std::string buildConstructor(PCStructs::clsConstructor);

	std::ofstream* LOG;

};

#endif /* end of include guard: BUILDER_H */
