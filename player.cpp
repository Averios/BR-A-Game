#include "player.h"

Player::Player(QObject *parent):
    QObject(parent)
{
    sequence = 0;
    updated = false;
    Sprite.setFrameTime(sf::seconds(0.15));
}

void Player::setAnimationSequence(int sequence){
    this->sequence= sequence;
    Sprite.setAnimation(animated[sequence]);
}

void Player::playAnimation(){
    Sprite.play(animated[sequence]);
}

void Player::stopAnimation(){
    Sprite.stop();
}

sf::Vector2f Player::getPosition(){
    return Sprite.getPosition();
}
