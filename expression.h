#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<iostream>
#include<sstream>
#include<evalstate.h>
#include <set>

namespace  SZYExp{

//优先级和合法操作符是属于exp的一部分 但实际上在parser中用到
static std::map<std::string,int> precedence = {
    {"(" , 0},
    {"+" , 2},
    {"-" , 2},
    {"*" , 3},
    {"/" , 3},
    {"**" , 4},

};
static std::set<std::string> validOperators = {
    "+","-","*","/","**","(",")"
};

static std::set<std::string> banned = {"IF","END","REM","THEN","GOTO","LET","PRINT","INPUT"};
static const std::string pad = "    ";
enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };
//class Expression;
//std::vector<std::string> createLevel(Expression* exp); //根据exp树构造层次遍历的结果 每层用空字符串隔开

class Expression
{
public:
    Expression();
    std::vector<std::string> tree;
    virtual ~Expression()=0;
    virtual int eval(Evalstate *state) = 0;
    virtual std::string toString()const = 0;
    virtual ExpressionType type()const=0;
    virtual std::string getOperator()const;

// /* Getter methods for convenience */

//    virtual int getConstantValue();
//    virtual std::string getIdentifierName();
//    virtual std::string getOperator();
    virtual Expression *getLeft()const;
    virtual Expression *getRight()const;

};

class ConstantExp: public Expression {

public:

   ConstantExp(int value);
   virtual ~ConstantExp();
   virtual int eval(Evalstate *state)override;
   virtual std::string toString()const override;
   virtual ExpressionType type()const override;
//   virtual ExpressionType type();

//   virtual int getConstantValue();

private:

   int value;

};
class IdentifierExp: public Expression {

public:

   IdentifierExp(std::string n);
   virtual ~IdentifierExp();
   virtual int eval(Evalstate *state)override;
   virtual std::string toString()const override;
   virtual ExpressionType type()const override;

//   virtual std::string getIdentifierName();

private:

   std::string name;

};


class CompoundExp: public Expression {

public:

   CompoundExp(std::string op, Expression *left, Expression *right);
   virtual ~CompoundExp();

   virtual int eval(Evalstate *state)override;
   virtual std::string toString()const override;
   virtual ExpressionType type()const override;
   std::string getOperator()const override;

//   virtual std::string getOperator();
   virtual Expression *getLeft()const override;
   virtual Expression *getRight()const override;

private:

   std::string op;
   Expression *left, *right;

};
}
#endif // EXPRESSION_H
