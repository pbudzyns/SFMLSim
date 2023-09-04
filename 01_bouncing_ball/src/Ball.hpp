#include <SFML/Graphics.hpp>

class Ball
{
public:
    Ball(float radius, sf::Vector2<float> pos);

    sf::Vector2<float> getPos();

    void handleWallCollision();

    void onUpdate();

    sf::Shape &getShape()
    {
        return shape;
    }

private:
    float radius;
    float wRadius, hRadius;
    float d;
    sf::CircleShape shape;
    sf::Vector2<float> pos;
    sf::Vector2<float> vel;
};
