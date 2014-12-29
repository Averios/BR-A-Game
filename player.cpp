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
