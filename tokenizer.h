#ifndef TOKENIZER_H
#define TOKENIZER_H
#include<vector>
#include<iostream>
#include<set>
typedef std::vector<std::string> Token ;
class Tokenizer
{
    std::set<std::string> operators={"+","-","*","/","**","=","<",">","(",")"};
private:
    Token tokens;
    bool isDigit(char c)const;
public:
    Tokenizer();
    ~Tokenizer();
    Token string2tokens(std::string s);
    std::string tokens2string(Token token);

};

#endif // TOKENIZER_H
