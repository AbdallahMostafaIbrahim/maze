#include "mazescene.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsPixmapItem>

MazeScene::MazeScene() {
    QFile file(":/map/file.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Error", "Couldn't load level");
    }
    QTextStream stream(&file);
    int block_size = 33;
    int value;
    setSceneRect(0, 0, 800,800);

    for(int i = 0; i < 24; i++) {
        for(int j = 0; j < 24; j++) {
            stream >> value;
            if(value == 0) {
                addRect(j * block_size, i * block_size, block_size, block_size, QPen(Qt::black), QBrush(Qt::black));
            }
            if(value == 1) {
                addRect(j * block_size, i * block_size, block_size, block_size, QPen(Qt::black), QBrush(Qt::blue));
            }
        }
    }

    QGraphicsPixmapItem* flag = new QGraphicsPixmapItem(QPixmap((":/images/flag.png")).scaledToWidth(block_size));
    flag->setPos(22 * block_size, 22 * block_size);
    addItem(flag);

    file.close();
}
