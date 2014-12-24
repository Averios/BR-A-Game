#include "mycanvas.h"
#include <QFrame>
#include <iostream>
#include <string>
#include <QDir>
#include <random>
#include <cmath>

MyCanvas::MyCanvas(QWidget *Parent, const QPoint &Position, const QSize &Size):
    QSFMLCanvas(Parent, Position, Size),
    map("Resources/Tileset/LPC_forest")
{
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

    mySprite.setPosition(mainFrame->size().width() / 2, mainFrame->size().height() /2);
    sf::FloatRect bound = mySprite.getGlobalBounds();
    mySprite.setOrigin(bound.width / 2, bound.height /2);
    map.AddSearchPath("Resources/Tileset/LPC_forest");
    map.Load("exampleMap.tmx");


    for(tmx::MapLayer& layers : map.GetLayers()){
        if(layers.name == "Top"){
            layers.visible = false;
            tops = &layers;
        }
        else if(layers.name == "Spawn"){
            std::random_device rd;
            std::mt19937 mts(rd());
            std::uniform_int_distribution<int> dist(0, layers.objects.size() - 1);
            animated.setPosition(layers.objects.at(dist(mts)).GetPosition());
        }
    }
    tops->visible = true;

    sf::View fixed = this->getView();
    standard = fixed;
    standard.setCenter(animated.getPosition());

    PlayerCollisionPoint.push_back(sf::Vector2f(0.f, 48.f));
    PlayerCollisionPoint.push_back(sf::Vector2f(32.f, 48.f));
    PlayerCollisionPoint.push_back(sf::Vector2f(0.f, 32.f));
    PlayerCollisionPoint.push_back(sf::Vector2f(32.f, 32.f));
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
    map.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 640.f, 640.f));

    for(tmx::MapObject* now : map.QueryQuadTree(animated.getGlobalBounds())){
        if(now->GetName() == "Wall" || now->GetName() == "Edge"){
            bool collide = false;
            for(sf::Vector2f collision : PlayerCollisionPoint){
                if(now->Contains(animated.getPosition() + collision)){
                    collide = true;
                    break;
                }
            }
            if(collide){
                animated.move(-movement * myTime.asSeconds());
                break;
            }
        }
    }

    sf::Vector2f distance = this->getView().getCenter() - animated.getPosition();
    if(fabs(distance.x) > 100.0f || fabs(distance.y) > 100.0f){
        standard.move(movement * myTime.asSeconds());
    }

    if(!directionPressed){
        animated.stop();
    }
    directionPressed = false;

    animated.update(myTime);

    RenderWindow::setView(standard);

    RenderWindow::draw(map);

    RenderWindow::draw(animated);

    RenderWindow::draw(*tops);

//    mySprite.setTextureRect(sf::IntRect(source.x * 32, source.y * 48, 32, 48));

//    RenderWindow::draw(mySprite);
}
