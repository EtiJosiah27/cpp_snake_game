#pragma once
#include <vector>

class Grid{
    public:
        Grid(int width, int height);

        void draw() const;

        int getWidth() const;
        int getHeight() const;
        void setCell(int x, int y, char c);


    private:
        int width, height;
        std::vector<std::vector<char>> cells;
};