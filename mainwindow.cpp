#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnLoadCode,&QPushButton::clicked,this,&MainWindow::LOAD);
    connect(ui->btnRunCode,&QPushButton::clicked,this,&MainWindow::RUN);
    connect(ui->btnClearCode,&QPushButton::clicked,this,&MainWindow::CLEAR);
    CLEAR();
    //由于控制台的存在 没有程序(string的命令)存在时 也可以执行单行的程序 因此是需要一个程序对象存在的 clear在清除原对象的同时会生成一个新的空program对象
    //program.reset(new SZYQBasic::Program());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    //ui->cmdLineEdit->setText("");
    ui->cmdLineEdit->clear();
    if(cmd.isEmpty()){
        return;
    }
    std::string c = cmd.toStdString();
    std::stringstream ss(c);
    int n = ss.peek();
    std::string num;
    if((n >= '0' && n <= '9') || n == '-'){//1 带行号 插入的代码  因为input的输入也是数字 所以正好一起判断了  但是input的数字可能是负数
        ss>>num;
        n = SZYQBasic::Program::stoi(num);
        if(program->isSuspended()){
            input(n);
            return;
        }
        c.clear();
        getline(ss,c);
        program->appendCMD(n,c);
        showCode();
        return;
    }
    std::string consoleCmd;
    ss>>consoleCmd;
    if(SZYQBasic::validCommand.count(consoleCmd)){
        program->execOne(c);
        showOutput();
        return;
    }
    if(consoleCmd == "LOAD"){
        LOAD();
        return;
    }
    if(consoleCmd == "RUN"){
        RUN();
        return;
    }
    if(consoleCmd == "CLEAR"){
        CLEAR();
        return;
    }
    if(consoleCmd == "HELP"){
        QMessageBox::information(this, "帮助", "控制台作用:\n1 LOAD RUN CLEAR HELP QUIT\n2 行号+代码插入到程序段\n3 代码 直接执行\n4 程序中input语句的交互");
        return;
    }
    if(consoleCmd == "QUIT"){
        exit(0);
    }
    QMessageBox::warning(this, "错误", "输入错误 请输入HELP查看合法命令");


}

void MainWindow::LOAD()
{

    QString filePath = QFileDialog::getOpenFileName(this,"选择代码文件");
    if(filePath.isEmpty()){
        return;
    }

    //QString filePath = QString::fromStdString("errorTest.txt");
    CLEAR();
    program.reset(new SZYQBasic::Program(filePath.toStdString()));
    showCode();
}
void MainWindow::input(int n)
{
    program->continueExec(n);
    showOutput();
}
void MainWindow::RUN()
{
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    if(!program){
        QMessageBox::warning(this, "错误", "请先加载程序");
        return;
    }
    //这里run是开始了程序 但是遇到input时控制会返回 所以exec结束 showOutput
    //但是这是应该是等待用户输入 应当开启下一轮的exec 因此在调用input后仍需showOutput
    program->exec();
    showOutput();
    showTree();
}
void MainWindow::CLEAR()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    program.reset(new SZYQBasic::Program());
}
void MainWindow::showCode()
{
    ui->CodeDisplay->clear();
    for(auto &cmd:program->rawCommands){
        ui->CodeDisplay->append(QString::number(cmd.first)+" "+QString::fromStdString(cmd.second));
    }
}

void MainWindow::showOutput()
{
    ui->textBrowser->clear();
    for(auto &o:program->getOutput()){
        ui->textBrowser->append(QString::fromStdString(o));
    }
}

void MainWindow::showTree(){
    bool first;
    for(auto &sta:program->statements){
        first = true;
        //ui->treeDisplay->append(QString::number(sta.first));
        for(auto &line:sta.second->getTree()){
            if(first){
                ui->treeDisplay->append(QString::number(sta.first) + " "+QString::fromStdString(line));
                first = false;
            }else{
                ui->treeDisplay->append(QString::fromStdString(line));
            }
        }
    }
    for(auto &err:program->error){
        ui->treeDisplay->append(QString::number(err.first) + " "+QString::fromStdString(err.second));
    }

}
