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

const char BODY_CH = '@'; 
const char HEAD_CH = 'O'; 
const char GRID_CH = '.';
const char FOOD_CH = '*';
const std::vector<int> QUIT_CH = {'q', 'Q'}; 
const std::vector<int> RESTART_CH = {'r', 'R'}; 
const std::vector<int> PAUSE_CH = {'p', 'P'}; 


bool foodPresent = true;
bool paused = false;

int timeoutValue = 500;
int score = 0;
int snakeLength = s.getSnakeLength();

void ncursesSetup(){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    srand(time(0));
}

void moveSnake(std::pair<int, int> directionPair){
    if (directionPair.first == -2 || directionPair.second == -2) return;
    auto [tx, ty] = s.getTail();
    s.move(directionPair.first, directionPair.second);
    g.setCell(tx, ty, GRID_CH);
}

void handleCollision(){
    auto [hx, hy] = s.getHead();
    if(hx < 0 || hx > GRID_WIDTH - 1 || hy < 0 || hy > GRID_HEIGHT - 1){
        // mvprintw(GRID_HEIGHT + 1, 0, "Game over! You hit a wall");
        gameState = GameState::GameOver;
    }

    std::deque<std::pair<int, int>> snakeBody = s.getBody();
    if (snakeBody.size() < 4) return;

    int i = 0;
    for (auto [x, y] : snakeBody){
        if(i == 0) {i++; continue;}
        if(hx == x && hy == y){
            // mvprintw(GRID_HEIGHT + 1, 0, "Game over! You bit yourself.");
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

void growSnakeFacingRight(){
    s.growInitialSnake(1, 0); 
    s.growInitialSnake(1, 0); 
}

void growSnakeFacingLeft(){
    s.growInitialSnake(-1, 0); 
    // s.growInitialSnake(-1, 0); 
}

void growSnakeFacingUp(){
    s.growInitialSnake(0, -1); 
    s.growInitialSnake(0, -1); 
}

void growSnakeFacingDown(){
    s.growInitialSnake(0, 1); 
    s.growInitialSnake(0, 1); 
}

void createInitialSnake(){
    s.growInitialSnake(SNAKE_X, SNAKE_Y); 
    for (auto [x, y] : s.getBody()){
        std::cout << "before switch = x: " << x << " y: " << y << "\n";
    }
    switch (dir){
        case Dir::Up: growSnakeFacingUp();
        case Dir::Down: growSnakeFacingDown();
        case Dir::Left: growSnakeFacingLeft();
        case Dir::Right: growSnakeFacingRight();
    }
    for (auto [x, y] : s.getBody()){
        std::cout << "after switch = x: " << x << " y: " << y << "\n";
    }
}

void initialSetUp(){
    createInitialSnake();
    updateFoodPosition();
    updateCellSnakePosition();
    gameState = GameState::InProgress;
    g.draw();
    timeoutValue = 500;
    timeout(timeoutValue);
    score = 0;
    snakeLength = s.getSnakeLength();
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

void updateGameState(){
    int prevLength = snakeLength;
    int currLen = s.getSnakeLength();

    if (currLen > prevLength) {
        int prevTier = std::max(0, (prevLength - 1) / 5);
        int currTier = std::max(0, (currLen - 1) / 5);

        score += 10 * (currTier + 1);

        if (currTier > prevTier) {
            timeoutValue = std::max(60, timeoutValue - 50); 
            timeout(timeoutValue);
        }
    }

    snakeLength = currLen;

    refresh();
}

void growSnake(std::pair<int, int> directionPair){
    if (directionPair.first == -2 || directionPair.second == -2) return;
    auto [hx, hy] = s.getHead();
    if (foodX == hx && foodY == hy){
        s.grow(directionPair.first, directionPair.second);
        score+=10;
        updateGameState();
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

bool handleQuit(int ch) {
    bool quitButtonPressed = std::find(QUIT_CH.begin(), QUIT_CH.end(), ch) != QUIT_CH.end();
    return quitButtonPressed;
}

void handlePause(int ch){
    bool pausedButtonPressed = std::find(PAUSE_CH.begin(), PAUSE_CH.end(), ch) != PAUSE_CH.end();
    if (pausedButtonPressed) paused = !paused;
}

void handleRestart(int ch){
    if(std::find(RESTART_CH.begin(), RESTART_CH.end(), ch) != RESTART_CH.end()) {
        clearSnake();
        s.resetSnake();
        initialSetUp();
    }
}

void handleDirection(int ch){
    std::pair<int, int> directionPair = dirDelta();

    if (ch == KEY_UP && dir != Dir::Down) dir = Dir::Up;
    if (ch == KEY_DOWN && dir != Dir::Up) dir = Dir::Down;
    if (ch == KEY_LEFT && dir != Dir::Right) dir = Dir::Left;
    if (ch == KEY_RIGHT && dir != Dir::Left) dir = Dir::Right;

    moveSnake(directionPair);
    growSnake(directionPair);
}

bool handleKeyPress() {
    int ch = getch();

    if (handleQuit(ch)) return false;

    handlePause(ch);
    if (paused) return true;

    if (gameState == GameState::GameOver) {
        handleRestart(ch);
    } else {
        handleDirection(ch);
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
        mvprintw(GRID_HEIGHT + 3, 0, "Score: %d", score); 
        handleCollision();
        refresh();
    }
    endwin();
    return 0;
}