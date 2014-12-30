#ifndef PLAYER_H
#define PLAYER_H
#include "AnimatedSprite.hpp"
#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject* parent = 0);
    void playAnimation();
    void stopAnimation();
    void setAnimationSequence(int sequence);
    Animation* animated;
    AnimatedSprite Sprite;
    bool updated;
private:
    int sequence;
};

#endif // PLAYER_H
