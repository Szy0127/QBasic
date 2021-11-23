#ifndef PROGRAM_H
#define PROGRAM_H
#include<iostream>
#include<vector>
#include<map>
#include<tokenizer.h>
#include<statement.h>

typedef std::vector<std::string> Token ;
class Program
{

public:
    Program(std::string fileName);
    void show();
    void getTokens();

    std::vector<std::string> rawCommands;
    std::vector<Token> commands;
    std::map<int,Statement> lineNumber2statement;

private:
    Tokenizer tokenizer;
};

#endif // PROGRAM_H
