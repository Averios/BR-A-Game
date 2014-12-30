#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet(sf::Vector2f position, float angle);
    void update(float time);
    sf::Sprite sprite;
    sf::Texture text;
private:
    float angle;
    double speed;
};

#endif // BULLET_H
