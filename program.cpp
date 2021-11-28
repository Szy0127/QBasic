#include "program.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
Program::Program(std::string fileName)
{
    std::ifstream f(fileName);
    std::string s;
    std::string tmp;
    //这里存代码的时候需要把最前面的行号作为map的键 后面的所有字符作为一个整体的字符串 方便在gui上输出
    std::stringstream ss;
    int n;
    while(getline(f,s)){
        ss.clear();
        ss<<s;
        ss>>n;
        s.clear();
        while(ss>>tmp){
            s += tmp;
            s += " ";
        }
        s.pop_back();//最后一个空格没必要
        rawCommands[n] = s;
    }
    validCompareOperators.insert("<");
    validCompareOperators.insert("=");
    validCompareOperators.insert(">");
    tokenizer = new Tokenizer;
    parser = new Parser;
    evalstate = new Evalstate;
}

//Program::~Program()
//{
//    std::cout<<123;
//    delete tokenizer;
//    delete parser;
//    delete evalstate;
//}

void Program::getTokens()
{
    for(auto &cmd:rawCommands){
        commands[cmd.first]= tokenizer->string2tokens(cmd.second);
    }
}


void Program::getStatements()
{
    std::map<int,int> nextLineNumber; // 此行下一行执行的行号
    std::vector<int> lineNumber; // 行号集合 用于排序

    Expression *exp = nullptr;
    Expression *exp1 = nullptr;
    Statement *sta = nullptr;

    for(auto &cmd:commands){
        int num = cmd.first;
        Token line(cmd.second);

        int hashKey = hash(line[0]);
        switch (hashKey) {
        case LET:
            exp = parser->token2Exp(std::vector<std::string>(line.begin()+3,line.end()));
            sta = new LETsta(line[1],exp);
            break;

        case PRINT:
            exp = parser->token2Exp(std::vector<std::string>(line.begin()+1,line.end()));
            sta = new PRINTsta(exp,&output);
            break;
        case GOTO:
            sta = new GOTOsta(stoi(line[1]));
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

            exp = parser->token2Exp(std::vector<std::string>(line.begin()+1,op));
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

        statements[num] = sta;
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
    evalstate->reset();
    Statement *sta = nullptr;
    int next;
    auto iter = statements.begin();
    while(iter!=statements.end()){
        sta = iter->second;
        sta->exec(evalstate);
        next = evalstate->getNextLineNumber();
        switch (next) {
        case -2:
            iter = statements.end();
            break;
        case -1:
            iter++;
            break;
        default:
            iter = statements.find(next);
        }
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
