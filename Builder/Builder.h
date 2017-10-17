#ifndef BUILDER_H
#define BUILDER_H

#include <fstream>

class Builder
{
public:
    Builder();
    Builder(std::ofstream*);
    ~Builder();

};

#endif /* end of include guard: BUILDER_H */
