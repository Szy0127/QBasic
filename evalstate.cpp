#include "evalstate.h"

Evalstate::Evalstate()
{
    number = -1;
}
Evalstate::~Evalstate(){}
int Evalstate::getValue(std::string name)
{
    return name2value[name];
}

void Evalstate::setValue(std::string name, int value)
{
    name2value[name] = value;
}

void Evalstate::setGoto(int n)
{

    number = n;
}
int Evalstate::getNextLineNumber()
{
    return number;
}
void Evalstate::setNext()
{
    number = -1;
}
void Evalstate::setEnd()
{
    number = -2;
}

void Evalstate::reset()
{
    number = -1;
    name2value.clear();
    output.clear();
}
void Evalstate::print(std::string s)
{
    output.push_back(s);
}
std::vector<std::string> Evalstate::getOutput()
{
    return output;
}
bool Evalstate::isSuspended()
{
    return !inputVar.empty();
}
void Evalstate::startInput(std::string name)
{
    inputVar = name;
}
void Evalstate::finishInput(int value)
{
    name2value[inputVar] = value;
    inputVar.clear();
    output.push_back(">>" + std::to_string(value));
}
