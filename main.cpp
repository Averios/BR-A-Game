#include "mainwindow.h"
#include "mycanvas.h"
#include <QApplication>
#include <QFrame>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow theMain;
    theMain.show();

//    theMain.DrawCanvas();
//    QFrame* mainFrame = new QFrame;

//    mainFrame->setWindowTitle("Damn It!");
//    mainFrame->setFixedSize(960, 540);

//    mainFrame->show();

//    MyCanvas* canvas = new MyCanvas(mainFrame, QPoint(0, 0), mainFrame->size());
//    canvas->show();
    return a.exec();
}
