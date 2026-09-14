#pragma once
#include <vector>

class GridMap
{
  public:
    GridMap(int width = 30, int height = 30);

    int getWidth() const;
    int getHeight() const;
    int getCell(int x, int y) const;
    bool isWalkable(int x, int y) const;

  private:
    int width_;
    int height_;
    std::vector<std::vector<int>> map_data_;
};