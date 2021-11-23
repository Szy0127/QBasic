#include "mainwindow.h"

#include <QApplication>
#include <program.h>
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}
int main(int argc, char *argv[])
{
    Program t1("t1.txt");
    t1.getTokens();
    t1.getStatements();
    t1.exec();
}
