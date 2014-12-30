#include "bullet.h"

Bullet::Bullet(sf::Vector2f position, float angle, QObject *parent):
    QObject(parent)
{
    this->texture.setPosition(position);
    this->angle = angle;
}

void Bullet::update(float time){
    sf::Vector2f move;
    move.x = time * speed * cos(angle);
    move.y = time * speed * sin(angle);
    this->texture.move(move);
}

void Bullet::setSpeed(double speed){
    this->speed = speed;
}
