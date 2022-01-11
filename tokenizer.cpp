#include "tokenizer.h"
#include "myexception.h"
#include<sstream>

Tokenizer::Tokenizer()
{

}
Tokenizer::~Tokenizer(){}

std::string Tokenizer::char2string(char c)
{
    return std::string(1,c);
}

std::string Tokenizer::char2string(char c1,char c2)
{
    return std::string(1,c1) + std::string(1,c2);
}
Token Tokenizer::string2tokens(std::string cmd)
{
    tokens.clear();
    std::stringstream s(cmd);
    std::string block;
    std::string buf;
    bool constant = false;

    while(s>>block){
        //先用空格分开
        buf.clear();
        for(size_t i = 0 ; i < block.length();i++){
            // **需要特殊对待
            if(i+1 < block.length() && operators.count(char2string(block[i],block[i+1]))){
                if(!buf.empty()){
                    tokens.push_back(buf);
                    buf.clear();
                }
                tokens.push_back(char2string(block[i],block[i+1]));
                i++;
                continue;
            }
            if(operators.count(char2string(block[i]))){
                if(!buf.empty()){
                    tokens.push_back(buf);
                    buf.clear();
                }
                tokens.push_back(char2string(block[i]));
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
                    throw IdentifierError(char2string(buf.back(),block[i]));
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
