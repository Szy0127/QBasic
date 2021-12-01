#ifndef PROGRAM_H
#define PROGRAM_H
#include<iostream>
#include<vector>
#include<map>
#include<tokenizer.h>
#include<statement.h>
#include<expression.h>
#include<parser.h>
namespace SZYQBasic{
typedef std::vector<std::string> Token ;
static std::set<std::string> validCommand{"LET","IF","REM","END","GOTO","PRINT","INPUT"};
static std::set<std::string> validCompareOperators{"=","<",">"};
class Program
{
    enum{e=65537};
    enum{LET=17672};
    enum{IF=18758};
    enum{REM=17659};
    enum{END=19967};
    enum{GOTO=3328};
    enum{PRINT=64524};
    enum{INPUT=1796};
public:
    Program(std::string fileName);
    Program();
    ~Program();
//    ~Program(){
//            delete tokenizer;
//            delete parser;
//            delete evalstate;
//    }

    static int stoi(std::string s);
    std::map<int,std::string> rawCommands; // 除了行号 都不处理 方便gui打印
    std::map<int,Token> commands;  //每一行string转为若干个token构成的vector 包括行数 指令 标识符 操作符 常数
    std::map<int,Statement*> statements; // 行号对应的代码

    void getTokens();
    void getStatements();
    Statement *getOneStatements(Token token);
    /*mainwindow的run调用exec这个函数 但事实上 exec结束并不表示程序的执行已经结束了
     * 在遇到input语句时 program会中断 将控制返回给console  获取输入 实际上这个时候exec函数已经结束了
     * 在console获取到输入后 再调用continueExc函数
     * 因为input语句导致了program与mainwindow控制的切换 所以没有想到以一个总体的函数表示的方法
     *
     *
     */
    void exec();
    void continueExec(int value);
    void execOne(std::string cmd);
    void appendCMD(int lineNumber,std::string cmd);
    bool isSuspended();

    std::vector<std::string> getOutput();
private:
    int hash(std::string s);
    void removeCmd(int n);
    void init();
    void _exec();
    std::map<int,Statement*>::iterator rip;
    Tokenizer *tokenizer;
    Parser *parser;
    Evalstate *evalstate;
};
}
#endif // PROGRAM_H
