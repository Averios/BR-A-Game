#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include "mycanvas.h"

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameDialog(QWidget *parent = 0);
    void connectTo(QString IP, quint16 port);
    ~GameDialog();

private:
    Ui::GameDialog *ui;
    MyCanvas* theCanvas;
    bool initialized;
    QTcpSocket socket;

    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *k);
public slots:
    void SendMessage();
    void ReadyToRead();
};

#endif // GAMEDIALOG_H
