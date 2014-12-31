#ifndef MYCANVAS_H
#define MYCANVAS_H
#include "qsfmlcanvas.h"
#include "AnimatedSprite.hpp"
#include "player.h"
#include "bullet.h"
#include <tmx/MapLoader.h>
#include <QTcpSocket>
#include <QQueue>

class MyCanvas: public QSFMLCanvas
{
public:
    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);
    void getChat(QWidget* chatWidget);
    void addBullet(sf::Vector2f position, float angle);
    void SetPosition(int character, sf::Vector2f position, int sequence, int sequenceNumber);
    void refocusCamera();
    void setPlayerNumber(int number);
    void startGame();
    void finishGame();
    bool isPlaying();
    void setSocket(QTcpSocket* socket);
    void addPlayer(int number);

    void updateBullet();

private:
    void OnInit();
    void OnUpdate();
    void RecalculatePosition(sf::Vector2f position, int moveSequence, int sequenceNumber);

    int playerNumber;
    sf::Texture myImage;
    sf::Texture bulletTexture;
    sf::Clock myClock;
    sf::Time myTime;
    enum Direction{Down, Left, Right, Up};
    bool directionPressed;
    sf::Vector2f lastPoint;
    sf::Vector2f movement;
    float moveSpeed;
    Animation walkAnimation[4];
    Animation* currentAnimetion;
    AnimatedSprite animated;
    QSharedPointer<Player> myPlayer;

    QList<QSharedPointer<Bullet> > bullets;
    QList<QSharedPointer<Bullet> > bulletRemovalList;
//    std::vector < Bullet*> bullets;

    tmx::MapLoader map;
    tmx::MapLayer* tops;
    bool playing;
    QQueue<QPair< sf::Vector2f, int> > movementQueue;
    QMap<int, QSharedPointer<Player> > PlayerMap;
    QList<QSharedPointer<Player> > PlayerList;
    int moveCounter;

    sf::View standard;
    QWidget* chatWidget;
    QTcpSocket* socket;
    QString moveString;
    float fireRate;
    float pushMouse;
};

#endif // MYCANVAS_H
