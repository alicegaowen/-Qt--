#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


      //获取可用桌面大小
//     QRect deskRect = QApplication::desktop()->availableGeometry();
//      w.move( deskRect.x(), deskRect.y() );
//      w.resize( deskRect.right() - deskRect.x(), deskRect.bottom() - deskRect.y());



    return a.exec();
}
