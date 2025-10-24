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
#include <optional>

enum class Dir { Up, Down, Left, Right };
enum class GameState { InProgress, GameOver };
Dir dir = Dir::Right;
GameState gameState = GameState::InProgress;
const int GRID_HEIGHT = 20;
const int GRID_WIDTH = 20;
const int SNAKE_X = 10;
const int SNAKE_Y = 10;
Grid g(GRID_HEIGHT, GRID_WIDTH);
Snake s;

int foodX = rand() % 20;
int foodY = rand() % 20;

bool foodPresent = true;

const char BODY_CH = '@'; 
const char HEAD_CH = 'O'; 
const char GRID_CH = '.';
const char FOOD_CH = '*';
const std::vector<int> QUIT_CH = {'q', 'Q'}; 
const std::vector<int> RESTART_CH = {'r', 'R'}; 

void ncursesSetup(){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    srand(time(0));
    timeout(500);
}

void moveSnake(std::pair<int, int> direction){
    if (direction.first == -2 || direction.second == -2) return;
    auto [tx, ty] = s.getTail();
    s.move(direction.first, direction.second);
    g.setCell(tx, ty, GRID_CH);
}

void handleCollision(){
    auto [hx, hy] = s.getHead();
    if(hx < 0 || hx > GRID_WIDTH - 1 || hy < 0 || hy > GRID_HEIGHT - 1){
        mvprintw(GRID_HEIGHT + 1, 0, "Game over! You hit a wall");
        gameState = GameState::GameOver;
    }

    std::deque<std::pair<int, int>> snakeBody = s.getBody();
    if (snakeBody.size() < 4) return;

    int i = 0;
    for (auto [x, y] : snakeBody){
        if(i == 0) {i++; continue;}
        if(hx == x && hy == y){
            mvprintw(GRID_HEIGHT + 1, 0, "Game over! You bit yourself.");
            gameState = GameState::GameOver;
        }
        i++;
    }
}

void updateFoodPosition(){
    g.setCell(foodX, foodY, FOOD_CH);
}

void updateCellSnakePosition(){
    int i = 0;
    for (auto [x, y] : s.getBody()) {
        char symbol = (i == 0) ? HEAD_CH : BODY_CH;
        g.setCell(x, y, symbol);
        i++;
    }
}

void clearSnake(){
    for (auto [x, y] : s.getBody()) {
        g.setCell(x, y, '.');
    }
}

void createInitialSnake(){
    s.growInitialSnake(SNAKE_X, SNAKE_Y); 
    s.growInitialSnake(1, 0); 
    s.growInitialSnake(1, 0); 
}

void initialSetUp(){
    createInitialSnake();
    updateFoodPosition();
    updateCellSnakePosition();
    gameState = GameState::InProgress;
    g.draw();
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

std::pair<int, int> dirDelta(){
    switch (dir) {
        case Dir::Up: return {0, -1};
        case Dir::Down: return {0, 1};
        case Dir::Left: return {-1, 0};
        case Dir::Right: return {1, 0};
    }
    return {0, 0};
}

bool handleKeyPress() {
    int ch = getch();
    if (std::find(QUIT_CH.begin(), QUIT_CH.end(), ch) != QUIT_CH.end()) return false;

    std::pair<int, int> direction = dirDelta();

    if (gameState == GameState::GameOver) {
        if(std::find(RESTART_CH.begin(), RESTART_CH.end(), ch) != RESTART_CH.end()) {
            clearSnake();
            s.resetSnake();
            initialSetUp();
        }
    } else {
        std::pair<int, int> direction = {-2, -2};
        if (ch == KEY_UP && dir != Dir::Down) dir = Dir::Up;
        if (ch == KEY_DOWN && dir != Dir::Up) dir = Dir::Down;
        if (ch == KEY_LEFT && dir != Dir::Right) dir = Dir::Left;
        if (ch == KEY_RIGHT && dir != Dir::Left) dir = Dir::Right;

        direction = dirDelta();

        moveSnake(direction);
        growSnake(direction);
    }

    return true;
}

int main(){
    ncursesSetup();
    initialSetUp();

    while (true){
        if (!handleKeyPress()) break;
        erase();
        updateCellSnakePosition();
        g.setCell(foodX, foodY, FOOD_CH);
        g.draw();
        handleCollision();
        refresh();
    }

    endwin();
    return 0;
}