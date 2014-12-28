#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = nullptr;
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(OpenGameWindow()));
//    MyFrame = new QFrame;
//    MyFrame->setWindowTitle("Game Window");
//    MyFrame->setFixedSize(960, 540);

//    canvas = new MyCanvas(MyFrame, QPoint(0,0), MyFrame->size());
//    connect(MyFrame, SIGNAL(destroyed(QObject*)), this, SLOT(GameWindowClosed()));
}

//void MainWindow::DrawCanvas(){
//    QFrame* mainframe = ui->frame;
//    canvas = new MyCanvas(mainframe, QPoint(0,0), mainframe->size());
//    canvas->show();
//}

void MainWindow::OpenGameWindow(){
//    if(canvas == nullptr){
//        canvas = new MyCanvas(MyFrame, QPoint(0,0), MyFrame->size());
//        connect(MyFrame, SIGNAL(destroyed(QObject*)), this, SLOT(GameWindowClosed()));
//    }
//    this->hide();
//    MyFrame->show();
//    canvas->show();
    GameDialog* theDialog = new GameDialog;
    theDialog->setWindowTitle("FP Progjar");
    theDialog->show();
    this->hide();
}

void MainWindow::GameWindowClosed(){
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
    delete MyFrame;
}
