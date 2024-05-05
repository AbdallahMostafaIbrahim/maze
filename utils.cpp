#include "utils.h"

Utils::Utils() {}

// Checks if the x and y are inside the bounds and makes sure that point is not a wall meaning it is 0.
bool Utils::isValidMove(int x, int y, const std::vector<std::vector<int> > &grid)
{
    return x >= 0 && x < grid.size() && y >= 0 && y < grid.size() && grid[x][y] == 0;
}

QPoint Utils::convertPoint(QPoint p, float block_size)
{
    return QPoint(p.x() * block_size, p.y() * block_size);
}


