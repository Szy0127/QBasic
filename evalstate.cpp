#include "evalstate.h"

Evalstate::Evalstate(std::map<int,int> l,int begin):nextLineNumber(l)
{
    number = begin;
}

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
    if(!nextLineNumber.count(number)){
        //error;
        return ;
    }
    number = n;
}
int Evalstate::getNextLineNumber()
{
    return number;
}
void Evalstate::setNext()
{
    if(nextLineNumber.count(number)){
        number = nextLineNumber[number];
    }else{
        number = -1;
    }
}
void Evalstate::setEnd()
{
    number = -1;
}
