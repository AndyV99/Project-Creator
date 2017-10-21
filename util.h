#ifndef PCSTRUCTS_H
#define PCSTRUCTS_H

#include "ConfigLoader/ConfigLoader.h"
#include <boost/shared_ptr.hpp>
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
    };

    struct myCls
    {
        std::string name;
        myCls* parent;
        std::vector<clsVar> publicVars;
        std::vector<clsVar> privateVars;
        std::vector<clsFunction> publicFunctions;
        std::vector<clsFunction> privateFunctions;
        std::vector<clsConstructor> constructors;
        std::vector<std::string> dependancies;
    };

    struct configSetting
    {
        std::string name; //title of variable
        std::string type; //what will be recognized from projectplan
        std::string typeCode; //what it will look like in code
        std::string dependancy; //what to include
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

    inline std::vector<std::string> generateVariable(std::string input)
    {
        
    }
} /* PCUtil */

#endif /* end of include guard: PCUTIL_H */
