#include "snake.h"

Snake::Snake(){}

void Snake::move(int dx, int dy){
    if (dx == 0 && dy == 0) return;
    auto[hx, hy] = body.front();
    int newX = dx + hx;
    int newY = dy + hy;

    body.pop_back();
    body.push_front({newX, newY});
}

void Snake::grow(int dx, int dy){
    if (dx == 0 && dy == 0) return;
    auto[hx, hy] = body.back();
    int newX = hx - dx;
    int newY = hy - dy;

    body.push_back({newX, newY});
}

void Snake::growInitialSnake(int dx, int dy){
    if (dx == 0 && dy == 0) return;

    int newX = dx;
    int newY = dy;

    if (!body.empty()){
        auto[hx, hy] = body.front();
        newX = dx + hx;
        newY = dy + hy;
    }
    
    body.push_front({newX, newY});
}

std::pair<int, int> Snake::getHead() const {
    return body.front();
}

std::pair<int, int> Snake::getTail() const {
    return body.back();
}

const std::deque<std::pair<int, int>>& Snake::getBody() const {
    return body;
}

void Snake::resetSnake(){
    body.clear();
}