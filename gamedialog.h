#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>
#include "mycanvas.h"

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameDialog(QWidget *parent = 0);
    ~GameDialog();

private:
    Ui::GameDialog *ui;
    MyCanvas* theCanvas;
    bool initialized;
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *k);
public slots:
    void SendMessage();
};

#endif // GAMEDIALOG_H
