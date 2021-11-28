#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnLoadCode,&QPushButton::clicked,this,&MainWindow::LOAD);
    connect(ui->btnRunCode,&QPushButton::clicked,this,&MainWindow::RUN);
    connect(ui->btnClearCode,&QPushButton::clicked,this,&MainWindow::CLEAR);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");

    ui->CodeDisplay->append(cmd);
}

void MainWindow::LOAD()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择代码文件");
    if(filePath.isEmpty()){
        return;
    }
    //QString filePath = QString::fromStdString("t1.txt");
    CLEAR();
    program.reset(new Program(filePath.toStdString()));
    for(auto &cmd:program->rawCommands){
        ui->CodeDisplay->append(QString::number(cmd.first)+" "+QString::fromStdString(cmd.second));
    }
}
void MainWindow::RUN()
{
    ui->textBrowser->clear();
    program->getTokens();
    program->getStatements();
    program->exec();
    for(auto &o:program->output){
        ui->textBrowser->append(QString::fromStdString(o));
    }
}
void MainWindow::CLEAR()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
}
