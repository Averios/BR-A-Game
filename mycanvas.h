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
    void addBullet(sf::Vector2f position, float angle);
    void SetPosition(int character, sf::Vector2f position);
    void refocusCamera();
    void setPlayerNumber(int number);
    void startGame();
    void finishGame();
    bool isPlaying();

private:
    void OnInit();
    void OnUpdate();

    int playerNumber;
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
    bool playing;
    QQueue<QPair< sf::Vector2f, int> > movementQueue;
    int moveCounter;

    sf::View standard;
    QWidget* chatWidget;
};

#endif // MYCANVAS_H
