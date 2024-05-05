#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>

// Simple Player Class that inherits from QGraphicsPixmapItem
class Player : public QGraphicsPixmapItem
{
public:
    Player(int bs);
};

#endif // PLAYER_H
