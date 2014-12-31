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

    myImage.loadFromFile("Resources/Character/mafuyu.png");
    for(int i = 0; i < 4; i++){
        walkAnimation[i].setSpriteSheet(myImage);
        for(int j = 0; j < 4; j++){
            walkAnimation[i].addFrame(sf::IntRect(j * 32, i * 48, 32, 48));
        }
    }
    bulletTexture.loadFromFile("Resources/Bullet/bullet.png");
    myPlayer = QSharedPointer<Player>(new Player);
    myPlayer.data()->animated = walkAnimation;
    myPlayer.data()->Sprite.setPosition(200.f, 300.f);
//    for(int i = 0; i < 4; i++){
//        myPlayer.data()->animated[i].setSpriteSheet(myImage);
//        for(int j = 0; j < 4; j++){
//            myPlayer.data()->animated[i].addFrame(sf::IntRect(j * 32, i * 48, 32, 48));
//        }
//    }
    myPlayer.data()->setAnimationSequence(Direction::Down);
    myPlayer.data()->playAnimation();
    fireRate = 100.f;
    pushMouse = 0.f;
}
void MyCanvas::OnInit(){
    moveSpeed = 150;



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
    standard.setCenter(myPlayer.data()->getPosition());

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

            myPlayer.data()->setAnimationSequence(Direction::Up);
            myPlayer.data()->updated = true;
            moveString = "WU ";
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            movement.y += moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Down];

            myPlayer.data()->setAnimationSequence(Direction::Down);
            myPlayer.data()->updated = true;
            moveString = "WD ";
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            movement.x += moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Right];

            myPlayer.data()->setAnimationSequence(Direction::Right);
            myPlayer.data()->updated = true;
            moveString = "WR ";
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            movement.x -= moveSpeed;
            directionPressed = true;
            currentAnimetion = &walkAnimation[Direction::Left];

            myPlayer.data()->setAnimationSequence(Direction::Left);
            myPlayer.data()->updated = true;
            moveString = "WL ";
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            pushMouse += myTime.asMilliseconds();
            if(pushMouse >= fireRate){
                pushMouse -= fireRate;
                sf::Vector2f mouse= standard.getCenter()-sf::Vector2f(480,270) + (sf::Vector2f)sf::Mouse::getPosition(*this);

                float y2=mouse.y;float x2=mouse.x;
                float y1=myPlayer.data()->Sprite.getPosition().y; float x1=myPlayer.data()->Sprite.getPosition().x;
                if (x2 < x1) {
                    addBullet(myPlayer.data()->Sprite.getPosition(), atan((y2 - y1)/ (x2 - x1)) + 135);
                    QString message = "F " + QString::number(myPlayer.data()->getPosition().x) + QString(" ") + QString::number(myPlayer.data()->getPosition().y) + QString(" ") + QString::number(atan((y2 - y1)/ (x2 - x1)) + 135) + "\n";
                    socket->write(message.toUtf8());
                }
                else
                {
                    addBullet(myPlayer.data()->Sprite.getPosition(), atan((y2 - y1)/ (x2 - x1)));
                    QString message = "F " + QString::number(myPlayer.data()->getPosition().x) + QString(" ") + QString::number(myPlayer.data()->getPosition().y) + QString(" ") + QString::number(atan((y2 - y1)/ (x2 - x1))) + "\n";
                    socket->write(message.toUtf8());
                }

                //atan2(y2 - y1, x2- x1) * 180 / M_PI <<

                //std::cout << animated.getPosition().x << " " << animated.getPosition().y << std::endl;
                std::cout << x1 << " " << y1 << ", " << x2 << " " << y2 << std::endl;
                //std::cout << " " << atan((y2 - y1)/ (x2 - x1)) << std::endl;
            }
        }
    }

    movement = movement * myTime.asSeconds();
    animated.play(*currentAnimetion);

    updateBullet();

    bool collide = false;
    if(playing){
        for(const tmx::MapObject* now : map.QueryQuadTree(myPlayer.data()->Sprite.getGlobalBounds())){
            if(now->GetName() == "Wall" || now->GetName() == "Edge"){
                if(now->GetAABB().intersects(sf::FloatRect(myPlayer.data()->Sprite.getPosition().x + movement.x, myPlayer.data()->Sprite.getPosition().y + 32.f + movement.y, 32.f, 16.f))){
                    collide = true;
                    break;
                }
            }
        }
    }

    if(!collide){
//        animated.move(movement);
        myPlayer.data()->Sprite.move(movement);
    }

    standard.setCenter(myPlayer.data()->Sprite.getPosition());
//    sf::Vector2f distance = this->getView().getCenter() - myPlayer.data()->getPosition();
//    if(fabs(distance.x) > 100.0f || fabs(distance.y) > 100.0f){
//        standard.move(movement);
//    }

    if(!directionPressed){
        animated.stop();
    }
    else{
        socket->write(moveString.toUtf8() + QString::number(moveCounter).toUtf8() + "\n");

        movementQueue.append(QPair<sf::Vector2f, int>(movement, moveCounter++));
//        socket->flush();
    }
    directionPressed = false;

    animated.update(myTime);

    RenderWindow::setView(standard);

    RenderWindow::draw(map);

//    RenderWindow::draw(animated);
    for(const QSharedPointer<Player> now : PlayerList){
        now.data()->playAnimation();
        if(!now.data()->updated){
            now.data()->Sprite.stop();
        }
        now.data()->updated = false;
        now.data()->Sprite.update(myTime);
        RenderWindow::draw(now.data()->Sprite);
    }

    for (int i=0; i<bullets.size(); i++) {
        //bullets.at(i)->update(myTime.asSeconds());
        RenderWindow::draw(bullets.at(i).data()->texture);
    }

    RenderWindow::draw(*tops);

}

void MyCanvas::getChat(QWidget *chatWidget){
    this->chatWidget = chatWidget;
}

void MyCanvas::startGame(){
    moveCounter = 0;
    this->playing = false;
    this->playing = true;
    movementQueue.clear();
    refocusCamera();
}

void MyCanvas::finishGame(){
    this->playing = false;
}

bool MyCanvas::isPlaying(){
    return this->playing;
}

void MyCanvas::refocusCamera(){
    standard.setCenter(myPlayer.data()->Sprite.getPosition());
    qDebug() << myPlayer.data()->Sprite.getPosition().x << " " << myPlayer.data()->Sprite.getPosition().y;
}

void MyCanvas::setPlayerNumber(int number){
    this->playerNumber = number;
    PlayerMap.insert(number, myPlayer);
    PlayerList.append(myPlayer);
}

void MyCanvas::setSocket(QTcpSocket* socket){
    this->socket = socket;
}

void MyCanvas::SetPosition(int character, sf::Vector2f position, int sequence, int sequenceNumber){
    if(character == playerNumber){
        RecalculatePosition(position, sequence, sequenceNumber);
    }
    else{
        QSharedPointer<Player> thePlayer = PlayerMap.value(character);
//        sf::Vector2f movement = position - thePlayer.data()->Sprite.getPosition();
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
    }
    else{
        thePlayer.data()->setAnimationSequence(moveSequence);
    }
    thePlayer.data()->updated = true;
//    sf::Vector2f diff = position - thePlayer.data()->Sprite.getPosition();
//    standard.move(diff);
    thePlayer.data()->Sprite.setPosition(position);
    lastPoint = position;
}

void MyCanvas::addBullet(sf::Vector2f position, float angle){
    //Add new bullet
    QSharedPointer<Bullet> theBullet(new Bullet(position, angle));
    theBullet.data()->setSpeed(500);
//    theBullet.data()->update(myTime.asSeconds());
    theBullet.data()->texture.setTexture(bulletTexture);
    bullets.append(theBullet);
}

void MyCanvas::addPlayer(int number){
    if(number != playerNumber && !PlayerMap.contains(number)){
        QSharedPointer<Player> thePlayer(new Player);
        thePlayer.data()->animated = walkAnimation;
        PlayerMap.insert(number, thePlayer);
        PlayerList.append(thePlayer);

//        for(int i = 0; i < 4; i++){
//            thePlayer.data()->animated[i].setSpriteSheet(myImage);
//            for(int j = 0; j < 4; j++){
//                thePlayer.data()->animated[i].addFrame(sf::IntRect(j * 32, i * 48, 32, 48));
//            }
//        }
    }
}

void MyCanvas::updateBullet(){
    for(int i=0;i< bullets.size();i++){
        //std::cout <<  bullets.size() << std::endl;
        bullets[i]->update(myTime.asSeconds());
    }
}

int MyCanvas::getPlayerNumber(){
    return playerNumber;
}
