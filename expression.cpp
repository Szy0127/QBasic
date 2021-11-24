#include "expression.h"
#include <cmath>
Expression::Expression()
{

}
Expression::~Expression(){}
ConstantExp::ConstantExp(int value):value(value){}
ConstantExp::~ConstantExp(){}
IdentifierExp::IdentifierExp(std::string name):name(name){}
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
std::string ConstantExp::toString()
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
std::string IdentifierExp::toString()
{
    return name;
}

int CompoundExp::eval(Evalstate *state) {
   int l = left->eval(state);
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

//std::string CompoundExp::getOperator()
//{
//    return op;
//}
//Expression* CompoundExp::getLHS()
//{
//    return left;
//}
//Expression* CompoundExp::getRHS()
//{
//    return right;
//}
std::string CompoundExp::toString()
{
    return "("+left->toString()+op+right->toString()+")";
}
