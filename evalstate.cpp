#include "evalstate.h"

Evalstate::Evalstate()
{

}

int Evalstate::getValue(std::string name)
{
    return name2value[name];
}

void Evalstate::setValue(std::string name, int value)
{
    name2value[name] = value;
}
