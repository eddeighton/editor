#include "mainwindow.h"
#include <QApplication>

#ifndef Q_MOC_RUN
//#include "common/log.h"
#endif

int main(int argc, char *argv[])
{
    //LoggingSystem log( "" );

    QApplication a(argc, argv);
    a.setApplicationName( "Blueprint Editor" );
    a.setApplicationDisplayName( "Blueprint Editor" );
    a.setWindowIcon( QIcon( ":/art/blueprint.png" ) );
    MainWindow w;
    w.show();

    return a.exec();
}
