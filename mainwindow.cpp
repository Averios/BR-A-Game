#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MyFrame = nullptr;
    canvas = nullptr;
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(OpenGameWindow()));
}

//void MainWindow::DrawCanvas(){
//    QFrame* mainframe = ui->frame;
//    canvas = new MyCanvas(mainframe, QPoint(0,0), mainframe->size());
//    canvas->show();
//}

void MainWindow::OpenGameWindow(){
    if(MyFrame == nullptr){
        MyFrame = new QFrame;
        MyFrame->setWindowTitle("Game Window");
        MyFrame->setFixedSize(960, 540);

        canvas = new MyCanvas(MyFrame, QPoint(0,0), MyFrame->size());
        connect(MyFrame, SIGNAL(destroyed()), this, SLOT(GameWindowClosed()));
    }
//    this->hide();
    MyFrame->show();
    canvas->show();
}

void MainWindow::GameWindowClosed(){
    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}
