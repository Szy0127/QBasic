#include "statement.h"

Statement::Statement()
{

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
    std::cout<<exp->eval(state);
}
