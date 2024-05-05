#include <QApplication>
#include <QGraphicsView>
#include "mazescene.h"

int main(int argc, char *argv[])
{
    // Store the exit code
    int result = 0;

    do
    {
        QApplication a(argc, argv);

        // Initialize the view
        QGraphicsView view;
        view.setFixedSize(800, 800);
        view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // Create the maze scene
        MazeScene scene;

        view.setScene(&scene);

        view.show();

        result = a.exec();
    // Keep restarting until exit code is not 1000 (which is not the restart code)
    } while(result == 1000);

    return result;
}
