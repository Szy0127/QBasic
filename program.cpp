#include "program.h"
#include<iostream>
#include<fstream>
#include<sstream>
Program::Program(std::string fileName)
{
    beginNum = INT_MAX;
    std::ifstream f(fileName);
    std::string s;
    while(getline(f,s)){
        rawCommands.push_back(s);
    }

    tokenizer = new Tokenizer;
    parser = new Parser;
    evalstate = new Evalstate;
}
void Program::getTokens()
{
    for(auto &cmd:rawCommands){
        commands.push_back(tokenizer->string2tokens(cmd));
    }
}
void Program::show()
{
    for(auto &tokens:commands){
        for(auto &token:tokens){
            std::cout<<token<<std::endl;
        }
    }
}

void Program::getStatements()
{
    Expression *exp = nullptr;
    Statement *sta = nullptr;
    for(auto &line:commands){
        int num = stoi(line[0]);
        if(num <= beginNum){
            beginNum = num;
        }
        if(line[1] == "LET"){
            //num LET var = exp;
            exp = parser->token2Exp(std::vector<std::string>(line.begin()+4,line.end()));
            sta = new LETsta(line[2],exp);
            //continue;
        }
        if(line[1] == "PRINT"){
            exp = parser->token2Exp(std::vector<std::string>(line.begin()+2,line.end()));
            sta = new PRINTsta(exp);
            //continue;
        }

        lineNumber2statement[num] = sta;
    }
}

int Program::stoi(std::string s)
{
    std::stringstream ss(s);
    int res;
    ss>>res;
    return res;
}
void Program::exec()
{
    int num = beginNum;
    Statement *sta = lineNumber2statement[num];
    sta->exec(evalstate);
    num = 20;
    sta = lineNumber2statement[num];
    sta->exec(evalstate);
}
