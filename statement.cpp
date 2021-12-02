#include "statement.h"
#include <queue>

using SZYExp::Expression;
const std::string Statement::pad4 = "    ";
Statement::Statement()
{

}
Tree Statement::getTree()
{
    return tree;
}
Expression *Statement::getExp()
{
    return nullptr;
}
Statement::~Statement(){}

LETsta::LETsta(std::string var,Expression *exp):var(var),exp(exp)
{
    level = createLevel(exp);
    createTree();
}
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

GOTOsta::GOTOsta(int n):lineNumber(n)
{
    createTree();
}
GOTOsta::~GOTOsta(){}
void GOTOsta::exec(Evalstate *state)
{
    state->setGoto(lineNumber);
}

PRINTsta::PRINTsta(Expression *exp):exp(exp)
{
    level = createLevel(exp);
    createTree();
}
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

IFsta::IFsta(std::string op,Expression *l,Expression *r,int n):op(op),left(l),right(r),lineNumber(n)
{
    createTree();
}

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

ENDsta::ENDsta()
{
    createTree();
}
ENDsta::~ENDsta(){}
void ENDsta::exec(Evalstate *state)
{
    state->setEnd();
}
REMsta::REMsta(std::string c):content(c)
{
    createTree();
}
REMsta::~REMsta(){}
void REMsta::exec(Evalstate *state)
{
    state->print("#"+content);
    state->setNext();
}
INPUTsta::INPUTsta(std::string name):name(name)
{
    createTree();
}
void INPUTsta::exec(Evalstate *state)
{
    state->print(name+"=?");
    state->startInput(name);
    state->setNext();
}
INPUTsta::~INPUTsta(){}
void INPUTsta::createTree()
{
    tree.push_back("INPUT");
    tree.push_back(pad4+name);
}
void LETsta::createTree()
{
    tree.push_back("LET = ");
    level.insert(level.begin(),var);
    std::string pad = pad4;
    for(auto &s:level){
        if(s.empty()){
            pad += pad4;
        }else{
            tree.push_back(pad+s);
        }
    }
}
void PRINTsta::createTree()
{
    tree.push_back("PRINT");
    std::string pad = pad4;
    for(auto &s:level){
        if(s.empty()){
            pad += pad4;
        }else{
            tree.push_back(pad+s);
        }
    }
}

void REMsta::createTree()
{
    tree.push_back("REM");
    tree.push_back("    " + content);
}

void GOTOsta::createTree()
{
    std::stringstream ss;
    ss<<lineNumber;
    std::string n;
    ss>>n;
    tree.push_back("GOTO");
    tree.push_back(pad4 + n);
}
void ENDsta::createTree()
{
    tree.push_back("END");
}
void IFsta::createTree()
{
    tree.push_back("IF THEN");

}
