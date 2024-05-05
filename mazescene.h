#ifndef MAZESCENE_H
#define MAZESCENE_H

#include <QGraphicsScene>
#include "player.h"

class MazeScene : public QGraphicsScene
{
public:
    MazeScene();
private:
    int gridSize;
    float block_size;
    std::vector<std::vector<int>> grid;
    void dijkstra(const QPoint& start, const QPoint& goal);
    Player* player;
    QGraphicsPixmapItem* flag;
    std::vector<QGraphicsPixmapItem*> path;
    QPoint playerPos;
    QPoint flagPos;
    void clearLines();
    void clearEnemies();
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAZESCENE_H
