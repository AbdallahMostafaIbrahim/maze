#include "mazescene.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

MazeScene::MazeScene() {
    QFile file(":/map/file.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Error", "Couldn't load level");
    }
    QTextStream stream(&file);
    int block_size = 24;
    int value;
    setSceneRect(0, 0, 800,800);

    for(int i = 0; i < 33; i++) {
        for(int j = 0; j < 33; j++) {
            stream >> value;
            if(value == 0) {
                addRect(j * block_size, i * block_size, block_size, block_size, QPen(Qt::black), QBrush(Qt::black));
            }
            if(value == 1) {
                addRect(j * block_size, i * block_size, block_size, block_size, QPen(Qt::black), QBrush(Qt::yellow));
            }
        }
    }
    file.close();
}
