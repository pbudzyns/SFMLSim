#include <SFML/Graphics.hpp>

#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "Vector.hpp"

const int WIDTH{1800};
const int HEIGHT{1000};

constexpr float PI{3.1415926535};

constexpr float simMinWidth{20.0f};
constexpr float cScale{std::min(WIDTH, HEIGHT) / simMinWidth};
constexpr float simWidth{static_cast<float>(WIDTH) / cScale};
constexpr float simHeight{static_cast<float>(HEIGHT) / cScale};

const sf::Vector2<float> gravity{0.f, -10.f};
const float timeStep{.2f / 60.f};

float getRandom()
{
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

float getRandom(float scale)
{
    return getRandom() * scale;
}

sf::Vector2<float> cPos(Vector2<float> pos)
{
    float cx{pos.x};
    float cy{HEIGHT - pos.y};
    return {cx, cy};
}

struct Ball
{
    Ball(float radius, Vector2<float> pos)
        : radius(radius), d(2 * radius), shape(radius), pos{pos}, vel{50.0f, 60.0f}, id{count_++}
    {
        shape.setPosition(cPos(pos));
        shape.setFillColor(sf::Color(255, 250, 50));
    }

    Ball(float radius, float mass, Vector2<float> pos, Vector2<float> vel)
        : radius{radius}, d{2 * radius}, shape{radius}, pos{pos}, vel{vel}, id{count_++}
    {
        shape.setPosition(cPos(pos));
        shape.setFillColor(sf::Color(255, 250, 50));
    }
    static int count_;
    float radius;
    float mass;
    float d;
    sf::CircleShape shape;
    Vector2<float> pos;
    Vector2<float> vel;

    void onUpdate()
    {
        // for (int i{0}; i < 5; ++i)
        // {
        //     vel += gravity * timeStep;
        //     pos += vel * timeStep;
        // }

        if (pos.x < radius)
        {
            pos.x = radius;
            vel.x = -vel.x;
        }
        else if (pos.x > WIDTH - radius)
        {
            pos.x = WIDTH - radius;
            vel.x = -vel.x;
        }
        if (pos.y < radius)
        {
            pos.y = radius;
            vel.y = -vel.y;
        }
        else if (pos.y > HEIGHT - radius)
        {
            pos.y = HEIGHT - radius;
            vel.y = -vel.y;
        }
        Vector2<float> posFix{pos.x - radius, pos.y + radius};
        shape.setPosition(cPos(posFix));
    }

    bool operator==(const Ball &other)
    {
        return id == other.id;
    }

private:
    int id;
};
int Ball::count_ = 0;
class PhysicsScene
{
public:
    PhysicsScene(int numBalls)
    {
        for (int i{0}; i < numBalls; ++i)
        {
            float radius{50.f + getRandom(80.f)};
            float mass{PI * radius * radius};
            Vector2<float> pos{getRandom(WIDTH), getRandom(HEIGHT)};
            Vector2<float> vel{-60.f + getRandom(30.f), -60.f + getRandom(30.f)};

            Ball ball{radius, mass, pos, vel};

            balls.push_back(ball);
        }
    }
    void update()
    {
        // for ball in balls
        // move a ball
        // check for collisions with other balls
        // if colided, handle collision
        for (auto &ball : balls)
        {
            move(ball);
            handleCollisionWithOthers(ball);
            handleCollisionWithWall(ball);
        }
    }

    void move(Ball &ball)
    {
        for (float i{0.f}; i < nSteps; i++)
        {
            ball.vel += gravity * dt / nSteps;
            ball.pos += ball.vel * dt / nSteps;
        }
    }

    void handleCollisionWithOthers(Ball &ball)
    {
        for (Ball &other : balls)
        {
            if (other == ball)
            {
                continue;
            }

            Vector2<float> diff{other.pos - ball.pos};
            float d{diff.length()};
            if (d == 0.0f || d > ball.radius + other.radius)
                continue;

            diff /= d;
            float corr{(ball.radius + other.radius - d) / 2};
            ball.pos -= diff * corr;
            other.pos += diff * corr;

            float v1{ball.vel.dot(diff)};
            float v2{other.vel.dot(diff)};

            float m1{ball.mass};
            float m2{other.mass};

            float newV1{(m1 * v1 + m2 * v2 - m2 * (v1 - v2) * restitution) / (m1 + m2)};
            float newV2{(m1 * v1 + m2 * v2 - m1 * (v2 - v1) * restitution) / (m1 + m2)};

            ball.vel += diff * (newV1 - v1);
            other.vel += diff * (newV2 - v2);
        }
    }

    void handleCollisionWithWall(Ball &ball)
    {
    }

    void draw(sf::RenderWindow &window)
    {
        for (auto &ball : balls)
        {
            ball.onUpdate();
            window.draw(ball.shape);
        }
    }
    std::vector<Ball> balls;

private:
    Vector2<float> gravity{0.0f, -300.0f};
    Vector2<float> worldSize{simWidth, simHeight};

    float dt{1.f / 60.0f};
    float restitution{1.0f};

    float nSteps{10.f};

    bool paused{true};
};

int main()
{

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    PhysicsScene scene(5);

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

        scene.update();
        scene.draw(window);

        // end the current frame
        window.display();
    }

    return 0;
}
