#include "cppqt_project_package/grid_map.hpp"

GridMap::GridMap(int width, int height) : width_(width), height_(height), map_data_(height, std::vector<int>(width, 0))
{
    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            // 테두리
            if (x == 0 || x == width_ - 1 || y == 0 || y == height_ - 1)
            {
                map_data_[y][x] = 1;
            }
            // 벽 1
            else if (x == 6 && y >= 5 && y <= 8)
            {
                map_data_[y][x] = 1;
            }
            // 벽 2
            else if (y == 12 && x >= 12 && x <= 15)
            {
                map_data_[y][x] = 1;
            }
            // 벽 3
            else if (x == 14 && y >= 3 && y <= 6)
            {
                map_data_[y][x] = 1;
            }
        }
    }
}

int GridMap::getWidth() const
{
    return width_;
}
int GridMap::getHeight() const
{
    return height_;
}

// 벽 or 길
int GridMap::getCell(int x, int y) const
{
    // 맵 바깥인 경우 벽으로 간주
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return 1;
    return map_data_[y][x];
}

// 해당 좌표 도달 가능 여부(bool)
bool GridMap::isWalkable(int x, int y) const
{
    return getCell(x, y) == 0;
}