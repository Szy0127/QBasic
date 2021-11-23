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
    virtual void exec(Evalstate *state)=0;
    virtual Expression* getExp()=0;
    //virtual int getNextNumber();
    //virtual std::string getVar();
    
};

class LETsta:public Statement
{
public:
    LETsta(std::string var,Expression *exp);
    virtual ~LETsta();
    std::string var;
    Expression *exp;
    virtual Expression * getExp() override;
    virtual void exec(Evalstate *state);
};

class PRINTsta:public Statement
{
public:
    PRINTsta(Expression *exp);
    virtual ~PRINTsta();
    Expression *exp;
    virtual Expression * getExp() override;
    virtual void exec(Evalstate *state);
};

#endif // STATEMENT_H
