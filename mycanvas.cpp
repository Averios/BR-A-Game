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
    this->socket = new QTcpSocket;
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
    bulletTexture.loadFromFile("Resources/Image/bullet.png");

    currentAnimetion = &walkAnimation[Direction::Down];

    animated.setFrameTime(sf::seconds(0.15));

    map.AddSearchPath("Resources/Tileset/LPC_forest");
    map.Load("exampleMap.tmx");

    for(tmx::MapLayer& layers : map.GetLayers()){
        if(layers.name == "Top"){
            layers.visible = false;
            tops = &layers;
            break;
        }
    }
    tops->visible = true;

    sf::View fixed = this->getView();
    standard = fixed;
    standard.setCenter(animated.getPosition());

    map.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 3200.f, 3200.f));
}

void MyCanvas::OnUpdate(){
    RenderWindow::clear(sf::Color(0, 128, 0));

    myTime = myClock.restart();
    movement.x = 0.f;
    movement.y = 0.f;
    if(!chatWidget->hasFocus() && this->isActiveWindow() && playing){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            movement.y -= moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Up];
            moveString = "WU";
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            movement.y += moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Down];
            moveString = "WD";
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            movement.x += moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Right];
            moveString = "WR";
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            movement.x -= moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Left];
            moveString = "WL";
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
    }

    if(!directionPressed){
        animated.stop();
    }
    else{
        socket->write(moveString.toUtf8() + QString::number(moveCounter).toUtf8() + "\n");

        movementQueue.append(QPair<sf::Vector2f, int>(movement, moveCounter++));
        socket->flush();
    }
    directionPressed = false;

    animated.update(myTime);

    RenderWindow::setView(standard);

    RenderWindow::draw(map);

    RenderWindow::draw(animated);

    RenderWindow::draw(*tops);

}

void MyCanvas::getChat(QWidget *chatWidget){
    this->chatWidget = chatWidget;
}

void MyCanvas::startGame(){
    moveCounter = 0;
    this->playing = true;
    refocusCamera();
}

void MyCanvas::finishGame(){
    this->playing = false;
}

bool MyCanvas::isPlaying(){
    return this->playing;
}

void MyCanvas::refocusCamera(){
    standard.setCenter(animated.getPosition());
}

void MyCanvas::setPlayerNumber(int number){
    this->playerNumber = number;
    PlayerMap.insert(number, animated);
    PlayerList.append(animated);
}

void MyCanvas::setSocket(qintptr socketDescriptor){
    this->socket->setSocketDescriptor(socketDescriptor);
}

void MyCanvas::SetPosition(int character, sf::Vector2f position, int sequence, int sequenceNumber){
    if(character == playerNumber){
        RecalculatePosition(position, sequence, sequenceNumber);
    }
    else{
        QSharedPointer<Player> thePlayer = PlayerMap.value(character);
        thePlayer.data()->Sprite.setPosition(position);
        thePlayer.data()->setAnimationSequence(sequence);
        thePlayer.data()->updated = true;
    }
}

void MyCanvas::RecalculatePosition(sf::Vector2f position, int moveSequence, int sequenceNumber){
    //Do calculation
    QSharedPointer<Player> thePlayer = PlayerMap.value(this->playerNumber);
    if(!movementQueue.empty()){
        while(sequenceNumber > movementQueue.first().second){
            movementQueue.pop_front();
        }
    }
    if(!movementQueue.empty()){
        int size = movementQueue.size();
        for(int i = 0; i < size; i++){
            bool collided = false;
            for(const tmx::MapObject* obj : map.QueryQuadTree(sf::FloatRect(position.x, position.y + 32.f, 32.f, 16.f))){
                if(obj->GetName() == "Wall" || obj->GetName() == "Edge"){
                    if(obj->GetAABB().intersects(sf::FloatRect(position.x + movementQueue.at(i).first.x, position.y + 32.f + movementQueue.at(i).first.y, 32.f, 16.f))){
                        collided =true;
                        break;
                    }
                }
            }
            if(!collided){
                position += movementQueue.at(i).first;
            }
        }
        thePlayer.data()->Sprite.setPosition(position);
    }
    else{
        thePlayer.data()->Sprite.setPosition(position);
        thePlayer.data()->setAnimationSequence(moveSequence);
    }
    thePlayer.data()->updated = true;
}

void MyCanvas::addBullet(sf::Vector2f position, float angle){
    //Add new bullet
    QSharedPointer<Bullet> theBullet(new Bullet(position, angle));
    theBullet.data()->setSpeed(500);
    theBullet.data()->texture.setTexture(bulletTexture);
    bullets.append(theBullet);
}
