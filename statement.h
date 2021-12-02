#ifndef STATEMENT_H
#define STATEMENT_H

#include<iostream>
#include<vector>
#include<expression.h>
#include<evalstate.h>


typedef std::vector<std::string> Token;
typedef std::vector<std::string> Tree;
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
protected:
    static const std::string pad4;
    Tree tree;//语法树
public:
    Statement();
    virtual ~Statement()=0;
    //带=0的是纯虚函数 所有派生类必须都实现
    virtual void exec(Evalstate *state)=0;
    virtual void createTree()=0;
    Tree getTree();
    virtual SZYExp::Expression* getExp();

    //virtual std::string getVar();
    
};

class LETsta:public Statement
{
private:
    std::string var;
    SZYExp::Expression *exp;
    std::vector<std::string> level;//exp层次遍历的结果

    virtual void createTree()override;
public:
    LETsta(std::string var,SZYExp::Expression *exp);
    virtual ~LETsta();
    virtual SZYExp::Expression * getExp() override;
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;
};

class PRINTsta:public Statement
{
private:
    SZYExp::Expression *exp;
    std::vector<std::string> level;
    virtual void createTree()override;
public:
    PRINTsta(SZYExp::Expression *exp);
    virtual ~PRINTsta();

    virtual SZYExp::Expression * getExp() override;
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;
};
class GOTOsta:public Statement
{
private:
    int lineNumber;
    virtual void createTree()override;
public:
    GOTOsta(int n);
    virtual ~GOTOsta();
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;

};

class IFsta:public Statement
{
private:
    std::string op;
    SZYExp::Expression *left;
    SZYExp::Expression *right;
    int lineNumber;
    std::vector<std::string> levelLeft;
    std::vector<std::string> levelRight;
    virtual void createTree()override;
public:
    IFsta(std::string op,SZYExp::Expression *l,SZYExp::Expression *r,int n);
    virtual ~IFsta();
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;

};
class ENDsta:public Statement
{
private:
    virtual void createTree()override;
public:
    ENDsta();
    virtual ~ENDsta();
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;

};
class REMsta:public Statement
{
private:
    std::string content;
    virtual void createTree()override;
public:
    REMsta(std::string c);
    virtual ~REMsta();
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;

};
class INPUTsta:public Statement
{
private:
    std::string name;
    virtual void createTree()override;
public:
    INPUTsta(std::string name);
    virtual ~INPUTsta();
    virtual void exec(Evalstate *state)override;
    //virtual Tree getTree()override;

};
#endif // STATEMENT_H
