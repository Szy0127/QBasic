#include "program.h"
#include "myexception.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<stdexcept>
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
    initTools();
    rip = statements.end();

}
Program::Program()
{
    initTools();
    rip = statements.end();
}
void Program::initTools()
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
        try {
            commands[cmd.first]= tokenizer->string2tokens(cmd.second);
        } catch (IdentifierError &e) {
            error[cmd.first] = e.what();
        }
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
    int hashKey = hash(token.at(0));
    switch (hashKey) {

    //这里对于每个statement 是根据不同的语法规则设计的  例如根据等号分开
    //对于正常的语句没有问题 但是对于错误语句可能会进行错误的判断 但反正都是错的  无所谓
    case LET:{
        try {
            token.at(3);//为了触发exception
        } catch (std::out_of_range &e) {
            throw StatementError("LET");
        }
        if(SZYExp::banned.count(token[1])){
            throw IdentifierError(token[1]);
        }
        Token expr(token.begin()+3,token.end());
        try {
            exp = parser->token2Exp(expr);
            sta = new LETsta(token[1],exp);
        } catch (IdentifierExp &e) {
            throw e;
        } catch(Myexception &e){
            throw ExpressionError(tokenizer->tokens2string(expr));
        }
        break;
    }

    case PRINT:{
        try {
            token.at(1);
        } catch (std::out_of_range &e) {
            throw StatementError("PRINT");
        }
        Token expr(token.begin()+1,token.end());
        try {
            exp = parser->token2Exp(expr);
            sta = new PRINTsta(exp);
        } catch (IdentifierExp &e) {
            throw e;
        } catch (Myexception &e){
            throw ExpressionError(tokenizer->tokens2string(expr));
        }

        break;
    }
    case GOTO:{
        try {
            token.at(1);
        } catch (std::out_of_range &e) {
            throw StatementError("GOTO");
        }
        if(token.size()>2){
            throw StatementError("GOTO");
        }
        int n = stoi(token[1]);
        if(rawCommands.count(n)){
            sta = new GOTOsta(n);
        }else{
            throw LineError(token[1]);
        }
        break;
    }
    case IF:{
        try {
            token.at(5);
        } catch (std::out_of_range &e) {
            throw StatementError("IF THEN");
        }
        int n = stoi(*(token.end()-1));
        if(!rawCommands.count(n)){
            throw LineError(*(token.end()-1));
        }
        // n IF exp1 op exp2 THEN n1
        auto op = token.end();
        for(const auto &validOp:validCompareOperators){
            if(op != token.end()){
                break;
            }
            op = find(token.begin(),token.end(),validOp);
        }
        if(op == token.end()){
            throw StatementError("IF");
        }
        Token token1(token.begin()+1,op);
        Token token2(op+1,token.end()-2);
        try {
            exp = parser->token2Exp(token1);
        } catch (IdentifierExp &e) {
            throw e;
        } catch (Myexception &e){
            throw ExpressionError(tokenizer->tokens2string(token1));
        }
        try {
            exp1 = parser->token2Exp(token2);
            sta = new IFsta(*op,exp,exp1,n);
        } catch (IdentifierExp &e) {
            throw e;
        } catch (Myexception &e){
            throw ExpressionError(tokenizer->tokens2string(token2));
        }

        break;
    }
    case INPUT:{
        try {
            token.at(1);
        } catch (std::out_of_range &e) {
            throw StatementError("INPUT");
        }
        if(token.size()>2){
            throw StatementError("INPUT");
        }
        if(SZYExp::banned.count(token[1])){
            throw IdentifierError(token[1]);
        }
        sta = new INPUTsta(token[1]);

        break;
    }
    case REM:{
        //rem的内容不需要解析为token 一个字符串整体就可以 但是其他的命令都需要 所以不想单独拿出来写一个逻辑
        //这里需要把提取出来的token再恢复回去
        //实际上token2string的操作只有这里用到 只要把函数中的开始定为begin+1（跳过REM） 就可以省略这里的复制操作 但是影响了函数的逻辑 没有采用
        //REM如果后面是空白的话这里认为也是不符合的
        try {
            token.at(1);
        } catch (std::out_of_range &e) {
            throw StatementError("REM");
        }
        sta = new REMsta(tokenizer->tokens2string(Token(token.begin()+1,token.end())));
        break;
    }
    case END:
        if(token.size()>1){
            throw StatementError("END");
        }
        sta = new ENDsta();
        break;
    default:
        throw StatementError(token[0]+" is not a valid statement");
        break;
    }
    return sta;
}

void Program::getStatements()
{
    statements.clear();
    for(auto &cmd:commands){
        int num = cmd.first;
        Token line(cmd.second);
        try {
            Statement *sta = getOneStatements(line);
            statements[num] = sta;
        } catch (Myexception &e) {
            error[num] = e.what();
        }
    }
}

void Program::getTree()
{
    for(auto &sta:statements){
        tree[sta.first] = sta.second->getTree();
    }
    for(auto &err:error){
        tree[err.first] = std::vector<std::string>{err.second};
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
    try {
        Token token = tokenizer->string2tokens(cmd);
        Statement *sta = getOneStatements(token);
        sta->exec(evalstate);
    } catch (Myexception &e) {
        //std::cout<< e.what()<<std::endl;
        evalstate->print(e.what());
    }

}

void Program::exec()
{
    getTokens();
    getStatements();
    getTree();
    evalstate->reset();//run之前会清空所有的变量 包括输出
    rip = statements.begin();
    _exec();
}
void Program::_exec()
{
    Statement *sta = nullptr;
    int next;
    while(rip !=statements.end()){
        sta = rip ->second;
        try {
            sta->exec(evalstate);
        } catch (Myexception &e) {
            evalstate->print(e.what());
        }
        next = evalstate->getNextLineNumber();
        switch (next) {
        case Evalstate::end:
            rip = statements.end();
            break;
        case Evalstate::next:
            rip++;
            break;
        default:
            if(error.count(next)){//if 或goto 的目标行有错误 执行if的下一行
                rip++;
            }else{
                rip = statements.find(next);
            }

        }
        if(evalstate->isSuspended()){
            break;
        }
    }
}
void Program::continueExec(std::string n)
{
    //input后 控制台返回用户输入 结束input  根据是否是整型判断输入合法与否 但是都需要接着执行
    try {
        int value = stoi(n);
        if(value == 0 && n[0] != 0){
            evalstate->finishInput(value,false);
            throw InputError(n);
        }else{
            evalstate->finishInput(value);
        }
    } catch (InputError &e) {
        evalstate->print(e.what());
    }
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
