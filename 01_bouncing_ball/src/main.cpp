#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <cmath>

const int WIDTH{1800};
const int HEIGHT{1000};

constexpr float simMinWidth{20.0f};
constexpr float cScale{std::min(WIDTH, HEIGHT)};
constexpr float simWidth{static_cast<float>(WIDTH) / cScale};
constexpr float simHeight{static_cast<float>(HEIGHT) / cScale};

const sf::Vector2<float> gravity{0.f, -10.f};
const float timeStep{1.f / 60.f};
const float nSteps{40.0f};

sf::Vector2<float> cPos(sf::Vector2<float> pos)
{
    float cx{pos.x};
    float cy{HEIGHT - pos.y};
    return {cx, cy};
}

class Simulation
{
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
};

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

struct Ball
{
    Ball(float radius, sf::Vector2<float> pos)
        : radius(radius), d(2 * radius), shape(radius), pos(windowToSim({pos.x + radius, pos.y + radius})), vel{1.0f, 1.5f},
          wRadius{radius / (float)WIDTH}, hRadius{radius / (float)WIDTH}
    {
        shape.setPosition(getPos());
        shape.setFillColor(sf::Color(100, 250, 50));
    }

    sf::Vector2<float> getPos()
    {
        sf::Vector2<float> posWin{simToWindow(pos)};
        return {posWin.x - radius, posWin.y - radius};
    }

    float radius;
    float wRadius, hRadius;
    float d;
    sf::CircleShape shape;
    sf::Vector2<float> pos;
    sf::Vector2<float> vel;
    float r{220.f};
    float b{180.f};

    void handleWallCollision()
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

    void onUpdate()
    {
        for (int i{0}; i < (int)nSteps; ++i)
        {
            vel += gravity * timeStep / nSteps;
            pos += vel * timeStep / nSteps;
        }

        handleWallCollision();
        shape.setPosition(getPos());
    }
};

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    window.setFramerateLimit(60);
    Ball b(50.f, {50.f, 500.f});

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        b.onUpdate();

        // draw everything here...
        window.draw(b.shape);

        // end the current frame
        window.display();
    }

    return 0;
}
