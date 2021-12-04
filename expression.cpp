#include "expression.h"
#include "myexception.h"
#include <cmath>
#include <queue>
using SZYExp::Expression;
using SZYExp::ConstantExp;
using SZYExp::IdentifierExp;
using SZYExp::CompoundExp;
using SZYExp::ExpressionType;

Expression::Expression()
{

}
Expression::~Expression(){}
std::string Expression::getOperator()const
{
    return "";
}
Expression* Expression::getLeft()const
{
    return nullptr;
}
Expression* Expression::getRight()const
{
    return nullptr;
}
ConstantExp::ConstantExp(int value):value(value){}
ConstantExp::~ConstantExp(){}
IdentifierExp::IdentifierExp(std::string n)
{
    if(banned.count(n)){
        throw IdentifierError(n);
    }else{
        name = n;
    }
}
IdentifierExp::~IdentifierExp(){}
CompoundExp::CompoundExp(std::string op,Expression *left,Expression *right):op(op),left(left),right(right){}
CompoundExp::~CompoundExp()
{
    delete left;
    delete right;
}

int ConstantExp::eval(Evalstate *state) {
   return value;
}
//int ConstantExp::getConstantValue()
//{
//    return value;
//}
std::string ConstantExp::toString()const
{
    std::stringstream ss;
    ss<<value;
    std::string s;
    ss>>s;
    return s;
}
int IdentifierExp::eval(Evalstate *state) {
   //if (!context.isDefined(name)) error(name + " is undefined");
   return state->getValue(name);
}
//std::string IdentifierExp::getIdentifierName()
//{
//    return name;
//}
std::string IdentifierExp::toString()const
{
    return name;
}

int CompoundExp::eval(Evalstate *state) {
   int l = left?left->eval(state):0;
   int r = right->eval(state);

   if (op == "+") return l + r;
   if (op == "-") return l - r;
   if (op == "*") return l * r;
   if (op == "/") {
      //if (right == 0) error("Division by 0");
      return l / r;
   }
   if (op == "**"){
       return pow(l,r);
   }
   //error("Illegal operator in expression");
   return 0;
}

std::string CompoundExp::toString()const
{
    return "("+left->toString()+op+right->toString()+")";
}

ExpressionType ConstantExp::type()const
{
    return CONSTANT;
}
ExpressionType IdentifierExp::type()const
{
    return IDENTIFIER;
}
ExpressionType CompoundExp::type()const
{
    return COMPOUND;
}
std::string CompoundExp::getOperator()const

{
    return op;
}
Expression* CompoundExp::getLeft()const
{
    return left;
}
Expression* CompoundExp::getRight()const
{
    return right;
}

std::vector<std::string> SZYExp::createLevel(Expression* exp)
{
    std::vector<std::string> level;
    std::queue<Expression*> q;
    q.push(exp);
    q.push(nullptr);
    Expression *tmp;
    while(!q.empty()){
        tmp = q.front();
        q.pop();
        if(!tmp){
            level.push_back("");
            if(!q.front()){
                break;
            }
            q.push(nullptr);
        }else{
            switch (tmp->type()) {
            case CONSTANT:
                level.push_back(tmp->toString());
                break;
            case IDENTIFIER:
                level.push_back(tmp->toString());
                break;
            case COMPOUND:
                level.push_back(tmp->getOperator());
                if(tmp->getLeft()){
                    q.push(tmp->getLeft());
                }
                if(tmp->getRight()){
                    q.push(tmp->getRight());
                }
                break;
            }
        }
    }
    return level;
}
