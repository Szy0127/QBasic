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

public:
    Program(std::string fileName);

    std::vector<std::string> rawCommands;
    std::vector<Token> commands;
    std::map<int,Statement*> lineNumber2statement;

    void show();
    void getTokens();
    void getStatements();
    void exec();
private:
    int stoi(std::string s);

    int beginNum;
    Tokenizer *tokenizer;
    Parser *parser;
    Evalstate *evalstate;
};

#endif // PROGRAM_H
