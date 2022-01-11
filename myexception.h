#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <iostream>

//在处理token或statement时 根据不同的情况throw exception
//外层catch住 并记录相应的错误信息
class Myexception
{
public:
    Myexception();
    virtual std::string what();
};

// LET a
// IF b THEN 10
class StatementError:public Myexception
{
private:
    std::string sta;
public:
    StatementError(std::string t);
    virtual std::string what() override;
};

// LET 123a = 4
// INPUT IF
class IdentifierError:public Myexception
{
private:
    std::string errorID;
public:
    IdentifierError(std::string e);
    virtual std::string what() override;
};

//LET a = 1+
class ExpressionError:public Myexception
{
private:
    std::string exp;
public:
    ExpressionError(std::string e);
    virtual std::string what() override;
};

//运行时错误
class NameError:public Myexception
{
private:
    std::string name;
public:
    NameError(std::string n);
    virtual std::string what() override;
};

//无对应行号 GOTO　IF
class LineError:public Myexception
{
private:
    std::string lineNumber;
public:
    LineError(std::string n);
    virtual std::string what() override;
};

//输入不是整数
class InputError:public Myexception
{
private:
    std::string input;
public:
    InputError(std::string n);
    virtual std::string what() override;
};
#endif // MYEXCEPTION_H
