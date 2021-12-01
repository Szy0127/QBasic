#ifndef STATEMENT_H
#define STATEMENT_H

#include<iostream>
#include<vector>
#include<expression.h>
#include<evalstate.h>
typedef std::vector<std::string> Token;

enum StatementType{LET,IF,PRINT,INPUT,GOTO,END,REM};
/* 程序在program环境中执行 每一条statement会修改变量指 下条执行语句位置
 *
 * LET 获得变量名与值 -> 修改变量名 = 值
 * IF GOTO 获得下条执行语句的位置N -1表示顺序执行 -> 定位下条执行语句
 * PRINT 获得变量名 ->输出值
 * INPUT 获得变量名 -> 等待输入值 变量名=值
 * REM 直接返回
 * END 结束
 *
 *
 *
 *
 */

class Statement
{

public:
    Statement();
    virtual ~Statement()=0;
    int nextNumber;
    std::string var;
    int value;
    bool end;
    //带=0的是纯虚函数 所有派生类必须都实现
    virtual void exec(Evalstate *state)=0;
    virtual Expression* getExp();

    //virtual std::string getVar();
    
};

class LETsta:public Statement
{
private:
    std::string var;
    Expression *exp;
public:
    LETsta(std::string var,Expression *exp);
    virtual ~LETsta();
    virtual Expression * getExp() override;
    virtual void exec(Evalstate *state)override;
};

class PRINTsta:public Statement
{
private:
    Expression *exp;
public:
    PRINTsta(Expression *exp);
    virtual ~PRINTsta();

    virtual Expression * getExp() override;
    virtual void exec(Evalstate *state)override;
};
class GOTOsta:public Statement
{
private:
    int lineNumber;
public:
    GOTOsta(int n);
    virtual ~GOTOsta();
    virtual void exec(Evalstate *state)override;

};

class IFsta:public Statement
{
private:
    std::string op;
    Expression *left;
    Expression *right;
    int lineNumber;
public:
    IFsta(std::string op,Expression *l,Expression *r,int n);
    virtual ~IFsta();
    virtual void exec(Evalstate *state)override;

};
class ENDsta:public Statement
{

public:
    ENDsta();
    virtual ~ENDsta();
    virtual void exec(Evalstate *state)override;

};
class REMsta:public Statement
{

public:
    REMsta();
    virtual ~REMsta();
    virtual void exec(Evalstate *state)override;

};
class INPUTsta:public Statement
{
private:
    std::string name;
public:
    INPUTsta(std::string name);
    virtual ~INPUTsta();
    virtual void exec(Evalstate *state)override;

};
#endif // STATEMENT_H
