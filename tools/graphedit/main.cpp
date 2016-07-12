#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QDebug>
#include <QTranslator>
#include <QLibraryInfo>
#include <utils/logs.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
 //_CrtSetDbgFlag(_
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("framework_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    a.setApplicationName("GraphEdit");
    utils::Logger::install();
    MainWindow w;
    w.show();
    if(argc>1)
    {
        qDebug() << argv[1];
        w.loadFile(argv[1]);
    }

    int x = a.exec();
    qDebug() << "Exit app";
    return x;
}
