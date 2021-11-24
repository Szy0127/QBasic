#ifndef EVALSTATE_H
#define EVALSTATE_H

#include<iostream>
#include<map>
class Evalstate
{
public:
    Evalstate(std::map<int,int> l,int begin);
    int getValue(std::string name);
    void setValue(std::string name,int value);
    void setGoto(int n);//下一行执行标号为n的语句
    void setNext();//顺序执行下一行
    void setEnd();
    int getNextLineNumber();
private:
    std::map<std::string,int> name2value;
    int number;
    std::map<int,int> nextLineNumber;

};

#endif // EVALSTATE_H
