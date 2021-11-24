#include "program.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
Program::Program(std::string fileName)
{
    std::ifstream f(fileName);
    std::string s;
    while(getline(f,s)){
        rawCommands.push_back(s);
    }
    validCompareOperators.insert("<");
    validCompareOperators.insert("=");
    validCompareOperators.insert(">");
    tokenizer = new Tokenizer;
    parser = new Parser;
    evalstate = nullptr;
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
    std::map<int,int> nextLineNumber; // 此行下一行执行的行号
    std::vector<int> lineNumber; // 行号集合 用于排序

    Expression *exp = nullptr;
    Expression *exp1 = nullptr;
    Statement *sta = nullptr;

    int beginNum = INT_MAX;
    for(auto &line:commands){
        int num = stoi(line[0]);
        lineNumber.push_back(num);//不考虑相等
        if(num <= beginNum){
            beginNum = num;
        }
        int hashKey = hash(line[1]);
        switch (hashKey) {
        case LET:
            exp = parser->token2Exp(std::vector<std::string>(line.begin()+4,line.end()));
            sta = new LETsta(line[2],exp);
            break;

        case PRINT:
            exp = parser->token2Exp(std::vector<std::string>(line.begin()+2,line.end()));
            sta = new PRINTsta(exp);
            break;
        case GOTO:
            sta = new GOTOsta(stoi(line[2]));
            break;
        case IF:{
            // n IF exp1 op exp2 THEN n1
            auto op = line.end();
            for(const auto &validOp:validCompareOperators){
                if(op != line.end()){
                    break;
                }
                op = find(line.begin(),line.end(),validOp);
            }

            exp = parser->token2Exp(std::vector<std::string>(line.begin()+2,op));
            exp1 = parser->token2Exp(std::vector<std::string>(op+1,line.end()-2));
            sta = new IFsta(*op,exp,exp1,stoi(*(line.end()-1)));
            break;
        }
        case REM:
            sta = new REMsta();
            break;
        case END:
            sta = new ENDsta();
            break;
        default:
            break;
            //error
        }

        lineNumber2statement[num] = sta;
    }
    sort(lineNumber.begin(),lineNumber.end());
    if(lineNumber.size()==0)return; // vector.size是unsigned long long 这里只需要n-1次 避免=0时-1=0xffffffffffffffff>0
    for(unsigned long long i = 0 ; i < lineNumber.size()-1 ; i++){
        nextLineNumber[lineNumber[i]] = lineNumber[i+1];
    }
    //nextLineNumber[lineNumber[lineNumber.size()-1]] = -1;

    evalstate = new Evalstate(nextLineNumber,beginNum);
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
    while(evalstate->getNextLineNumber()!=-1){
        //std::cout<<evalstate->getNextLineNumber()<<std::endl;
        Statement *sta = lineNumber2statement[evalstate->getNextLineNumber()];
        sta->exec(evalstate);

    }
}

int Program::hash(std::string s)
{
    int sum = 0;
    for(char c:s){
        sum<<=8;
        sum += c;
    }
    return sum % e;
}
