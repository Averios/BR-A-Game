#ifndef MYCANVAS_H
#define MYCANVAS_H
#include "qsfmlcanvas.h"

class MyCanvas: public QSFMLCanvas
{
public:
    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);
private:
    void OnInit();
    void OnUpdate();

    sf::Texture myImage;
    sf::Sprite mySprite;
    sf::Clock myClock;    
};

#endif // MYCANVAS_H
