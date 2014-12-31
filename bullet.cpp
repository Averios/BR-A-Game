#include "bullet.h"

Bullet::Bullet(sf::Vector2f position, float angle, QObject *parent):
    QObject(parent)
{
    this->texture.setPosition(position);
    this->angle = angle;
    //text.loadFromFile("Resources/Bullet/bullet.png");
    //this->speed = 100.f;
    //this->texture.setTexture(text);
    //this->texture.setTextureRect(sf::IntRect(10, 10, 32, 32));
    //std::cout << this->texture.getPosition().x << " " << this->texture.getPosition().y << std::endl;
}

void Bullet::update(float time){
    sf::Vector2f move;
    move.x = time * speed * cos(angle);
    move.y = time * speed * sin(angle);
    //std::cout << this->texture.getPosition().x << " " << this->texture.getPosition().y << std::endl;
    this->texture.move(move);
}

void Bullet::setSpeed(double speed){
    this->speed = speed;
    //this->speed = 100.f;
}
