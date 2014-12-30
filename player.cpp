#include "player.h"

Player::Player()
{
    sequence = 0;
    updated = false;
    Sprite.setFrameTime(sf::seconds(0.15));
}

void Player::setAnimationSequence(int sequence){
    this->sequence= sequence;
}

void Player::playAnimation(){
    Sprite.play(Animation[sequence]);
}

void Player::stopAnimation(){
    Sprite.stop();
}
void Player::setCooldownBullet(int time){
    this->cooldown=time;
}

bool Player::isReadyBullet(){
    if(this->cooldown>0)return false;
    else return true;
}
void Player::update(){
    if(this->cooldown>0)this->cooldown--;
}
