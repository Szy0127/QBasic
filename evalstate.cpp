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
//    if(!nextLineNumber.count(number)){
//        //error;
//        return ;
//    }
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
}
