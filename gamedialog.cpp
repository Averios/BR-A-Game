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
        theCanvas->setSocket(socket.socketDescriptor());
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
        else if(stream.at(0) == 'W'){
            QStringList message = stream.split(" ");
            int moveSequence;
            if(message.at(2) == "D"){
                moveSequence = 0;
            }
            else if(message.at(2) == "L"){
                moveSequence = 1;
            }
            else if(message.at(2) == "R"){
                moveSequence = 2;
            }
            else if(message.at(2) == "U"){
                moveSequence = 3;
            }
            theCanvas->SetPosition(message.at(1).toInt(), sf::Vector2f(message.at(3).toFloat(), message.at(4).toFloat()), moveSequence, message.at(5).toInt());
        }
        else if(stream.at(0) == 'P'){
            QStringList message = stream.split(" ");
            theCanvas->setPlayerNumber(message.at(1).toInt());
        }
        else if(stream.contains("GS")){
            theCanvas->startGame();
        }
//    }
}

GameDialog::~GameDialog()
{
    delete ui;
}
