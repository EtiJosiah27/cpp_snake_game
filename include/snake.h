#pragma once
#include <deque>
#include <utility>

class Snake{
    public:
        Snake();

        void move(int dx, int dy);

        void growInitialSnake(int dx, int dy);

        void grow(int dx, int dy);

        std::pair<int, int> getHead() const;

        std::pair<int, int> getTail() const;

        const std::deque<std::pair<int, int>>& getBody() const;

        void resetSnake();

    private:
        std::deque<std::pair<int, int>> body;
};