#ifndef MYCANVAS_H
#define MYCANVAS_H
#include "qsfmlcanvas.h"
#include "AnimatedSprite.hpp"
#include <tmx/MapLoader.h>
class MyCanvas: public QSFMLCanvas
{
public:
    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);
    void getChat(QWidget* chatWidget);
private:
    void OnInit();
    void OnUpdate();

    sf::Texture myImage;
    //sf::Sprite mySprite;
    sf::Clock myClock;
    sf::Time myTime;
    sf::Event events;
    enum Direction{Down, Left, Right, Up};
    bool directionPressed;
    sf::Vector2i source;
    sf::Vector2f movement;
    float moveSpeed;
    Animation walkAnimation[4];
    Animation* currentAnimetion;
    AnimatedSprite animated;
    std::vector<AnimatedSprite*> bullets;
    tmx::MapLoader map;
    tmx::MapLayer* tops;

    sf::View standard;
    QWidget* chatWidget;
};

#endif // MYCANVAS_H
