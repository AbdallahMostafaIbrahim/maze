#include "enemy.h"
#include "utils.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QApplication>

Enemy::Enemy(QPoint pos, float bs, const std::vector<std::vector<int>>& g) : QGraphicsPixmapItem(QPixmap(":/images/ghost.png").scaled(bs,bs)) {
    // Initial Current Position
    currentPos = pos;
    block_size = bs;
    grid = &g;
    timer = new QTimer();

    // Set Current Position
    setPos(Utils::convertPoint(currentPos, bs));
    // Initialize a random direction to point to
    generateRandomDirection();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(170);
}

bool Enemy::isValidMove(int x, int y)
{
    return x >= 0 && x < grid->size() && y >= 0 && y < grid->size() && (*grid)[x][y] == 0;
}

// Randomly sets the direction to either up, down, right, or left.
void Enemy::generateRandomDirection() {
    int rand = QRandomGenerator::global()->bounded(4);
    switch(rand) {
    case 0:
        direction = UP;
        break;
    case 1:
        direction = DOWN;
        break;
    case 2:
        direction = RIGHT;
        break;
    case 3:
        direction = LEFT;
        break;
    }
}

// Function that gets called every 170ms to move the enemy to the random direction
void Enemy::move() {
    // First, it checks if it collides with a player. If so, then it deletes the player and outputs the game over message box.
    QList<QGraphicsItem *> items = collidingItems();
    for(QGraphicsItem* item : items) {
        if(Player* player = dynamic_cast<Player*>(item)) {
            if(scene()) {
                scene()->removeItem(player);
                delete player;
                QMessageBox msgBox;
                msgBox.setWindowTitle("Game Over!");
                msgBox.setText("You lost!");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                QApplication::exit(1000); // 1000 exit code means I want to restart. refer to main.cpp
            }
        }
    }

    bool changedPos = false;
    // Same Idea as the Keypress function in the mazescene
    switch(direction) {
    case UP:
        if(isValidMove(currentPos.y() - 1, currentPos.x() )) {
            currentPos.setY(currentPos.y() - 1);
            changedPos = true;
        }
        break;
    case DOWN:
        if(isValidMove(currentPos.y() + 1, currentPos.x() )) {
            currentPos.setY(currentPos.y() + 1);
            changedPos = true;
        }
        break;
    case RIGHT:
        if(isValidMove(currentPos.y(), currentPos.x() + 1)) {
            currentPos.setX(currentPos.x() + 1);
            changedPos = true;
        }
        break;
    case LEFT:
        if(isValidMove(currentPos.y(), currentPos.x()  - 1)) {
            currentPos.setX(currentPos.x() - 1);
            changedPos = true;
        }
        break;
    }
    // If it hit a dead end, then we want to change its direction
    if(changedPos == false) {
        generateRandomDirection();
        move();
    } else {
        setPos(Utils::convertPoint(currentPos, block_size));
    }
}




