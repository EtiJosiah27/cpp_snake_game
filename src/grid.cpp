#include "grid.h"
#include <ncurses.h>

Grid::Grid(int width, int height) 
: width(width), height(height), cells(height, std::vector<char>(width, '.')){}

int Grid::getWidth() const{
    return width;
}

int Grid::getHeight() const{
    return height;
}

void Grid::draw() const{
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            mvaddch(y, x, cells[y][x]);
        }
    }
}

void Grid::setCell(int x, int y, char c){
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cells[y][x] = c;
    }
}



