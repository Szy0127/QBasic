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
    int n = ss.peek();
    std::string num;
    if(n >= '0' && n <= '9'){
        ss>>num;
        n = Program::stoi(num);
        getline(ss,c);
        program->appendCMD(n,c);
        showCode();
    }else{
        program->execOne(c);
        showOutput();
    }
}

void MainWindow::LOAD()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择代码文件");
    if(filePath.isEmpty()){
        return;
    }
    //QString filePath = QString::fromStdString("t2.txt");
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
