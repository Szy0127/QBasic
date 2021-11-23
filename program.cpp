#include "program.h"
#include<iostream>
#include<fstream>
Program::Program(std::string fileName)
{
    std::ifstream f(fileName);
    std::string s;
    while(getline(f,s)){
        rawCommands.push_back(s);
    }
}
void Program::getTokens()
{
    for(auto &cmd:rawCommands){
        commands.push_back(tokenizer.string2tokens(cmd));
    }
}
void Program::show()
{
    std::cout<<commands.empty();
    for(auto &cmds:commands){
        for(auto &token:cmds){
            std::cout<<token<<std::endl;
        }
    }
}
