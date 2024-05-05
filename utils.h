#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <QPoint>

class Utils
{
public:
    Utils();
    static bool isValidMove(int x, int y, const std::vector<std::vector<int>>& grid);
    static QPoint convertPoint(QPoint p, float block_size);
};

#endif // UTILS_H
