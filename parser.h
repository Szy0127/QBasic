#ifndef PARSER_H
#define PARSER_H
#include<vector>
#include<stack>
#include<set>
#include<iostream>
#include<expression.h>
#include<map>
using namespace SZYExp;
typedef std::vector<std::string> Token;
class Parser
{
public:
    Parser();
    ~Parser();
    Expression* token2Exp(Token token);

private:
    void merge();

    bool isConstant(std::string s)const;
    int getConstant(std::string s);

    std::stack<std::string> operators;//操作符 优先级严格递增的单调栈
    std::stack<Expression*> operands;//操作数 每次取两个

    bool isFirst;//第一个或跟在(后的第一个token 用于判断负号还是减号
    std::stack<bool> isNegative;

};

#endif // PARSER_H
