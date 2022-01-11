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
typedef std::vector<std::string> Tree;
static std::set<std::string> validCommand{"LET","IF","REM","END","GOTO","PRINT","INPUT"};
static std::set<std::string> validCompareOperators{"=","<",">"};
class Program
{
    //这里是提前计算了7个statement语句标识符的字符串哈希值 方便使用switch
    enum{e=98999};
    enum{LET=48534};
    enum{IF=18758};
    enum{REM=45747};
    enum{END=87065};
    enum{GOTO=77347};
    enum{PRINT=93836};
    enum{INPUT=71843};
public:
    Program(std::string fileName);
    Program();
    ~Program();


    static int stoi(std::string s);
    std::map<int,std::string> rawCommands; // 行号->command 内容不处理 仅供gui打印
    std::map<int,Token> commands;  //每一行string转为若干个token构成的vector 包括行数 指令 标识符 操作符 常数
    std::map<int,Statement*> statements; // 行号对应的代码
    std::map<int,std::string> error; //错误的行号对应的错误信息
    std::map<int, Tree> tree; // 每个行号对应的语法树

    //error是在构造token和statement的时候都会产生的  只包含一个字符串的错误信息
    //tree是综合了statement的tree及error的错误信息

    void getTokens();
    void getStatements();
    void getTree();

    Statement *getOneStatements(Token token);
    /*mainwindow的run调用exec这个函数 但事实上 exec结束并不表示程序的执行已经结束了
     * 在遇到input语句时 program会中断 将控制返回给console  获取输入 实际上这个时候exec函数已经结束了
     * 在console获取到输入后 再调用continueExc函数
     * 因为input语句导致了program与mainwindow控制的切换 所以没有想到以一个总体的函数表示的方法
     */
    void exec();//解析token 构造语句  调用真正执行_exec
    void continueExec(std::string n);
    void execOne(std::string cmd);//token statement run
    void appendCMD(int lineNumber,std::string cmd);
    bool isSuspended()const;

    std::vector<std::string> getOutput();
private:
    int hash(std::string s);
    void removeCmd(int n);
    void initTools();
    void _exec();//一行行执行statement

    std::map<int,Statement*>::iterator rip;//指向要执行的statement 等于statement.end时程序结束 (需要初始化为该值)
    Tokenizer *tokenizer;
    Parser *parser;
    Evalstate *evalstate;
};
}
#endif // PROGRAM_H
