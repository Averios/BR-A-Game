#include "bullet.h"
#include <iostream>
Bullet::Bullet(sf::Vector2f position, float angle)
{
    this->sprite.setPosition(position);
    this->angle = angle;
    speed= 100;
    text.loadFromFile("Resources/Bullet/bullet.png");
    this->sprite.setTexture(text);
    this->sprite.setTextureRect(sf::IntRect(10, 10, 32, 32));
    //std::cout << this->sprite.getPosition().x << " " << this->sprite.getPosition().y << std::endl;
}

void Bullet::update(float time){
    sf::Vector2f move;
    move.x = time * speed * cos(angle);
    move.y = time * speed * sin(angle);
    //std::cout << this->sprite.getPosition().x << " " << this->sprite.getPosition().y << std::endl;
    this->sprite.move(move);
}
