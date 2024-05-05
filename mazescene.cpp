#include "mazescene.h"
#include "enemy.h"
#include "utils.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <queue>
#include <QKeyEvent>
#include <QApplication>

// This is used for the priorty queue to compare between two two pairs. We want to compare the first as it is the weight.
struct CostPointComparator {
    bool operator()(const std::pair<float, QPoint>& a, const std::pair<float, QPoint>& b) const {
        return a.first > b.first;
    }
};

MazeScene::MazeScene() {
    // Initializing the grid
    gridSize = 23;
    block_size = 800.0 / gridSize;
    grid = std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize));

    // Reading them map from a file. 0 is free space and 1 is wall.
    QFile file(":/map/file.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Error", "Couldn't load level");
    }
    QTextStream stream(&file);

    setSceneRect(0, 0, 800, 800);

    // Filling the grid from the file stream while adding the rects to the scene
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            stream >> grid[i][j];
            QBrush brush = (grid[i][j] == 0) ? QBrush(Qt::black) : QBrush(Qt::blue);
            addRect(j * block_size, i * block_size, block_size, block_size, QPen(Qt::black), brush);
        }
    }

    // Creating a flag as a finish line on the bottom right corner
    flagPos = QPoint(gridSize - 2, gridSize - 2);
    flag = new QGraphicsPixmapItem(QPixmap((":/images/flag.png")).scaledToWidth(block_size));
    flag->setPos(Utils::convertPoint(flagPos, block_size));
    addItem(flag);

    // Create the player on the top left corner
    playerPos = QPoint(1, 1);
    player = new Player(block_size);
    player->setPos(Utils::convertPoint(playerPos, block_size));
    addItem(player);

    // Create some enemies
    addItem(new Enemy(QPoint(10, 3), block_size, grid));
    addItem(new Enemy(QPoint(10, 5), block_size, grid));
    addItem(new Enemy(QPoint(10, 10), block_size, grid));
    addItem(new Enemy(QPoint(7, 10), block_size, grid));

    // Draw the path to the finish line using dijkstra's algorithim
    dijkstra(playerPos, {gridSize - 2, gridSize - 2});

    file.close();
}

// Draws the path from the player to the finish line using dijkstra's algorithim
void MazeScene::dijkstra(const QPoint& start, const QPoint& goal) {
    // First it clears the original path, if there is any
    clearLines();

    // Initialize the cost 2x2 matrix with really high values except for the start position
    std::vector<std::vector<float>> cost(gridSize, std::vector<float>(gridSize, 9999999));
    cost[start.x()][start.y()] = 0;

    //
    std::priority_queue<std::pair<float, QPoint>, std::vector<std::pair<float, QPoint>>, CostPointComparator> queue;
    queue.push(std::make_pair(0, start));

    // Initialize the parent matrix to reconstruct the path later
    std::vector<std::vector<QPoint>> parent(gridSize, std::vector<QPoint>(gridSize, {-1, -1}));


    // Define possible movement directions: down, up, right, left
    std::vector<QPoint> directions{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // Loop over the priority queue until it's empty
    while (!queue.empty()) {
        float c = queue.top().first;
        QPoint p = queue.top().second;

        queue.pop();

        // Stop if we reached the finish line
        if (p == goal) break;

        // Examine each possible direction
        for (auto& dir : directions) {
            QPoint np = p + dir; // Calculate the new position
            // Check if that position is within the grid and is not a wall (valid)
            if (Utils::isValidMove(np.x(), np.y(), grid)) {
                float newCost = c + 1; // Increment the cost
                // If that cost is lower than the existing cost for that point, then replace it
                if (newCost < cost[np.x()][np.y()]) {
                    cost[np.x()][np.y()] = newCost;
                    parent[np.x()][np.y()] = p; // Record the parent to traverse the path later
                    queue.push({newCost, np}); // Push that new position to the priority queue
                }
            }
        }
    }

    // Draw the path
    QPoint step = goal;
    while (step != start) {
        QPoint prev_step = parent[step.x()][step.y()];
        QGraphicsLineItem* line = new QGraphicsLineItem(
            prev_step.y() * block_size + block_size / 2,
            prev_step.x() * block_size + block_size / 2,
            step.y() * block_size + block_size / 2,
            step.x() * block_size + block_size / 2
            );
        line->setPen(QPen(Qt::green, 2));
        addItem(line);
        step = prev_step;
    }
}

// Deletes any Line Items to clear the path. This is used at the start of the dijstra function
void MazeScene::clearLines()
{
    QList<QGraphicsItem*> itemsToRemove;

    for (QGraphicsItem* item : items()) {
        if (item->type() == QGraphicsLineItem::Type) {
            itemsToRemove.append(item);
        }
    }

    for (QGraphicsItem* item : itemsToRemove) {
        removeItem(item);
        delete item;
    }
}

// Loops over items in scene to delete enemies. This is used when the player wins
void MazeScene::clearEnemies()
{
    QList<QGraphicsItem*> itemsToRemove;

    for (QGraphicsItem* item : items()) {
        if (Enemy* enemy = dynamic_cast<Enemy*>(item)) {
            itemsToRemove.append(item);
        }
    }

    for (QGraphicsItem* item : itemsToRemove) {
        removeItem(item);
        delete item;
    }
}

// Handle Keypresses
void MazeScene::keyPressEvent(QKeyEvent *event)
{
    bool changedPos = false;
    switch(event->key()) {
    // If user pressed on up key, then we check if it is a valid for him to go up. If so, we decrease the Y by 1. Same thing for each direction
    case Qt::Key_Up:
        if(Utils::isValidMove(playerPos.y() - 1, playerPos.x(), grid)) {
            playerPos.setY(playerPos.y() - 1);
            changedPos = true;
        }
        break;
    case Qt::Key_Down:
        if(Utils::isValidMove(playerPos.y() + 1, playerPos.x(), grid)) {
            playerPos.setY(playerPos.y() + 1);
            changedPos = true;
        }
        break;
    case Qt::Key_Right:
        if(Utils::isValidMove(playerPos.y(), playerPos.x() + 1, grid)) {
            playerPos.setX(playerPos.x() + 1);
            changedPos = true;
        }
        break;
    case Qt::Key_Left:
        if(Utils::isValidMove(playerPos.y(), playerPos.x()  - 1, grid)) {
            playerPos.setX(playerPos.x() - 1);
            changedPos = true;
        }
        break;
    }

    // If we changed the QPoint playerPos, then we want to reflect it in the scene.
    if(changedPos) {
        // Change the player's actual position
        player->setPos(Utils::convertPoint(playerPos, block_size));
        // Draw the path to the finish line again
        dijkstra(playerPos.transposed(), flagPos);
        // If the player reaches the flag position, then they won.
        if(playerPos == flagPos) {
            clearEnemies();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Congratulations!");
            msgBox.setText("You win!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            QApplication::quit();
        }
    }
}
