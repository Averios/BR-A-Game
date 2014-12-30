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
        theCanvas->setSocket(&socket);
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
//    qDebug() << stream;
    for(QString now : stream.split("\n")){
        qDebug() << now;
        if(now == NULL)continue;
        if(now.at(0) == 'C'){
            QStringList message = now.split(" ");
            QString player = "Player" + message.at(1);
            QString realMessage;
            for(int i = 2; i < message.length(); i++){
                realMessage += QString(" ") + message.at(i);
            }
            ui->chatBox->append(player + ":" + realMessage.split("\n").at(0));
        }
        else if(now.at(0) == 'W'){
            QStringList message = now.split(" ");
            int moveSequence;
            if(message.at(0).at(1) == 'D'){
                moveSequence = 0;
            }
            else if(message.at(0).at(1) == 'L'){
                moveSequence = 1;
            }
            else if(message.at(0).at(1) == 'R'){
                moveSequence = 2;
            }
            else if(message.at(0).at(1) == 'U'){
                moveSequence = 3;
            }

            theCanvas->SetPosition(message.at(1).toInt(), sf::Vector2f(message.at(2).toFloat(), message.at(3).toFloat()), moveSequence, now.split(" ").at(4).toInt());
        }
        else if(now.at(0) == 'P'){
            QStringList message = now.split(" ");
            theCanvas->setPlayerNumber(message.at(1).toInt());
        }
        else if(now.contains("GS")){
            theCanvas->startGame();
        }
    }
}

GameDialog::~GameDialog()
{
    delete ui;
}
