#include "gamedialog.h"
#include "ui_gamedialog.h"
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
        theCanvas = new MyCanvas(ui->frame, QPoint(0, 0), ui->frame->size());
        theCanvas->getChat(ui->chatImput);
        theCanvas->show();
        initialized = true;
    }
}

void GameDialog::SendMessage(){
    //Send the message
    socket.write(QString("C ").toUtf8() + ui->chatImput->text().toUtf8() + "\n");
    socket.flush();


    ui->chatImput->clear();
    ui->frame->setFocus();
}

void GameDialog::keyPressEvent(QKeyEvent *k){
    if(!ui->chatImput->hasFocus() && k->key() == Qt::Key_Space){
        ui->chatImput->setFocus();
    }
    else if(ui->chatImput->hasFocus() && k->key() == Qt::Key_Escape){
        ui->chatImput->clear();
        ui->frame->setFocus();
    }
}

void GameDialog::connectTo(QString IP, quint16 port){
    connect(&socket, SIGNAL(readyRead()), this, SLOT(ReadyToRead()));
    socket.connectToHost(IP, port);
}

void GameDialog::ReadyToRead(){
    QByteArray data = socket.readAll();
    QString stream(data);
//    stream = stream.toUtf8();
//    QStringList stringList = stream.split(QRegExp("\n|\r\n|\r"));
//    const int listSize = stringList.size();
//    for(int i = 0; i < listSize; i++){
        if(stream.at(0) == 'C'){
            QStringList message = stream.split(" ");
            QString player = "Player" + message.at(1);
            QString realMessage;
            for(int i = 2; i < message.length(); i++){
                realMessage += QString(" ") + message.at(i);
            }
            ui->chatBox->append(player + ":" + realMessage.split("\n").at(0));
        }
//    }
}

GameDialog::~GameDialog()
{
    delete ui;
}
