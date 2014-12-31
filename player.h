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
    sf::Vector2f getPosition();

    void setCooldownBullet(int time);
    bool isReadyBullet();
    void update();
private:
    int sequence;

    int cooldown;
};

#endif // PLAYER_H
