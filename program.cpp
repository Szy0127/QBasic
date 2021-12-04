#include "program.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
using namespace SZYQBasic;
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
        getline(ss,s);
        appendCMD(n,s);
    }
    init();
    rip = statements.end();

}
Program::Program()
{

}
void Program::init()
{
    tokenizer = new Tokenizer;
    parser = new Parser;
    evalstate = new Evalstate;
}
Program::~Program()
{
    delete tokenizer;
    delete parser;
    delete evalstate;
}

void Program::getTokens()
{
    commands.clear();
    for(auto &cmd:rawCommands){
        commands[cmd.first]= tokenizer->string2tokens(cmd.second);
    }
}

Statement *Program::getOneStatements(Token token)
{
    Expression *exp = nullptr;
    Expression *exp1 = nullptr;
    Statement *sta = nullptr;

    //本来没考虑到命令窗口可以增加statment 所以整体是写在另一个函数内的循环
    //为了避免多层嵌套的if-else 用switch
    //但是这里单独一个函数的话 其实if return就可以了
    int hashKey = hash(token[0]);
    switch (hashKey) {
    case LET:
        exp = parser->token2Exp(std::vector<std::string>(token.begin()+3,token.end()));
        sta = new LETsta(token[1],exp);
        break;

    case PRINT:
        exp = parser->token2Exp(std::vector<std::string>(token.begin()+1,token.end()));
        sta = new PRINTsta(exp);
        break;
    case GOTO:
        sta = new GOTOsta(stoi(token[1]));
        break;
    case IF:{
        // n IF exp1 op exp2 THEN n1
        auto op = token.end();
        for(const auto &validOp:validCompareOperators){
            if(op != token.end()){
                break;
            }
            op = find(token.begin(),token.end(),validOp);
        }

        exp = parser->token2Exp(Token(token.begin()+1,op));
        exp1 = parser->token2Exp(Token(op+1,token.end()-2));
        sta = new IFsta(*op,exp,exp1,stoi(*(token.end()-1)));
        break;
    }
    case INPUT:
        sta = new INPUTsta(token[1]);
        break;
    case REM:
        //rem的内容不需要解析为token 一个字符串整体就可以 但是其他的命令都需要 所以不想单独拿出来写一个逻辑
        //这里需要把提取出来的token再恢复回去
        //实际上token2string的操作只有这里用到 只要把函数中的开始定为begin+1（跳过REM） 就可以省略这里的复制操作 但是影响了函数的逻辑 没有采用
        sta = new REMsta(tokenizer->tokens2string(Token(token.begin()+1,token.end())));
        break;
    case END:
        sta = new ENDsta();
        break;
    default:
        break;
        //error
    }
    return sta;
}

void Program::getStatements()
{
    statements.clear();
    for(auto &cmd:commands){
        int num = cmd.first;
        Token line(cmd.second);
        Statement *sta = getOneStatements(line);
        statements[num] = sta;
    }
}
void Program::removeCmd(int n)
{
    for(auto iter = rawCommands.begin();iter!=rawCommands.end();iter++){
        if(iter->first == n){
            rawCommands.erase(iter);
            break;
        }
    }
}

int Program::stoi(std::string s)
{
    std::stringstream ss(s);
    int res;
    ss>>res;
    return res;
}
void Program::execOne(std::string cmd)
{
    Token token = tokenizer->string2tokens(cmd);
    Statement *sta = getOneStatements(token);
    sta->exec(evalstate);
}

void Program::exec()
{
    getTokens();
    getStatements();
    evalstate->reset();
    rip = statements.begin();
    _exec();
}
void Program::_exec()
{
    Statement *sta = nullptr;
    int next;
    while(rip !=statements.end()){
        sta = rip ->second;
        sta->exec(evalstate);
        next = evalstate->getNextLineNumber();
        switch (next) {
        case -2:
            rip = statements.end();
            break;
        case -1:
            rip++;
            break;
        default:
            rip = statements.find(next);
        }
        if(evalstate->isSuspended()){
            break;
        }
    }
}
void Program::continueExec(int value)
{
    evalstate->finishInput(value);
    if(rip != statements.end()){
        _exec();
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

void Program::appendCMD(int lineNumber, std::string cmd)
{
    if(rawCommands.count(lineNumber) && cmd.empty()){
        removeCmd(lineNumber);
        return;
    }
    if(!cmd.empty()){
        rawCommands[lineNumber] = cmd;
    }
}

std::vector<std::string> Program::getOutput()
{
    return evalstate->getOutput();
}

bool Program::isSuspended()const
{
    return evalstate->isSuspended();
}
