#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <utility>
#include <memory>
#include <stdio.h>
#include <iomanip>

void buildClasses(std::vector<myCls*>, std::string);
std::string buildVariable(std::vector<myCls*>, boost::shared_ptr<clsVar>);
std::string buildStructors(std::vector<myCls*>, int);
std::string buildAccessors(std::vector<myCls*>, boost::shared_ptr<clsVar>);


int main()
{
    std::ofstream LOG("log.txt");


    std::ifstream projectPlan;
    projectPlan.open("projectplan/projectplan.txt");
    std::string projectName;
    std::getline(projectPlan, projectName);

    mkdir(projectName.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    std::string driverPath = projectName + "/driver.cpp";
    std::ofstream driver(driverPath.c_str());

    driver << "int main() {}";

    driver.close();

    std::vector<myCls*> classes;

    std::string line;
    while(std::getline(projectPlan, line))
    {

        LOG << "PARSE LINE: \n" << line << '\n';
        if(isupper(line[0]))
        {
            LOG << "\tCLASS: " << line << '\n';
            classes.push_back(new myCls());
            classes.back()->name = line;
            while (std::getline(projectPlan, line) && line != "")
            {
                if(line[4] == '=')
                {
                    int classNum = line[5] - '0';
                    LOG << "\tPARENT: " << classes[classNum]->name << '\n';
                    classes.back()->parent = classes[classNum];
                    classes.back()->publicVars = classes[classNum]->publicVars;
                    classes.back()->privateVars = classes[classNum]->privateVars;
                }
                else if(line[4] == '-')
                {
                    boost::shared_ptr<clsVar> c(new clsVar);
                    classes.back()->privateVars.push_back(std::move(c));
                    int paramsLength = 0;
                    for (int i = 0; line[4+(i*4)] == '-'; i++) {
                        paramsLength += 4;
                    }
                    classes.back()->privateVars.back()->type = line.substr(5, paramsLength);
                    classes.back()->privateVars.back()->name = line.substr(5+paramsLength);
                    LOG << "\tPRIVATE-VAR: TYPE=" << line.substr(5, paramsLength) << " NAME=" << line.substr(5+paramsLength) << '\n';
                }
                else if(line[4] == '+')
                {
                    if(line.substr(5, 3) == "con")
                    {
                        boost::shared_ptr<clsConstructor> c(new clsConstructor);
                        classes.back()->constructors.push_back(std::move(c));
                        if(line.length() >= 10 && line.substr(10, 3) == "var")
                        {
                            for (int i = 0; line.c_str()[9+(i*7)] == '+' || line.c_str()[9+(i*7)] == '-'; i++)
                            {
                                int varNum = std::stoi(line.substr(13+(i*7), 3), nullptr, 10);
                                if(line.c_str()[9+(i*7)] == '+')
                                {
                                    classes.back()->constructors.back()->constructorVars.push_back(classes.back()->publicVars[varNum]);
                                }
                                else if(line.c_str()[9+(i*7)] == '-')
                                {
                                    classes.back()->constructors.back()->constructorVars.push_back(classes.back()->privateVars[varNum]);
                                }

                            }
                        }
                        LOG << "\tCONSTRUCTOR: NUM-OF-PARAMS= " << classes.back()->constructors.back()->constructorVars.size() << '\n';
                    }
                    else
                    {
                        boost::shared_ptr<clsVar> c(new clsVar);
                        classes.back()->publicVars.push_back(std::move(c));
                        int paramsLength = 3;
                        for (int i = 1; line[4+(i*4)] == '-'; i++)
                        {
                            paramsLength += 4;
                        }
                        classes.back()->publicVars.back()->type = line.substr(5, paramsLength);
                        classes.back()->publicVars.back()->name = line.substr(6+paramsLength);
                        LOG << "\tPUBLIC-VAR: TYPE=" << line.substr(5, paramsLength) << " NAME=" << line.substr(6+paramsLength) << '\n';
                    }
                }
            }
        }
        LOG << '\n';
    }
    LOG.close();
    buildClasses(classes, projectName);
    for (int i = 0; i < classes.size(); i++) {
        classes[i]->publicVars.clear();
        classes[i]->privateVars.clear();
        delete classes[i]->parent;
    }
    classes.clear();
}

void buildClasses(std::vector<myCls*> v, std::string pName)
{
    std::fstream LOG;
    LOG.open("log.txt", std::ifstream::out | std::ifstream::app);
    LOG << "BUID CLASSES" << '\n';
    LOG.close();
    for (int i = 0; i < v.size(); i++) {
        std::fstream LOG;
        LOG.open("log.txt", std::ifstream::out | std::ifstream::app);

        LOG << "\tBUILDLING CLASS: " << v[i]->name << '\n';

        std::string dirname = pName + "/" + v[i]->name;
        mkdir(dirname.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        LOG << "\t\tMADE DIR: " << dirname << '\n';

        std::string fileName = dirname + "/" + v[i]->name + ".h";
        std::ofstream outFile(fileName.c_str());

        LOG << "\t\tOPENED FILE: " << fileName << '\n';

        std::string upperCaseName = boost::to_upper_copy<std::string>(v[i]->name);
        outFile << "#ifndef " << upperCaseName << "_H" << '\n';
        outFile << "#define " << upperCaseName << "_H" << '\n' << '\n';
        outFile << "class " << v[i]->name;
        if(v[i]-> parent != NULL)
        {
            outFile << " : public " << v[i]->parent->name;
        }
        outFile << '\n' << '{' << '\n';
        outFile << "private:" << '\n';

        LOG << "\tBUILDING VARIABLES" << '\n';
        LOG.close();
        for (int j = 0; j < v[i]->privateVars.size(); j++) {
            outFile << '\t' << buildVariable(v, v[i]->privateVars[j]) << '\n';
        }

        outFile << '\n' << "public:" << '\n';
        for (int j = 0; j < v[i]->publicVars.size(); j++) {
            outFile << '\t' << buildVariable(v, v[i]->publicVars[j]) << '\n';
            outFile << '\t' << buildAccessors(v, v[i]->privateVars[j]) << '\n';
        }
        outFile << buildStructors(v, i) << '\n';
        outFile << '\n' << "};";
        outFile.close();

    }
}

std::string buildVariable(std::vector<myCls*> classes, boost::shared_ptr<clsVar> variable)
{
    std::fstream LOG;
    LOG.open("log.txt", std::ifstream::out | std::ifstream::app);

    LOG << "\t\tVARIABLE: \n\t\t\tNAME: " << variable->name << "\n\t\t\tTYPE: " << variable->type;

    std::vector<std::string> validTypes = {"int", "str", "flt", "dbl"};
    std::vector<std::string> typeTranslations = {"int", "std::string", "float", "double"};

    for (int classIndex = 0; classIndex < classes.size(); classIndex++)
    {
        std::stringstream string;
        string << std::setw(3) << std::setfill('0') << classIndex;
        validTypes.push_back(string.str());
        typeTranslations.push_back(classes[classIndex]->name);
    }

    std::stringstream outString;
    if(variable->type.substr(0,3) == "vec")
    {
        for (int i = 0; i < validTypes.size(); i++)
        {
            if(variable->type.substr(4, 3) == validTypes[i])
            {
                if(variable->type[7] == '-')
                {
                    if(variable->type.substr(8, 3) == "ptr")
                    {
                        outString << "std::vector<" << typeTranslations[i] << "*> " << variable->name << ";";
                        variable->typeCode = "std::vector<" + typeTranslations[i] + "*>";
                    }
                    else
                    {
                        std::cout << "INVALID 3RD TYPE" << '\n';
                    }
                }
                else
                {
                    outString << "std::vector<" << typeTranslations[i] << "> " << variable->name << ";";
                    variable->typeCode = "std::vector<" + typeTranslations[i] + ">";
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < validTypes.size(); i++) {
            if(variable->type.substr(0,3) == validTypes[i])
            {
                if(variable->type[3] == '-')
                {
                    outString << typeTranslations[i] << "* " << variable->name << ";";
                    variable->typeCode = typeTranslations[i] + "*";
                }
                else
                {
                    outString << typeTranslations[i] << " " << variable->name << ";";
                    variable->typeCode = typeTranslations[i];
                }
            }
        }
    }
    LOG << "\n\t\t\tTYPECODE: " << variable->typeCode << '\n';
    LOG.close();
    return outString.str();
}

std::string buildStructors(std::vector<myCls*> v, int index)
{
    std::fstream LOG;
    LOG.open("log.txt", std::ifstream::out | std::ifstream::app);
    LOG << "\tBUILDING STRUCTORS" << '\n';
    std::stringstream outString;
    for (int i = 0; i < v[index]->constructors.size(); i++)
    {
        outString << '\t' << v[index]->name << "(";
        LOG << "\t\t" << v[index]->name << "(";
        for (int j = 0; j < v[index]->constructors[i]->constructorVars.size(); j++)
        {
            outString << v[index]->constructors[i]->constructorVars[j]->typeCode;
            LOG << v[index]->constructors[i]->constructorVars[j]->typeCode;
            if(j != v[index]->constructors[i]->constructorVars.size()-1)
            {
                outString << ", ";
                LOG << ", ";
            }
        }
        outString << ");\n";
        LOG << ");\n";
    }
    outString << "\t~" << v[index]->name << "();";
    LOG << "\t\t~" << v[index]->name << "();";
    LOG << "\n\n";
    LOG.close();
    return outString.str();
}
std::string buildAccessors(std::vector<myCls*>, boost::shared_ptr<clsVar>)
{
    return "";
}
