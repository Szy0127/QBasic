#ifndef PROGRAM_H
#define PROGRAM_H
#include<iostream>
#include<vector>
#include<map>
#include<tokenizer.h>
#include<statement.h>
#include<expression.h>
#include<parser.h>
typedef std::vector<std::string> Token ;
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

    std::vector<std::string> rawCommands; // 读入的原始代码 一行是一个string
    std::vector<Token> commands;  //每一行string转为若干个token构成的vector 包括行数 指令 标识符 操作符 常数
    std::map<int,Statement*> lineNumber2statement; // 行号对应的代码


    void show();
    void getTokens();
    void getStatements();
    void exec();
private:
    int stoi(std::string s);
    int hash(std::string s);

    std::set<std::string> validCompareOperators;

    Tokenizer *tokenizer;
    Parser *parser;
    Evalstate *evalstate;
};

#endif // PROGRAM_H
