#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../snake.h" 

TEST_CASE("Snake starts with correct head"){
    Snake s;
    s.growInitialSnake(10, 10);

    CHECK(s.getSnakeLength() == 1);

    auto[hx, hy] = s.getHead();
    CHECK(hx == 10);
    CHECK(hy == 10);
}

TEST_CASE("Snake grows to the right"){
    Snake s;
    s.growInitialSnake(10, 10);
    s.growInitialSnake(1, 0);

    CHECK(s.getSnakeLength() == 2);

    auto body = s.getBody();
    auto [hx, hy] = body.front();
    auto [tx, ty] = body.back();

    CHECK(hx == 11);
    CHECK(hy == 10);
    CHECK(tx == 10);
    CHECK(ty == 10);
}