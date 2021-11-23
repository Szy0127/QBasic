#include "tokenizer.h"
#include<sstream>

Tokenizer::Tokenizer()
{

}

Token Tokenizer::string2tokens(std::string cmd)
{
    tokens.clear();
    std::stringstream s(cmd);
    std::string token;
    while(s>>token){
        tokens.push_back(token);
    }
    return tokens;
}
