#include <ncurses.h>
#include "grid.h"
#include "snake.h"
#include <iostream>
#include <deque>
#include <cstdlib>   
#include <ctime>     

enum class Dir { Up, Down, Left, Right };
Dir dir = Dir::Right;
Grid g(20, 20);
Snake s(10, 10);

int foodX = rand() % 20;
int foodY = rand() % 20;

const char BODY_CH = '@'; 
const char HEAD_CH = 'O'; 
const char GRID_CH = '.';
const char FOOD_CH = '*';
const std::vector<int> QUIT_CH = {'q', 'Q'}; 

void ncursesSetup(){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
}

std::pair<int, int> handleKeyUp(){
    dir = Dir::Up;
    return {0, -1};
}

std::pair<int, int> handleKeyDown(){
    dir = Dir::Down;
    return {0, 1};
}

std::pair<int, int> handleKeyLeft(){
    dir = Dir::Left;
    return {-1, 0};
}

std::pair<int, int> handleKeyRight(){
    dir = Dir::Right;
    return {1, 0};
}

void moveSnake(std::pair<int, int> direction){
    if (direction.first == -2 || direction.second == -2) return;
    auto [tx, ty] = s.getTail();
    s.move(direction.first, direction.second);
    g.setCell(tx, ty, GRID_CH);
}

void growSnake(std::pair<int, int> direction){
    if (direction.first == -2 || direction.second == -2) return;
    auto [hx, hy] = s.getHead();
    if (foodX == hx && foodY == hy){
        s.grow(direction.first, direction.second);
    }
}

bool handleKeyPress(){
    int ch = getch();
    if(std::find(QUIT_CH.begin(), QUIT_CH.end(), ch) != QUIT_CH.end()) return false;

    std::pair<int, int> direction = {-2, -2};
    if (ch == KEY_UP && dir != Dir::Down) direction = handleKeyUp();
    if (ch == KEY_DOWN && dir != Dir::Up) direction = handleKeyDown();
    if (ch == KEY_LEFT && dir != Dir::Right) direction = handleKeyLeft();
    if (ch == KEY_RIGHT && dir != Dir::Left) direction = handleKeyRight();

    moveSnake(direction);
    growSnake(direction);

    return true;
}

void updateCellState(){
    int i = 0;
    for (auto [x, y] : s.getBody()) {
        char symbol = (i == 0) ? HEAD_CH : BODY_CH;
        g.setCell(x, y, symbol);
        i++;
    }
}

void updateFoodPosition(){
    for (auto [x, y] : s.getBody()) {
        g.setCell(foodX, foodY, FOOD_CH);
    }
}

void initialSetUp(){
    s.grow(1, 0); 
    s.grow(1, 0); 
    updateFoodPosition();
    updateCellState();
    g.draw();
}

int main(){
    ncursesSetup();
    initialSetUp();

    while (true){
        if (!handleKeyPress()) break;
        erase();
        updateCellState();
        g.draw();
        refresh();
    }

    endwin();
    return 0;
}