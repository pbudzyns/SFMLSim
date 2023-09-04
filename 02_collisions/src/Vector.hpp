#include <cmath>

#include <SFML/Graphics.hpp>

template <typename T>
class Vector2 : public sf::Vector2<T>
{
    typedef sf::Vector2<T> Parent;

public:
    Vector2(T x, T y) : sf::Vector2<T> { x, y }
    {
    }

    Vector2(const sf::Vector2<T> &vec) : sf::Vector2<T> { vec }
    {
    }

    Vector2(const Vector2<T> &vec) : sf::Vector2<T> { vec.x, vec.y }
    {
    }

    Vector2(Vector2<T> &&vec) : sf::Vector2<T> { vec.x, vec.y }
    {
    }

    void set(const Vector2<T> &vec)
    {
        Parent::x = vec.x;
        Parent::y = vec.y;
    }

    float length()
    {
        T x{Parent::x};
        T y{Parent::y};
        return std::sqrt(x * x + y * y);
    }

    float dot(const Vector2<T> other)
    {
        T x{Parent::x};
        T y{Parent::y};
        return x * other.x + y * other.y;
    }

    Vector2<T> &operator+(T val)
    {
        Parent::x += val;
        Parent::y += val;
        return *this;
    }
};
