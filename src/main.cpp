#include <ncurses.h>
#include "grid.h"
#include "snake.h"
#include <iostream>
#include <deque>
#include <cstdlib>   
#include <ctime>     
#include <vector>  
#include <algorithm>   
#include <utility> 

enum class Dir { Up, Down, Left, Right };
Dir dir = Dir::Right;
const int GRID_HEIGHT = 20;
const int GRID_WIDTH = 20;
Grid g(GRID_HEIGHT, GRID_WIDTH);
Snake s(10, 10);

int foodX = rand() % 20;
int foodY = rand() % 20;

bool foodPresent = true;

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
    srand(time(0));
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

    auto [hx, hy] = s.getHead();
    if(hx < 0 || hx >= GRID_WIDTH || hy < 0 || hy >= GRID_HEIGHT){
        endwin();
        std::cout << "Game Over! You hit the wall.\n";
        exit(0);
    }

    int i = 0;
    for (auto [x, y] : s.getBody()){
        if(i == 0) {i++; continue;}
        if(hx  == x && hy == y){
            endwin();
            std::cout << "Game over! You bit yourself";
            exit(0);
        }
        i++;
    }

    g.setCell(tx, ty, GRID_CH);
}

void updateFoodPosition(){
        g.setCell(foodX, foodY, FOOD_CH);
}

void updateNewFoodPositions(){
    while (true){
        int x = rand() % 20;
        int y = rand() % 20;

        bool onSnake = false;
        for (auto [sx, sy] : s.getBody()) {
            if (sx == x && sy == y) { onSnake = true; break; }
        }

        if (!onSnake) {
            foodX = x;
            foodY = y;
            break;
        }
    }
    foodPresent = true;
    g.setCell(foodX, foodY, FOOD_CH);  
}

void growSnake(std::pair<int, int> direction){
    if (direction.first == -2 || direction.second == -2) return;
    auto [hx, hy] = s.getHead();
    if (foodX == hx && foodY == hy){
        s.grow(direction.first, direction.second);
        updateNewFoodPositions();
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
        g.setCell(foodX, foodY, FOOD_CH);
        g.draw();
        refresh();
    }

    endwin();
    return 0;
}