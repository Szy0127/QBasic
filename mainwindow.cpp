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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");
    if(cmd.isEmpty() || !program){
        return;
    }
    std::string c = cmd.toStdString();
    std::stringstream ss(c);
    c.clear();
    std::string first;
    ss>>first;
    int n = Program::stoi(first);
    std::string tmp;
    bool run = false;
    if(n == 0 && first!="0"){//不带行号 直接执行
        c += first;
        c += " ";
        run = true;
    }
    while(ss>>tmp){
        c += tmp;
        c += " ";
    }
    c.pop_back();//最后一个空格没必要
    if(!run){
        program->appendCMD(n,c);
        showCode();
    }else{
        program->execOne(c);
        showOutput();
    }


}

void MainWindow::LOAD()
{
//    QString filePath = QFileDialog::getOpenFileName(this,"选择代码文件");
//    if(filePath.isEmpty()){
//        return;
//    }
    QString filePath = QString::fromStdString("t2.txt");
    CLEAR();
    program.reset(new Program(filePath.toStdString()));
    showCode();
}
void MainWindow::RUN()
{
    ui->textBrowser->clear();
    if(!program){
        QMessageBox::information(this, "错误", "请先加载程序");
        return;
    }
    program->exec();
    showOutput();
}
void MainWindow::CLEAR()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
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
