#ifndef TOKENIZER_H
#define TOKENIZER_H
#include<vector>
#include<iostream>
#include<set>
typedef std::vector<std::string> Token ;
class Tokenizer
{
private:
    Token tokens;
    std::set<std::string> operators;
    bool isDigit(char c);
public:
    Tokenizer();
    Token string2tokens(std::string s);

};

#endif // TOKENIZER_H
