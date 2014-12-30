#ifndef MYCANVAS_H
#define MYCANVAS_H
#include "qsfmlcanvas.h"
#include "AnimatedSprite.hpp"
#include <tmx/MapLoader.h>
#include "bullet.h"
#include <math.h>
#include <vector>
#include <QQueue>
#include "player.h"
# define M_PI           3.14159265358979323846
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
    void updateBullet();
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
    tmx::MapLoader map;
    tmx::MapLayer* tops;
    bool playing;
    QQueue<QPair< sf::Vector2f, int> > movementQueue;
    int moveCounter;
    std::vector < Bullet*> bullets;
    sf::View standard;
    QWidget* chatWidget;
    Player player;
};

#endif // MYCANVAS_H
