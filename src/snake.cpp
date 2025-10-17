#include "snake.h"

Snake::Snake(int startX, int startY){
    body.push_front({startX, startY});
}

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
    auto[hx, hy] = body.front();
    int newX = dx + hx;
    int newY = dy + hy;

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