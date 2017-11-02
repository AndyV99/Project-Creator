#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "../ConfigLoader/ConfigLoader.h"
#include "../util.h"

class Parser
{
public:
    Parser();
    Parser(std::ofstream*, std::ifstream*, ConfigLoader*);
    ~Parser();

	void parseFile();
    std::vector<PCStructs::myCls> getClasses();

private:
	// All the classes in the file
    std::vector<PCStructs::myCls> classes;
	
	// Returns clsFunction based on input
	// INPUT: "    -int getDouble(int number)
	// OUTPUT: 
	// clsFunction
	// {
	//     name = "getDouble",
	//     returnType = int,
	//     returnTypeCode = int,
	//     functionVars = [
	//         clsVar
	//         {
	//             name = "number",
	//             type = int,
	//             typeCode = int,
	//             dependancy = "",
	//          }
	//      ]
	// }
    PCStructs::clsFunction makeFunction(std::string);
	
	// Returns clsVar based on input
	// INPUT: "str myString"
	// OUTPUT:
	// clsVar
	// {
	//     name = "myString",
	//     type = str,
	//     typeCode = std::string,
	//     dependancy = <string>
	// }
	//
    PCStructs::clsVar makeVariable(std::string);

	// Returns clsConstructor based on input
	// INPUT: "    =Class_1(int inValue)"
	// OUTPUT:
	// clsConstructor
	// {
	//    inherited = false,
	//    contructorVars = [
	//        clsVar
	//        {
	//		      name = "inValue",
	//		      type = int,
	//		      typeCode = int,
	//		      dependancy = ""
	//		  }
	//	  ]
	//	  parentVars = []
	// }
    PCStructs::clsConstructor makeConstructor(std::string);

	// Returns variable vector based on input
	// INPUT: "(int inInt, str inString)"
	// OUTPUT:
	// vec<clsVar>
	// [
	//     clsVar
	//     {
	//         name = "inInt",
	//         type = int,
	//         typeCode = int,
	//         dependancy = ""
	//     },
	//     clsVar
	//     {
	//         name = "inString",
	//         type = str,
	//         typeCode = std::string,
	//         dependancy = <string>
	//     }
	// ]
	std::vector<PCStructs::clsVar> makeFunctionVars(std::string);
    ConfigLoader* config;
    std::ofstream* LOG;
    std::ifstream* projectFile;

};

#endif
