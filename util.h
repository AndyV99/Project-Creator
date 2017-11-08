#ifndef PCSTRUCTS_H
#define PCSTRUCTS_H

#include <vector>
#include <string>

namespace PCStructs {
    struct clsVar
    {
        std::string name;
        std::string type;
        std::string typeCode;
        std::string dependancy;
    };

    struct clsFunction
    {
        std::string name;
        std::string returnType;
        std::string returnTypeCode;
        std::vector<clsVar> functionVars;
    };

    struct clsConstructor
    {
        bool inherited;
        std::vector<clsVar> constructorVars;
		std::vector<clsVar> parentVars;
    };

    struct myCls
    {
        std::string name;
		std::string parent;
        std::vector<clsVar> publicVars;
        std::vector<clsVar> privateVars;
        std::vector<clsFunction> publicFunctions;
        std::vector<clsFunction> privateFunctions;
        std::vector<clsConstructor> constructors;
        std::vector<std::string> dependancies;
		myCls(){}
		myCls(myCls* c)
		{
			this->name = c->name;
			this->parent = c->parent;
			this->publicVars = c->publicVars;
			this->privateVars = c->privateVars;
			this->publicFunctions = c->publicFunctions;
			this->privateFunctions = c->privateFunctions;
			this->constructors = c->constructors;
			this->dependancies = c->dependancies;
		}
		void buildDependancies()
		{
			std::vector<std::string> allDependancies;
			for(int i = 0; i < publicVars.size(); i++)
			{
				allDependancies.push_back(publicVars[i].dependancy);
			}

			for(int i = 0; i < privateVars.size(); i++)
			{
				allDependancies.push_back(privateVars[i].dependancy);
			}
			
			for(int i = 0; i < publicFunctions.size(); i++)
			{
				for(int j = 0; j < publicFunctions[i].functionVars.size(); j++)
				{
					allDependancies.push_back(publicFunctions[i].functionVars[j].dependancy);
				}
			}

			for(int i = 0; i < privateFunctions.size(); i++)
			{
				for(int j = 0; j < privateFunctions[i].functionVars.size(); j++)
				{
					allDependancies.push_back(privateFunctions[i].functionVars[j].dependancy);
				}
			}
			for(int i = 0; i < allDependancies.size(); i++)
			{ // For all dependancies in the class
				bool alreadyExists = false;
				for(int j = 0; j < this->dependancies.size(); j++)
				{ // For all dependancies we've already added
					if(allDependancies[i] == this->dependancies[j] || allDependancies[i] == "")
					{ // If the dependancy has already been added
						alreadyExists = true;	
					}
				}
				if(!alreadyExists)
				{ // If the dependancy hasn't already been added
					this->dependancies.push_back(allDependancies[i]);
				}
			}
		}
	};

    struct configSetting
    {
        std::string name; //title of variable
        std::string type; //what will be recognized from projectplan
        std::string typeCode; //what it will look like in code
        std::string dependancy; //what to include
    };

	struct project
	{
		std::string name;
		std::vector<myCls*> classes;
	};
} /* PCStructs */
#endif /* end of include guard: PCSTRUCTS_H */

#ifndef PCUTIL_H
#define PCUTIL_H

namespace PCUtil {
    inline int findNext(int start, std::string line, char c)
    {
        for (int i = start; i < line.length(); i++) {
            if(line[i] == c)
            {
                return i;
            }
        }
        return 0;
    }

} /* PCUtil */

#endif /* end of include guard: PCUTIL_H */
