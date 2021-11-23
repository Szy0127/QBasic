#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<iostream>
#include<sstream>
#include<evalstate.h>

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };


class Expression
{
public:
    Expression();
    virtual ~Expression()=0;
    virtual int eval(Evalstate *state) = 0;
    virtual std::string toString() = 0;
//    virtual ExpressionType type() = 0;

// /* Getter methods for convenience */

//    virtual int getConstantValue();
//    virtual std::string getIdentifierName();
//    virtual std::string getOperator();
//    virtual Expression *getLHS();
//    virtual Expression *getRHS();

};

class ConstantExp: public Expression {

public:

   ConstantExp(int value);
   virtual ~ConstantExp();
   virtual int eval(Evalstate *state);
   virtual std::string toString();
//   virtual ExpressionType type();

//   virtual int getConstantValue();

private:

   int value;

};
class IdentifierExp: public Expression {

public:

   IdentifierExp(std::string name);
   virtual ~IdentifierExp();
   virtual int eval(Evalstate *state);
   virtual std::string toString();
//   virtual ExpressionType type();

//   virtual std::string getIdentifierName();

private:

   std::string name;

};


class CompoundExp: public Expression {

public:

   CompoundExp(std::string op, Expression *left, Expression *right);
   virtual ~CompoundExp();

   virtual int eval(Evalstate *state);
   virtual std::string toString();
//   virtual ExpressionType type();

//   virtual std::string getOperator();
//   virtual Expression *getLHS();
//   virtual Expression *getRHS();

private:

   std::string op;
   Expression *left, *right;

};

#endif // EXPRESSION_H
