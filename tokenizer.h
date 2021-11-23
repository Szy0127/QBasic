#ifndef TOKENIZER_H
#define TOKENIZER_H
#include<vector>
#include<iostream>

typedef std::vector<std::string> Token ;
class Tokenizer
{
private:
    Token tokens;
public:
    Tokenizer();
    Token string2tokens(std::string s);

};

#endif // TOKENIZER_H
