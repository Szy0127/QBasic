#include "statement.h"

Statement::Statement()
{

}
Expression *Statement::getExp()
{
    return nullptr;
}
Statement::~Statement(){}
LETsta::LETsta(std::string var,Expression *exp):var(var),exp(exp){}
LETsta::~LETsta()
{
    delete exp;
}
Expression *LETsta::getExp()
{
    return exp;
}
void LETsta::exec(Evalstate *state)
{
    state->setValue(var,exp->eval(state));
    state->setNext();
}

GOTOsta::GOTOsta(int n):lineNumber(n){}
GOTOsta::~GOTOsta(){}
void GOTOsta::exec(Evalstate *state)
{
    state->setGoto(lineNumber);
}

PRINTsta::PRINTsta(Expression *exp):exp(exp){}
PRINTsta::~PRINTsta()
{
    delete exp;
}
Expression *PRINTsta::getExp()
{
    return exp;
}
void PRINTsta::exec(Evalstate *state)
{
    //std::cout<<exp->eval(state)<<std::endl;
    int resInt = exp->eval(state);
    std::stringstream ss;
    ss<<resInt;
    std::string resString;
    ss>>resString;
    state->print(resString);
    state->setNext();

}

IFsta::IFsta(std::string op,Expression *l,Expression *r,int n):op(op),left(l),right(r),lineNumber(n){}

IFsta::~IFsta()
{
    delete left;
    delete right;
}
void IFsta::exec(Evalstate *state)
{
    int l = left->eval(state);
    int r = right->eval(state);
    bool go = false;
    switch (op[0]) {//这里因为没有 >= <= 直接char比 可以用switch
    case '>':
        go = l > r;
        break;
    case '<':
        go = l < r;
        break;
    case '=':
        go = l == r;
        break;

    }
    if(go){
        state->setGoto(lineNumber);
    }else{
        state->setNext();
    }
}

ENDsta::ENDsta(){}
ENDsta::~ENDsta(){}
void ENDsta::exec(Evalstate *state)
{
    state->setEnd();
}
REMsta::REMsta(){}
REMsta::~REMsta(){}
void REMsta::exec(Evalstate *state)
{
    state->setNext();
}
INPUTsta::INPUTsta(std::string name):name(name){}
void INPUTsta::exec(Evalstate *state)
{
    state->print(name+"=?");
    state->startInput(name);
    state->setNext();
}
INPUTsta::~INPUTsta(){}
