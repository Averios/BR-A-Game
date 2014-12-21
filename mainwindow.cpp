#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::DrawCanvas(){
    QFrame* mainframe = ui->frame;
    canvas = new MyCanvas(mainframe, QPoint(0,0), mainframe->size());
    canvas->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}
