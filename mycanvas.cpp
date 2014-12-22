#include "mycanvas.h"
#include <QFrame>
#include <iostream>
#include <string>
#include <QDir>

MyCanvas::MyCanvas(QWidget *Parent, const QPoint &Position, const QSize &Size):
    QSFMLCanvas(Parent, Position, Size){
    directionPressed = false;
}
void MyCanvas::OnInit(){
    moveSpeed = 100;
    myImage.loadFromFile("Resources/Character/mafuyu.png");
    QFrame* mainFrame = (QFrame*)parent();
    for(int i = 0; i < 4; i++){
        walkAnimation[i].setSpriteSheet(myImage);
        for(int j = 0; j < 4; j++){
            walkAnimation[i].addFrame(sf::IntRect(j * 32, i * 48, 32, 48));
        }
    }

    mySprite.setTexture(myImage);
    currentAnimetion = &walkAnimation[Direction::Down];

    animated.setFrameTime(sf::seconds(0.15));
    animated.setPosition(mainFrame->size().width() / 2, mainFrame->size().height() /2);

    mySprite.setPosition(mainFrame->size().width() / 2, mainFrame->size().height() /2);
    sf::FloatRect bound = mySprite.getGlobalBounds();
    mySprite.setOrigin(bound.width / 2, bound.height /2);
}

void MyCanvas::OnUpdate(){
    RenderWindow::clear(sf::Color(0, 128, 0));

//    mySprite.rotate(myClock.getElapsedTime().asSeconds() * 100.0f);
    myTime = myClock.restart();
    movement.x = 0.f;
    movement.y = 0.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        movement.y -= moveSpeed;
        source.y = Up;
        directionPressed = true;
        currentAnimetion = &walkAnimation[Direction::Up];
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        movement.y += moveSpeed;
        source.y = Down;
        directionPressed = true;
        currentAnimetion = &walkAnimation[Direction::Down];
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        movement.x += moveSpeed;
        source.y = Right;
        directionPressed = true;
        currentAnimetion = &walkAnimation[Direction::Right];
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        movement.x -= moveSpeed;
        source.y = Left;
        directionPressed = true;
        currentAnimetion = &walkAnimation[Direction::Left];
    }
    animated.play(*currentAnimetion);
    animated.move(movement * myTime.asSeconds());

    if(!directionPressed){
        animated.stop();
    }
    directionPressed = false;

    animated.update(myTime);

    RenderWindow::draw(animated);

//    mySprite.setTextureRect(sf::IntRect(source.x * 32, source.y * 48, 32, 48));

//    RenderWindow::draw(mySprite);
}
