#ifndef PCSTRUCTS_H
#define PCSTRUCTS_H

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace PCStructs {
    struct clsVar
    {
        std::string name;
        std::string type;
        std::string typeCode;
    };

    struct clsFunction
    {
        std::string name;
        std::string retrunType;
        std::string returnTypeCode;
        std::vector<boost::shared_ptr<clsVar> > functionVars;
    };

    struct clsConstructor
    {
        bool inherited;
        std::vector<boost::shared_ptr<clsVar> > constructorVars;
    };

    struct myCls
    {
        std::string name;
        myCls* parent;
        std::vector<boost::shared_ptr<clsVar> > publicVars;
        std::vector<boost::shared_ptr<clsVar> > privateVars;
        std::vector<boost::shared_ptr<clsFunction> > functions;
        std::vector<boost::shared_ptr<clsConstructor> > constructors;
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
} /* PCUtil */

#endif /* end of include guard: PCUTIL_H */
