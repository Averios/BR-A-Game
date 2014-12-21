#include "mycanvas.h"
#include <QFrame>
#include <iostream>
#include <string>
#include <QDir>

MyCanvas::MyCanvas(QWidget *Parent, const QPoint &Position, const QSize &Size):
    QSFMLCanvas(Parent, Position, Size){
    QFrame* mainFrame = (QFrame*) parent();

}
void MyCanvas::OnInit(){
    myImage.loadFromFile("Resources/Image/multiplatform.png");

    mySprite.setTexture(myImage);
    QFrame* mainFrame = (QFrame*)parent();
    mySprite.setPosition(mainFrame->size().width() / 2, mainFrame->size().height() /2);
    sf::FloatRect bound = mySprite.getGlobalBounds();
    mySprite.setOrigin(bound.width / 2, bound.height /2);
}

void MyCanvas::OnUpdate(){
    RenderWindow::clear(sf::Color(0, 128, 0));

    mySprite.rotate(myClock.getElapsedTime().asSeconds() * 100.0f);

    RenderWindow::draw(mySprite);
    myClock.restart();
}
