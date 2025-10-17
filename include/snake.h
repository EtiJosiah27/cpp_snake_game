#pragma once
#include <deque>
#include <utility>

class Snake{
    public:
        Snake(int startX, int startY);

        void move(int dx, int dy);

        void grow(int dx, int dy);

        std::pair<int, int> getHead() const;

        std::pair<int, int> getTail() const;

        const std::deque<std::pair<int, int>>& getBody() const;

    private:
        std::deque<std::pair<int, int>> body;
};