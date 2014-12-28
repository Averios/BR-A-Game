#include "gamedialog.h"
#include "ui_gamedialog.h"
#include "mycanvas.h"
#include <QKeyEvent>

GameDialog::GameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameDialog),
    initialized(false)
{
    ui->setupUi(this);
    connect(ui->chatImput, SIGNAL(returnPressed()), this, SLOT(SendMessage()));
    ui->frame->setFocus();
}

void GameDialog::showEvent(QShowEvent *){
    if(!initialized){
        MyCanvas* theCanvas = new MyCanvas(ui->frame, QPoint(0, 0), ui->frame->size());
        theCanvas->show();
        initialized = true;
    }
}

void GameDialog::SendMessage(){
    QString message = ui->chatImput->text();
    //Send the message

    ui->chatImput->clear();
    ui->frame->setFocus();
}

void GameDialog::keyPressEvent(QKeyEvent *k){
    if(ui->frame->hasFocus() && k->key() == Qt::Key_Space){
        ui->chatImput->setFocus();
    }
}

GameDialog::~GameDialog()
{
    delete ui;
}
