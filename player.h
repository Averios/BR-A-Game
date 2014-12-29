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
private:
    int sequence;
};

#endif // PLAYER_H
