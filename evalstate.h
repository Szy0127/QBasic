#ifndef EVALSTATE_H
#define EVALSTATE_H

#include<iostream>
#include<map>
class Evalstate
{
public:
    Evalstate();
    int getValue(std::string name);
    void setValue(std::string name,int value);
private:
    std::map<std::string,int> name2value;
};

#endif // EVALSTATE_H
