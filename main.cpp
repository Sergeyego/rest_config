#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include "rest/restlogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/ico.ico"));
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    RestLogin d(QObject::tr("Настройка REST API"));
    if (d.exec()!=QDialog::Accepted) exit(1);

    MainWindow w;
    w.show();
    return a.exec();
}
