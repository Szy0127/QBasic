#include "tokenizer.h"
#include<sstream>

Tokenizer::Tokenizer()
{

}
Tokenizer::~Tokenizer(){}

Token Tokenizer::string2tokens(std::string cmd)
{
    tokens.clear();
    std::stringstream s(cmd);
    std::string block;
    std::string buf;
    bool constant = false;
    while(s>>block){
        buf.clear();
        for(size_t i = 0 ; i < block.length();i++){
            // **需要特殊对待
            if(i+1 < block.length() && operators.count(std::string(1,block[i])+std::string(1,block[i+1]))){
                if(!buf.empty()){
                    tokens.push_back(buf);
                    buf.clear();
                }
                tokens.push_back(std::string(1,block[i])+std::string(1,block[i+1]));
                i++;
                continue;
            }
            if(operators.count(std::string(1,block[i]))){
                if(!buf.empty()){
                    tokens.push_back(buf);
                    buf.clear();
                }
                tokens.push_back(std::string(1,block[i]));
                continue;
            }
            //不是操作符只能是操作数 根据当前状态连接
            if(buf.empty()){
                constant = isDigit(block[i]);
                buf.push_back(block[i]);
                continue;
            }
            if(constant){//按道理来说 常数的数字后面只能跟操作符了 应该在上面退出了 报错
                if(isDigit(block[i])){
                    buf.push_back(block[i]);
                }else{
                    //error
                }
            }else{
                buf.push_back(block[i]);
            }
        }
        if(!buf.empty()){
            tokens.push_back(buf);
        }

            //tokens.push_back(token);
    }
    return tokens;
}
std::string Tokenizer::tokens2string(Token token)
{
    std::string res = "";
    for(auto &t:token){
        res += t;
        res += " ";
    }
    res.pop_back();//最后一个空格
    return res;
}
bool Tokenizer::isDigit(char c)const
{
    return c >= '0' && c <= '9';
}
