#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>
#include <QObject>

class Bullet : public QObject
{
    Q_OBJECT
public:
    explicit Bullet(sf::Vector2f position, float angle, QObject* parent = 0);
    void update(float time);
    void setSpeed(double speed);
    sf::Sprite texture;
    //sf::Texture text;
private:
    float angle;
    double speed;
};

#endif // BULLET_H
