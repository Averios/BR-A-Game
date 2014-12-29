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
    playing = false;
}
void MyCanvas::OnInit(){
    moveSpeed = 100;

    myImage.loadFromFile("Resources/Character/mafuyu.png");
    for(int i = 0; i < 4; i++){
        walkAnimation[i].setSpriteSheet(myImage);
        for(int j = 0; j < 4; j++){
            walkAnimation[i].addFrame(sf::IntRect(j * 32, i * 48, 32, 48));
        }
    }

    //mySprite.setTexture(myImage);
    currentAnimetion = &walkAnimation[Direction::Down];

    animated.setFrameTime(sf::seconds(0.15));

    //mySprite.setPosition(mainFrame->size().width() / 2, mainFrame->size().height() /2);
    //sf::FloatRect bound = mySprite.getGlobalBounds();
   // mySprite.setOrigin(bound.width / 2, bound.height /2);
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

    sf::Vector2f topLeft = standard.getCenter();
    topLeft.x -= 480.f;
    topLeft.y -= 270.f;
    map.UpdateQuadTree(sf::FloatRect(topLeft.x, topLeft.y, 960.f, 540.f));
}

void MyCanvas::OnUpdate(){
    RenderWindow::clear(sf::Color(0, 128, 0));
//    mySprite.rotate(myClock.getElapsedTime().asSeconds() * 100.0f);
    myTime = myClock.restart();
    movement.x = 0.f;
    movement.y = 0.f;
    if(!chatWidget->hasFocus() && this->isActiveWindow() && playing){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            movement.y -= moveSpeed;
            source.y = Up;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Up];
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            movement.y += moveSpeed;
            source.y = Down;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Down];
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            movement.x += moveSpeed;
            source.y = Right;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Right];
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            movement.x -= moveSpeed;
            source.y = Left;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Left];
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f mouse= standard.getCenter()-sf::Vector2f(480,270) + (sf::Vector2f)sf::Mouse::getPosition(*this);
            std::cout << "the left button was pressed" << std::endl;
            std::cout << "mouse x: " << mouse.x << std::endl;
            std::cout << "mouse y: " << mouse.y << std::endl;

            std::cout << animated.getPosition().x << " " << animated.getPosition().y << std::endl;
        }
    }

    movement = movement * myTime.asSeconds();
    animated.play(*currentAnimetion);
//    animated.move(movement);
//    map.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 640.f, 640.f));

//    for(tmx::MapObject* now : map.QueryQuadTree(animated.getGlobalBounds())){
//        if(now->GetName() == "Wall" || now->GetName() == "Edge"){
//            bool collide = false;
//            for(sf::Vector2f collision : PlayerCollisionPoint){
//                if(now->Contains(animated.getPosition() + collision)){
//                    collide = true;
//                    break;
//                }
//            }
//            if(collide){
//                animated.move(-movement * myTime.asSeconds());
//                break;
//            }
//        }
//    }


    bool collide = false;
    for(const tmx::MapObject* now : map.QueryQuadTree(animated.getGlobalBounds())){
        if(now->GetName() == "Wall" || now->GetName() == "Edge"){
            if(now->GetAABB().intersects(sf::FloatRect(animated.getPosition().x + movement.x, animated.getPosition().y + 32.f + movement.y, 32.f, 16.f))){
                collide = true;
                break;
            }
        }
    }
    if(!collide){
        animated.move(movement);
    }

    sf::Vector2f distance = this->getView().getCenter() - animated.getPosition();
    if(fabs(distance.x) > 100.0f || fabs(distance.y) > 100.0f){
        standard.move(movement);
        sf::Vector2f topLeft = standard.getCenter();
        topLeft.x -= 480.f;
        topLeft.y -= 270.f;
        map.UpdateQuadTree(sf::FloatRect(topLeft.x, topLeft.y, 960.f, 540.f));
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

void MyCanvas::getChat(QWidget *chatWidget){
    this->chatWidget = chatWidget;
}

void MyCanvas::startGame(){
    this->playing = true;
}

void MyCanvas::finishGame(){
    this->playing = false;
}

bool MyCanvas::isPlaying(){
    return this->playing;
}
