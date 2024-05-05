#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>

enum Direction {
    UP = 0,
    DOWN,
    RIGHT,
    LEFT
};

class Enemy : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
public:
    Enemy(QPoint, float, const std::vector<std::vector<int>>&);
private:
    Direction direction;
    QPoint currentPos;
    QTimer* timer;
    QTimer* collision_timer;
    const std::vector<std::vector<int>>* grid;
    bool isValidMove(int x, int y);
    void generateRandomDirection();
    float block_size;
private slots:
    void move();

};

#endif // ENEMY_H
