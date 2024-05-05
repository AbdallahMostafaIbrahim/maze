#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsView>
#include "mazescene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsView view;
    view.setFixedSize(800, 800);
    MazeScene scene;

    view.setScene(&scene);

    view.show();

    return a.exec();
}
