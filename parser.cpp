#include "parser.h"
#include <sstream>
Parser::Parser()
{
    validOperators.insert("+");
    validOperators.insert("-");
    validOperators.insert("*");
    validOperators.insert("/");
    validOperators.insert("**");
    validOperators.insert("(");
    validOperators.insert(")");

    precedence["("] = 0;
    precedence["+"] = 2;
    precedence["-"] = 2;
    precedence["*"] = 3;
    precedence["/"] = 3;
    precedence["**"] = 4;
}
Parser::~Parser(){}
void Parser::merge()
{
    std::string op = operators.top();
    operators.pop();
    bool negative = false;
    if(op == "-" && (operands.empty() || operators.top()=="(" )){
        negative = true;
    }
    Expression *right = operands.top();
    operands.pop();
    Expression *left = nullptr;//表示负数的-是单目运算符
    if(!negative){
        left = operands.top();
        operands.pop();
    }
    operands.push(new CompoundExp(op,left,right));
    //operators.pop();
}
Expression* Parser::token2Exp(Token tokens)
{
    while(!operands.empty()){
        operands.pop();
    }
    while(!operators.empty()){
        operands.pop();
    }

    for(auto &token:tokens){
        if(validOperators.count(token)){//token是操作符
            if(token=="("){
                operators.push(token);
                continue;
            }
            if(token==")"){//遇到右括号 把左括号前的操作符全弹出
                while(operators.top()!="("){//这里可能出现括号不匹配的情况
                    merge();
                }
                operators.pop();
                continue;
            }
            while(!operators.empty() && precedence[operators.top()] >= precedence[token] ){
                merge();
            }
            operators.push(token);
        }else{//token是操作数
            Expression *exp = nullptr;
            if(isConstant(token)){
                exp = new ConstantExp(getConstant(token));
            }else{
                exp = new IdentifierExp(token);
            }
            operands.push(exp);
        }
    }
    while(!operators.empty()){//这里可能数量不匹配
        merge();
    }
    return operands.top();
}

bool Parser::isConstant(std::string s)
{
    for(auto c:s){
        if(c<'0' || c > '9'){//暂不支持十六进制
            return false;
        }
    }
    return true;
}
int Parser::getConstant(std::string s)
{
    std::stringstream ss(s);
    int res;
    ss>>res;
    return res;
}
