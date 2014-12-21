#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include "mycanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    void DrawCanvas();
private:
    Ui::MainWindow *ui;
    QFrame* MyFrame;
    MyCanvas* canvas;
public slots:
    void OpenGameWindow();
    void GameWindowClosed();
};

#endif // MAINWINDOW_H
