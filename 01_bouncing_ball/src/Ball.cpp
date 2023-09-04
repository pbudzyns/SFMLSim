#include "Ball.hpp"

int WIDTH;
int HEIGHT;
float nSteps;
float timeStep;
sf::Vector2<float> gravity;

/**
 * @brief Mapping window position [[0,Width],[0,Height]] to [[0,1],[1,0]]
 *
 * @param pos
 * @return sf::Vector2<float>
 */
sf::Vector2<float> windowToSim(sf::Vector2<float> pos)
{
    float cx{pos.x / static_cast<float>(WIDTH)};
    float cy{1.0f - pos.y / static_cast<float>(HEIGHT)};
    return {cx, cy};
}
/**
 * @brief Mapping simulation position [[0,1],[1,0]] to [[0,Width],[0,Height]]
 *
 * @param pos
 * @return sf::Vector2<float>
 */
sf::Vector2<float> simToWindow(sf::Vector2<float> pos)
{
    float cx{pos.x * static_cast<float>(WIDTH)};
    float cy{(1.0f - pos.y) * static_cast<float>(HEIGHT)};
    return {cx, cy};
}

Ball::Ball(float radius, sf::Vector2<float> pos)
    : radius(radius), d(2 * radius), shape(radius), pos(windowToSim({pos.x + radius, pos.y + radius})), vel{1.0f, 1.5f},
      wRadius{radius / (float)WIDTH}, hRadius{radius / (float)WIDTH}
{
    shape.setPosition(getPos());
    shape.setFillColor(sf::Color(100, 250, 50));
}

sf::Vector2<float> Ball::getPos()
{
    sf::Vector2<float> posWin{simToWindow(pos)};
    return {posWin.x - radius, posWin.y - radius};
}

void Ball::handleWallCollision()
{
    if (pos.x < wRadius)
    {
        pos.x = wRadius;
        vel.x = -vel.x;
    }
    else if (pos.x > 1.0f - wRadius)
    {
        pos.x = 1.0f - wRadius;
        vel.x = -vel.x;
    }
    if (pos.y < hRadius)
    {
        pos.y = hRadius;
        vel.y = -vel.y;
    }
    else if (pos.y > 1.0f - hRadius)
    {
        pos.y = 1.0f - hRadius;
        vel.y = -vel.y;
    }
}

void Ball::onUpdate()
{
    for (int i{0}; i < (int)nSteps; ++i)
    {
        vel += gravity * timeStep / nSteps;
        pos += vel * timeStep / nSteps;
    }

    handleWallCollision();
    shape.setPosition(getPos());
}
