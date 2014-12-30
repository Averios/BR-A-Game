#ifndef PLAYER_H
#define PLAYER_H
#include "AnimatedSprite.hpp"
class Player
{
public:
    Player();
    void playAnimation();
    void stopAnimation();
    void setAnimationSequence(int sequence);
    Animation* Animation;
    AnimatedSprite Sprite;
    bool updated;
    void setCooldownBullet(int time);
    bool isReadyBullet();
    void update();
private:
    int sequence;
    int cooldown;
};

#endif // PLAYER_H
