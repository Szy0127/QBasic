#ifndef EVALSTATE_H
#define EVALSTATE_H

#include<iostream>
#include<map>
#include<vector>
class Evalstate
{
public:
    Evalstate();
    ~Evalstate();
    int getValue(std::string name);
    void setValue(std::string name,int value);
    void setGoto(int n);//下一行执行标号为n的语句
    void setNext();//顺序执行下一行
    void setEnd();
    int getNextLineNumber();
    void reset();
    void startInput(std::string name);
    void finishInput(int value);
    void print(std::string s);
    bool isSuspended();
    std::vector<std::string> getOutput();
private:
    std::map<std::string,int> name2value;
    int number;
    std::string inputVar;
    std::vector<std::string> output;

};

#endif // EVALSTATE_H
