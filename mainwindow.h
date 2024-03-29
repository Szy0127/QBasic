#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <program.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdLineEdit_editingFinished();
private:

    void LOAD();
    void RUN();
    void CLEAR();
    void input(std::string n);
    void showCode();
    void showOutput();
    void showTree();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<SZYQBasic::Program> program;
};
#endif // MAINWINDOW_H
